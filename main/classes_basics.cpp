/**
 * 02_classes_basics.cpp — Classes in C++: Encapsulation, Constructors, RAII
 *
 * Compile: g++ -std=c++17 -Wall -o 02_classes_basics 02_classes_basics.cpp
 * Run:     ./02_classes_basics
*/

#include <iostream>
#include <string>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 1 — Why classes? The concept of an INVARIANT
//
// A struct exposes all its data. That's fine when any combination of values
// is valid. But what if some combinations are INVALID?
//
// Example: a BankAccount where balance can never go negative.
// If balance is public, anyone can write: account.balance = -1000;
// A class lets you ENFORCE rules — these are called invariants.
// ─────────────────────────────────────────────────────────────────────────────

class BankAccount {
private:
    // Private: nobody outside this class can touch these directly
    std::string owner;
    double      balance;
    int         account_number;

    static int next_account_number;

public:
    // Static getter — can be called without an instance
    static int get_total_accounts() { return next_account_number; }

    // Constructor: the only way to create a valid BankAccount
    // Enforces: initial balance must be >= 0
    BankAccount(const std::string& owner, double initial_balance)
        : owner(owner), balance(0.0),
          account_number(++next_account_number)
    {
        if (initial_balance < 0.0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
        balance = initial_balance;
        std::cout << "  [+] Account #" << account_number
                  << " opened for " << owner
                  << " ($" << balance << ")\n";
    }

    // Destructor: called automatically when the object goes out of scope
    ~BankAccount() {
        std::cout << "  [-] Account #" << account_number
                  << " (" << owner << ") closed\n";
    }

    // Public interface — the only way to interact with the account
    void deposit(double amount) {
        if (amount <= 0) throw std::invalid_argument("Deposit must be positive");
        balance += amount;
        std::cout << "  Deposited $" << amount
                  << " → balance: $" << balance << "\n";
    }

    void withdraw(double amount) {
        if (amount <= 0)       throw std::invalid_argument("Withdrawal must be positive");
        if (amount > balance)  throw std::runtime_error("Insufficient funds");
        balance -= amount;
        std::cout << "  Withdrew $" << amount
                  << " → balance: $" << balance << "\n";
    }

    // Getters — read-only access to private data
    double      get_balance()        const { return balance; }
    std::string get_owner()          const { return owner;   }
    int         get_account_number() const { return account_number; }

    void print() const {
        std::cout << "  Account #" << account_number
                  << " [" << owner << "] $" << balance << "\n";
    }
};

// Static member must be defined outside the class
int BankAccount::next_account_number = 0;

void section1() {
    std::cout << "\n=== SECTION 1: Why Classes? Invariants. ===\n";

    BankAccount alice("Alice", 1000.0);
    BankAccount bob("Bob", 500.0);

    alice.deposit(200.0);
    alice.withdraw(150.0);

    // alice.balance = -9999;  // ✗ compile error — private!
    // The invariant (balance >= 0) is GUARANTEED by the class.

    try {
        bob.withdraw(9999.0);   // triggers the invariant check
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }

    alice.print();
    bob.print();

    // Static member: shared across instances
    std::cout << "  Total accounts created: "
              << BankAccount::get_total_accounts() << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 2 — Access specifiers in detail
//
//   private:   only this class can access
//   protected: this class AND derived (child) classes can access
//   public:    anyone can access
//
// A class defaults to private. A struct defaults to public.
// ─────────────────────────────────────────────────────────────────────────────

class AccessDemo {
private:
    int private_val = 1;    // only AccessDemo methods can read/write this

protected:
    int protected_val = 2;  // AccessDemo + subclasses can access this

public:
    int public_val = 3;     // anyone can access this

    void show() const {
        // Inside the class, all members are visible regardless of specifier
        std::cout << "  private=" << private_val
                  << " protected=" << protected_val
                  << " public=" << public_val << "\n";
    }
};

// A derived class can access protected but not private
class DerivedDemo : public AccessDemo {
public:
    void touch() {
        // private_val = 99;  // ✗ compile error — private to AccessDemo
        protected_val = 99;   // ✓ protected is accessible in derived class
        public_val    = 100;  // ✓ always accessible
    }
};

void section2() {
    std::cout << "\n=== SECTION 2: Access Specifiers ===\n";

    AccessDemo obj;
    // obj.private_val   = 99;  // ✗ error
    // obj.protected_val = 99;  // ✗ error
    obj.public_val = 99;        // ✓ fine
    obj.show();

    DerivedDemo d;
    d.touch();
    d.show();
}

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 3 — Multiple constructors and the 'this' pointer
//
// A class can have multiple constructors (constructor overloading).
// 'this' is a pointer to the current object — useful to disambiguate
// when parameter names shadow member names.
// ─────────────────────────────────────────────────────────────────────────────

class Rectangle {
private:
    double width;
    double height;

public:
    // Constructor 1: explicit width and height
    Rectangle(double width, double height) {
        // 'this->width' refers to the MEMBER, 'width' refers to the PARAMETER
        // Without 'this->' you'd be assigning width = width (a no-op)
        this->width  = width;
        this->height = height;
    }

    // Constructor 2: square (single side length)
    explicit Rectangle(double side)
        : width(side), height(side) {}   // member initializer list avoids 'this'

    // Constructor 3: default — unit square
    Rectangle() : width(1.0), height(1.0) {}

    double area()      const { return width * height; }
    double perimeter() const { return 2.0 * (width + height); }

    void print() const {
        std::cout << "  Rectangle(" << width << " x " << height << ")"
                  << "  area=" << area()
                  << "  perimeter=" << perimeter() << "\n";
    }
};

void section3() {
    std::cout << "\n=== SECTION 3: Multiple Constructors & 'this' ===\n";

    Rectangle r1(4.0, 3.0);    // constructor 1
    Rectangle r2(5.0);         // constructor 2 — square
    Rectangle r3;              // constructor 3 — default

    r1.print();
    r2.print();
    r3.print();
}

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 4 — RAII: Resource Acquisition Is Initialization
//
// This is the most important C++ idiom. The idea:
//
//   Acquire a resource in the constructor.
//   Release it in the destructor.
//
// Since destructors run automatically when objects go out of scope,
// resources are ALWAYS cleaned up — even if an exception is thrown.
// This is how std::unique_ptr, std::vector, std::mutex etc. all work.
// ─────────────────────────────────────────────────────────────────────────────

class ManagedBuffer {
private:
    int*   data;
    size_t size;
    std::string name;

public:
    // Constructor: ACQUIRE the resource
    ManagedBuffer(size_t size, const std::string& name)
        : data(new int[size]()), size(size), name(name)
    {
        std::cout << "  [+] ManagedBuffer \"" << name
                  << "\" allocated (" << size << " ints)\n";
    }

    // Destructor: RELEASE the resource — called automatically
    ~ManagedBuffer() {
        delete[] data;
        std::cout << "  [-] ManagedBuffer \"" << name << "\" freed\n";
    }

    void fill(int value) {
        for (size_t i = 0; i < size; ++i) data[i] = value;
    }

    int  get(size_t i)          const { return data[i]; }
    void set(size_t i, int val)       { data[i] = val; }
    size_t get_size()           const { return size; }

    void print_first_n(size_t n) const {
        std::cout << "  [";
        for (size_t i = 0; i < n && i < size; ++i) {
            std::cout << data[i];
            if (i + 1 < n && i + 1 < size) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

void risky_function(ManagedBuffer& buf) {
    buf.fill(42);
    throw std::runtime_error("something went wrong mid-function");
    // Even though we throw here, ManagedBuffer's destructor WILL be called.
    // The stack unwinds and all local objects are properly destroyed.
}

void section4() {
    std::cout << "\n=== SECTION 4: RAII ===\n";

    {
        ManagedBuffer buf(8, "primary");
        buf.fill(7);
        buf.print_first_n(8);
    }   // ← destructor called here automatically, memory freed

    std::cout << "  -- Now with an exception --\n";
    try {
        ManagedBuffer buf2(4, "risky");
        risky_function(buf2);   // throws inside
    } catch (const std::exception& e) {
        // buf2's destructor still ran before we got here — no leak!
        std::cout << "  Caught: " << e.what() << "\n";
        std::cout << "  (buf2 was still properly freed — RAII works)\n";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 5 — Static members and methods
//
// Static members belong to the CLASS, not to any individual instance.
// All instances share the same static member.
// Static methods can be called without creating an object.
// ─────────────────────────────────────────────────────────────────────────────

class Counter {
private:
    int id;
    static int instance_count;   // shared across ALL Counter objects

public:
    Counter() : id(++instance_count) {
        std::cout << "  Counter #" << id << " created"
                  << "  (total alive: " << instance_count << ")\n";
    }

    ~Counter() {
        --instance_count;
        std::cout << "  Counter #" << id << " destroyed"
                  << "  (total alive: " << instance_count << ")\n";
    }

    // Static method — no 'this' pointer, no instance needed
    static int get_count() { return instance_count; }
};

int Counter::instance_count = 0;

void section5() {
    std::cout << "\n=== SECTION 5: Static Members ===\n";

    std::cout << "  Count before any: " << Counter::get_count() << "\n";
    {
        Counter a, b, c;
        std::cout << "  Count inside scope: " << Counter::get_count() << "\n";
    }   // a, b, c destroyed here
    std::cout << "  Count after scope: " << Counter::get_count() << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    section1();
    section2();
    section3();
    section4();
    section5();

    return 0;
}
