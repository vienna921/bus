# ecs36bproject
ECS 36B WQ 25 Project 2-4

## Project Status

## Known Issues

## Code References

Student 1: Vienna Tan (925205757)
Student 2: Sophia Chan ()
## Generative AI Use
Student 1 used ChatGPT for Prompt 1, 2

### Prompt 1
what is the basic format for writing xml tests
### Response 1

1️⃣ The BASIC format of an XML test (this is the core)

    Every XML reader test follows this structure:

    TEST(XMLReaderTest, TestName){
        // 1. Create XML input
        auto src = std::make_shared<CStringDataSource>(
            "<root/>"
        );

        // 2. Create reader
        CXMLReader reader(src);

        // 3. Read entities
        SXMLEntity entity;
        bool result = reader.ReadEntity(entity);

        // 4. Verify entity contents
        EXPECT_TRUE(result);
        EXPECT_EQ(entity.DNameData, "root");
    }


    That’s it.
    Everything else is a variation on this theme.

    2️⃣ The universal XML test pattern (memorize this)
    ✅ Step 1 — Input XML as a string
    auto src = std::make_shared<CStringDataSource>(xml_string);


    You never read files in tests.

    ✅ Step 2 — Construct the reader
    CXMLReader reader(src);

    ✅ Step 3 — Repeatedly call ReadEntity()
    SXMLEntity entity;
    while(reader.ReadEntity(entity)){
        // inspect entity
    }


    Or explicitly:

    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_FALSE(reader.ReadEntity(entity));

    ✅ Step 4 — Assert what you got

    You check:

    DType

    DNameData

    attributes

    End-of-stream behavior
### Changes 1
Updated XMLTest.cpp to follow the format

### Prompt 2
how to write struct in C++
### Response 2
Gotchu 👍 — here’s the clean, practical way to think about struct in C++, plus how it differs from class.

1️⃣ Basic struct in C++
struct Point {
    int x;
    int y;
};


Usage:

Point p;
p.x = 3;
p.y = 4;


👉 Key rule:

Members are public by default

2️⃣ struct with constructor

Yes, structs can have constructors (this is C++, not C):

struct Point {
    int x;
    int y;

    Point(int xVal, int yVal) {
        x = xVal;
        y = yVal;
    }
};


Usage:

Point p(3, 4);

3️⃣ struct with methods
struct Point {
    int x;
    int y;

    void Move(int dx, int dy) {
        x += dx;
        y += dy;
    }
};


Usage:

Point p{1, 2};
p.Move(3, 4);

4️⃣ struct vs class (IMPORTANT)

They are almost the same in C++.

Feature	struct	class
Default access	public	private
Can have methods	✅	✅
Can have constructors	✅	✅
Inheritance	✅	✅

Example showing the only difference:

struct A {
    int x;   // public
};

class B {
    int x;   // private
};


That’s it. No other magic difference.

5️⃣ When to use struct (common convention)

Use struct when:

It’s mainly data

You want fields accessible

You want a “plain data object”

Example (very common in systems code):

struct Rect {
    int x;
    int y;
    int width;
    int height;
};


Use class when:

You want encapsulation

Invariants matter

Private data

6️⃣ struct with inheritance
struct Animal {
    void Speak() {
        std::cout << "Animal sound\n";
    }
};

struct Dog : public Animal {
    void Bark() {
        std::cout << "Woof!\n";
    }
};


Usage:

Dog d;
d.Speak();
d.Bark();

7️⃣ struct with default values (C++11+)
struct Config {
    int port = 8080;
    bool debug = false;
};


Usage:

Config c;
### Changes 2
XMLReader.h and XMLReader.cpp structs understanding and implementation






## Generative AI Use
Student 2 used ChatGPT for Prompt 1

### Prompt 1

### Response 1

### Changes 1

