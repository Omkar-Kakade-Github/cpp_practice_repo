/**
 * shared_ptr_examples.cpp
 *
 * A comprehensive tour of std::shared_ptr in modern C++ (C++11 and beyond).
 *
 * Compile:  g++ -std=c++17 -Wall -o shared_ptr_examples shared_ptr_examples.cpp
 * Run:      ./shared_ptr_examples
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Helper: small class to make construction / destruction visible
// ─────────────────────────────────────────────────────────────────────────────
struct Resource {
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "  [+] Resource \"" << name << "\" constructed\n";
    }
    ~Resource() {
        std::cout << "  [-] Resource \"" << name << "\" destroyed\n";
    }
    void greet() const { std::cout << "  Hello from \"" << name << "\"\n"; }
};

// ─────────────────────────────────────────────────────────────────────────────
// 1. BASICS — construction, use_count, reset
// ─────────────────────────────────────────────────────────────────────────────
void basics() {
    std::cout << "\n=== 1. Basics ===\n";

    // Preferred: make_shared allocates object + control block in one shot
    auto sp1 = std::make_shared<Resource>("Alpha");
    std::cout << "  use_count after sp1 created : " << sp1.use_count() << "\n";  // 1

    {
        auto sp2 = sp1;   // copy → shared ownership, ref count becomes 2
        std::cout << "  use_count after sp2 = sp1  : " << sp1.use_count() << "\n";  // 2
        sp2->greet();
    } // sp2 goes out of scope → ref count drops back to 1

    std::cout << "  use_count after sp2 gone   : " << sp1.use_count() << "\n";  // 1

    sp1.reset();  // explicitly release ownership; ref count → 0 → object destroyed
    std::cout << "  use_count after sp1.reset() : " << sp1.use_count() << "\n";  // 0
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. SHARED OWNERSHIP — multiple owners, last one cleans up
// ─────────────────────────────────────────────────────────────────────────────
void shared_ownership() {
    std::cout << "\n=== 2. Shared Ownership ===\n";

    std::shared_ptr<Resource> keeper;

    {
        auto outer = std::make_shared<Resource>("Beta");

        {
            auto inner = outer;  // ref count = 2
            keeper = inner;      // ref count = 3
            std::cout << "  use_count inside inner scope: " << outer.use_count() << "\n";
        } // inner destroyed → ref count = 2

        std::cout << "  use_count after inner gone  : " << outer.use_count() << "\n";
    } // outer destroyed → ref count = 1 (keeper still holds it)

    std::cout << "  use_count after outer gone  : " << keeper.use_count() << "\n";
    keeper->greet();
    // keeper goes out of scope → ref count = 0 → "Beta" is destroyed here
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. SHARING WITH CONTAINERS — shared_ptr in a vector
// ─────────────────────────────────────────────────────────────────────────────
void sharing_with_containers() {
    std::cout << "\n=== 3. Sharing With Containers ===\n";

    auto shared = std::make_shared<Resource>("Gamma");

    std::vector<std::shared_ptr<Resource>> observers;
    observers.push_back(shared);   // ref count = 2
    observers.push_back(shared);   // ref count = 3

    std::cout << "  use_count with vector holding 2 copies: "
              << shared.use_count() << "\n";  // 3

    observers.clear();  // vector released → ref count drops to 1
    std::cout << "  use_count after vector.clear(): " << shared.use_count() << "\n";
    // "Gamma" lives until 'shared' goes out of scope at the end of this function
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. WEAK_PTR — observe without owning (breaks cycles, avoids dangling)
// ─────────────────────────────────────────────────────────────────────────────
void weak_ptr_demo() {
    std::cout << "\n=== 4. weak_ptr — Non-Owning Observer ===\n";

    std::weak_ptr<Resource> observer;

    {
        auto owner = std::make_shared<Resource>("Delta");
        observer = owner;   // weak_ptr does NOT increment use_count
        std::cout << "  use_count (weak_ptr does not affect it): "
                  << owner.use_count() << "\n";  // still 1

        // To use the object, lock() tries to get a shared_ptr
        if (auto locked = observer.lock()) {
            locked->greet();
        }
    } // owner destroyed → "Delta" is gone

    // After owner is destroyed, lock() returns an empty shared_ptr
    if (observer.expired()) {
        std::cout << "  observer.expired() == true: object was deleted\n";
    }
    if (!observer.lock()) {
        std::cout << "  observer.lock() returned null — safe, no crash\n";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. CIRCULAR REFERENCE PROBLEM — and the weak_ptr fix
// ─────────────────────────────────────────────────────────────────────────────
struct NodeBad {
    std::string id;
    std::shared_ptr<NodeBad> next;   // ← causes a memory leak in a cycle
    explicit NodeBad(std::string s) : id(std::move(s)) {
        std::cout << "  [+] NodeBad \"" << id << "\" constructed\n";
    }
    ~NodeBad() {
        std::cout << "  [-] NodeBad \"" << id << "\" destroyed\n";
    }
};

struct NodeGood {
    std::string id;
    std::weak_ptr<NodeGood> next;    // ← weak_ptr breaks the cycle
    explicit NodeGood(std::string s) : id(std::move(s)) {
        std::cout << "  [+] NodeGood \"" << id << "\" constructed\n";
    }
    ~NodeGood() {
        std::cout << "  [-] NodeGood \"" << id << "\" destroyed\n";
    }
};

void circular_reference() {
    std::cout << "\n=== 5. Circular Reference ===\n";

    std::cout << "  -- BAD (memory leak): destructors never called --\n";
    {
        auto a = std::make_shared<NodeBad>("A");
        auto b = std::make_shared<NodeBad>("B");
        a->next = b;
        b->next = a;  // cycle: a→b→a, ref counts never reach 0
    } // a, b go out of scope but destructors are NOT called → leak

    std::cout << "  -- GOOD (weak_ptr breaks cycle): destructors called --\n";
    {
        auto a = std::make_shared<NodeGood>("A");
        auto b = std::make_shared<NodeGood>("B");
        a->next = b;  // weak_ptr, no ref-count bump
        b->next = a;  // weak_ptr, no ref-count bump
    } // ref counts reach 0 correctly → both destructors fire
}

// ─────────────────────────────────────────────────────────────────────────────
// 6. CUSTOM DELETER — useful for C-style resources (FILE*, SDL_*, etc.)
// ─────────────────────────────────────────────────────────────────────────────
void custom_deleter() {
    std::cout << "\n=== 6. Custom Deleter ===\n";

    // Wrap a raw pointer (e.g., from a C API) with a custom cleanup function
    auto file_closer = [](FILE* f) {
        if (f) {
            std::cout << "  Custom deleter: closing FILE*\n";
            fclose(f);
        }
    };

    // Open a temporary file just to demonstrate
    {
        std::shared_ptr<FILE> file(fopen("/tmp/shared_ptr_demo.txt", "w"), file_closer);
        if (file) {
            fputs("shared_ptr with custom deleter\n", file.get());
            std::cout << "  Wrote to file via shared_ptr<FILE>\n";
        }
    } // file_closer is called automatically here
}

// ─────────────────────────────────────────────────────────────────────────────
// 7. ENABLE_SHARED_FROM_THIS — get a valid shared_ptr from inside the class
// ─────────────────────────────────────────────────────────────────────────────
struct Widget : public std::enable_shared_from_this<Widget> {
    std::string label;
    explicit Widget(std::string l) : label(std::move(l)) {
        std::cout << "  [+] Widget \"" << label << "\" constructed\n";
    }
    ~Widget() { std::cout << "  [-] Widget \"" << label << "\" destroyed\n"; }

    // Returns a shared_ptr to *this that shares ownership with existing owners
    std::shared_ptr<Widget> get_self() { return shared_from_this(); }
};

void enable_shared_from_this_demo() {
    std::cout << "\n=== 7. enable_shared_from_this ===\n";

    auto w = std::make_shared<Widget>("Omega");
    auto alias = w->get_self();   // same object, ref count = 2
    std::cout << "  use_count after get_self(): " << w.use_count() << "\n";
    // Both 'w' and 'alias' share the Widget; it is destroyed only once
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    basics();
    shared_ownership();
    sharing_with_containers();
    weak_ptr_demo();
    circular_reference();
    custom_deleter();
    enable_shared_from_this_demo();

    std::cout << "\nAll examples complete.\n";
    return 0;
}
