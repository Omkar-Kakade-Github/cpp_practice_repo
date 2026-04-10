#include <iostream>
#include <string>
#include <cmath>
#include <cstdint>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 1 — The C-style struct: pure data grouping
//
// A struct is just a way to bundle related data under one name.
// In C, that was all structs could do. In C++, they can do much more.
// ─────────────────────────────────────────────────────────────────────────────
 
struct PointC {
    float x;
    float y;
};

void section1() {
    cout << "\n=== SECTION 1: C-Style Struct (pure data) ===\n";

    PointC p;
    p.x = 3.7f;
    p.y = 4.8f;
    
    cout << "Point p : ("<< p.x <<", "<< p.y <<")\n";

    //Aggregate initialization - works when there are no user-defined constructors
    PointC q = {1.1f, 2.2f};
    
    cout << "Point q : ("<< q.x <<", "<< q.y << ")\n";

    // KEY OBSERVATION:
    // Without a struct you'd write: float px, py; — they'd have no logical bond.
    // Structs give related data a name and a home.

};

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 2 — C++ struct: add member functions
//
// In C++, structs can have member functions, constructors, and methods.
// This is the first major difference from C.
// ─────────────────────────────────────────────────────────────────────────────

struct Point2D {
    float x;
    float y;

    // Constructor - initializes the struct cleanly in one line
    Point2D(float x, float y) : x(x), y(y) {}

    // Default constructor - a point at the origin
    Point2D() : x(0.0f),y(0.0f) {}

    // Member function — computes distance from origin
    // 'const' means: "I promise not to modify any members"
    float length() const {
        return sqrt(x * x + y * y);
    }

    Point2D add(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }

    void print() const {
    cout << "("<< x <<", "<< y <<")\n";
    }
                                            
};

void section2() {
    cout << "\n=== SECTION 2: C++ Struct with Member Functions ===\n";
 
    Point2D a(3.0f, 4.0f);
    cout << "  a = "; a.print();
    cout << "  |a| = " << a.length() << "\n";
 
    Point2D b(1.0f, 2.0f);
    Point2D c = a.add(b);
    cout << "  a + b = "; c.print();    
}

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 3 — Default member values (C++11 and beyond)
//
// Members can have default values directly in the definition.
// Reduces constructor boilerplate and makes intent obvious.
// ─────────────────────────────────────────────────────────────────────────────
 
struct Server_Config {
    int    port    = 1000;
    string name    = "base";
    bool   status  = false;
    float  timeout = 30.0f;

    void print() {
        cout <<"Sever Config : "<< endl;
        cout <<"port :                 "<< port << endl;
        cout <<"name :                 "<< name << endl;
        cout <<"online status :        "<< status << endl;
        cout <<"current timeout :      "<< timeout << endl;
    }
};

void section3() {
    cout << "\n=== SECTION 3: Default Member Values ===\n";
 
    Server_Config default_cfg;
    cout <<"\n  Default "; default_cfg.print();

    Server_Config my_server;

    my_server.port = 1927;
    my_server.name = "aws";
    my_server.status = true;

    cout <<"\n  Production "; my_server.print();
}


// ─────────────────────────────────────────────────────────────────────────────
// SECTION 4 — Struct vs Class: the ONE real language difference
//
//   struct → members are PUBLIC  by default
//   class  → members are PRIVATE by default
//
// That's it. Everything else is convention.
//
// CONVENTION (not enforced by the compiler):
//   struct → passive data bundle, all fields public, no invariants
//   class  → object with encapsulation, invariants, complex behavior
// ─────────────────────────────────────────────────────────────────────────────
 
struct  OpenData   { int value; };   // value is public  by default
class   HiddenData { int value; };   // value is private by default
 
void section4() {
    cout << "\n=== SECTION 4: Struct vs Class (the one real difference) ===\n";
 
    OpenData od;
    od.value = 42;   // ✓ compiles — public
    cout << "  OpenData.value  = " << od.value << "\n";
 
    HiddenData hd;
    // hd.value = 42; // ✗ compile error — private by default in class
    (void)hd;
    cout << "  HiddenData.value is private — cannot touch it directly\n";
}
 
// ─────────────────────────────────────────────────────────────────────────────
// SECTION 5 — A realistic struct: 3D Vector
//
// Vec3 is the canonical example of where struct is the right choice:
//   - All data public (x, y, z are meant to be read/written freely)
//   - Pure computation, no hidden state or invariants
//   - Used everywhere in graphics, physics, HPC
// ─────────────────────────────────────────────────────────────────────────────
 
struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
 
    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
 
    float dot(const Vec3& o) const {
        return x*o.x + y*o.y + z*o.z;
    }
 
    Vec3 cross(const Vec3& o) const {
        return Vec3(y*o.z - z*o.y,
                    z*o.x - x*o.z,
                    x*o.y - y*o.x);
    }
 
    float magnitude() const { return sqrt(dot(*this)); }
 
    Vec3 normalized() const {
        float m = magnitude();
        return Vec3(x/m, y/m, z/m);
    }
 
    void print(const string& label = "") const {
        if (!label.empty()) cout << "  " << label << ": ";
        cout << "  (" << x << ", " << y << ", " << z << ")\n";
    }
};
 
void section5() {
    cout << "\n=== SECTION 5: Realistic Struct — Vec3 ===\n";
 
    Vec3 a(1.0f, 0.0f, 0.0f);   // unit X
    Vec3 b(0.0f, 1.0f, 0.0f);   // unit Y
 
    a.print("unit X");
    b.print("unit Y");
    cout << "  dot(X, Y) = " << a.dot(b) << "\n";     // 0.0 — perpendicular
    a.cross(b).print("cross(X, Y)");                         // unit Z: (0, 0, 1)
 
    Vec3 v(3.0f, 1.0f, 4.0f);
    cout << "  |v| = " << v.magnitude() << "\n";
    v.normalized().print("v.normalized()");
}
 
// ─────────────────────────────────────────────────────────────────────────────
// SECTION 6 — Nested structs
//
// Structs can contain other structs, reflecting real-world data hierarchies.
// This is how physics engines, game engines, and HPC frameworks model data.
// ─────────────────────────────────────────────────────────────────────────────
 
struct Color {
    uint8_t r = 255, g = 255, b = 255, a = 255;
};
 
struct Particle {
    Vec3  position;
    Vec3  velocity;
    Color color;
    float mass     = 1.0f;
    float lifetime = 5.0f;   // seconds remaining
 
    // Advance the particle by one timestep dt (Euler integration)
    void update(float dt) {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
        position.z += velocity.z * dt;
        lifetime   -= dt;
    }
 
    bool is_alive() const { return lifetime > 0.0f; }
};
 
void section6() {
    std::cout << "\n=== SECTION 6: Nested Structs — Particle ===\n";
 
    Particle p;
    p.position = Vec3(0.0f, 10.0f, 0.0f);
    p.velocity = Vec3(1.0f, -9.8f,  0.0f);
    p.color    = Color{255, 128, 0, 255};   // orange
 
    p.position.print("t=0.0s position");
    p.update(0.5f);
    p.position.print("t=0.5s position");
    p.update(0.5f);

    p.position.print("t=1.0s position");
    std::cout << "  alive: " << (p.is_alive() ? "yes" : "no") << "\n";
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
    section6();

    return 0;
}
