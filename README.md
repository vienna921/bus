# ecs36bproject
ECS 36B WQ 25 Project 2-4

## Project Status

## Known Issues

## Code References
* [std::to_string] https://en.cppreference.com/w/cpp/string/basic_string/to_string
* [std::make_unique] https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
* [auto (C++)] https://learn.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-170



Student 1: Vienna Tan (925205757)
Student 2: Sophia Chan (925208411)
## Generative AI Use
Student 1 used ChatGPT for Prompt 1, 2, 3

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

### Prompt 3
how do I commit to main but im sharing git with my partner
### Response 3
Totally normal situation — this is exactly what Git is designed for. You can commit to main while sharing a repo, you just need to be disciplined about when and how.

Here’s the clean, low-stress way to do it 👇

The safest workflow (recommended)
1. Always pull first

Make sure you’re up to date before touching main.

git checkout main
git pull


If this pulls changes from your partner → good, you’re synced.

2. Make your changes

Edit code as usual.

Check what you changed:

git status
git diff

3. Commit locally (this does NOT affect your partner yet)
git add .
git commit -m "Describe what you changed"


At this point:

✅ your work is saved

❌ your partner hasn’t seen anything yet

4. Push to shared main
git push origin main


Now your partner can pull your changes.

What your partner should do

They should run:

git pull


That’s it — Git merges automatically if there’s no conflict.

🚨 Very important rules when sharing main
Rule 1: Never force push
 ❌ DO NOT DO THIS
git push --force


This can erase your partner’s work.

Rule 2: Communicate before big changes

If you’re about to:

rename files

change function signatures

restructure directories

Tell your partner first.

If you and your partner edit the SAME file

This can cause conflicts.

What happens:

Git stops the merge

Marks conflict sections in the file

Example:

your code
partner's code


Fix it manually → then:

git add conflicted_file.cpp
git commit

Even safer (best practice, if allowed)

Use feature branches, then merge into main.

git checkout -b xml-parser
 work
git add .
git commit -m "Add XML test reader"
git checkout main
git pull
git merge xml-parser
git push


This avoids stepping on each other.

TL;DR (safe checklist)

Before committing to main:

git checkout main
git pull


After committing:

git push


Never:

force push

push without pulling first

If you want, tell me:

are you both pushing directly to main?

GitHub, GitLab, or local shared repo?

class project or personal?

I’ll tailor a “zero-conflict” workflow for your setup.
### Changes 3
Learned how to use git with partners




## Generative AI Use
Student 2 used ChatGPT for Prompt 1

### Prompt 1
what are example outputs for each function so i have a reference?

bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){

}

bool CSVGWriter::Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
    
}

bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){


}

bool CSVGWriter::GroupBegin(const TAttributes &attrs){

}

bool CSVGWriter::GroupEnd(){
   
}

### Response 1
Got it! Let’s make clear example outputs for each function, based on how your CSVGWriter is implemented and standard SVG syntax. These can go in comments so you always know what each function should produce.

1️⃣ Circle
bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
    // Example output:
    // <circle cx="50" cy="50" r="10" fill="red" stroke="black"/>
}

cx and cy come from center.DX and center.DY
r comes from radius
Any TAttributes from style are added (like fill, stroke, etc.)

2️⃣ Rectangle
bool CSVGWriter::Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
    // Example output:
    // <rect x="10" y="20" width="100" height="50" fill="blue" stroke="black"/>
}

x/y = top-left corner
width/height = from size.DWidth and size.DHeight
style attributes added at the end

3️⃣ Line
bool CSVGWriter::Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
    // Example output:
    // <line x1="0" y1="0" x2="100" y2="100" stroke="black" stroke-width="2"/>
}

x1, y1 = start point
x2, y2 = end point
style attributes added at the end

4️⃣ SimplePath
bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
    // Example output:
    // <path d="M 10 10 L 20 20 L 30 15" stroke="black" fill="none"/>
}

d="..." contains the path instructions
First point = M x y (move to)
Remaining points = L x y (line to)
style attributes appended at the end

5️⃣ GroupBegin
bool CSVGWriter::GroupBegin(const TAttributes &attrs){
    // Example output:
    // <g fill="red" stroke="black">
}

Opens an SVG group <g>
Style attributes go inside the tag
The group will later be closed with GroupEnd()

6️⃣ GroupEnd
bool CSVGWriter::GroupEnd(){
    // Example output:
    // </g>
}

Just closes the group opened by GroupBegin()


### Changes 1
I added comments of these examples for reference



### Prompt 2
am i supposed to write something in a destructor in c++

### Response 2
Short answer: sometimes — but often no.
When you don’t need to write anything
If your class does not manage resources manually, your destructor can be empty or omitted entirely.

Examples:
Only uses stack variables
Uses STL containers (std::vector, std::string, std::map, etc.)
Uses smart pointers (std::unique_ptr, std::shared_ptr)

class Foo {
    int x;
    std::string s;
}; // no destructor needed

The compiler-generated destructor is sufficient.
When you do need to write code in a destructor
You need a destructor if your class owns resources that must be released:
Memory allocated with new / new[]
File handles (FILE*, streams)
Mutexes / locks
Network sockets
Any resource acquired outside RAII containers

class Bar {
    int* data;
public:
    Bar() { data = new int[10]; }
    ~Bar() { delete[] data; }
};

Very important rules
Always match new with delete, new[] with delete[]
If your class has a destructor and manages resources, you likely need:
copy constructor
copy assignment operator
(Rule of Three / Five)
Polymorphism rule (this is commonly tested)
If a class is meant to be used polymorphically, the destructor must be virtual:

class Base {
public:
    virtual ~Base() {}
};

Otherwise, deleting derived objects through a base pointer causes undefined behavior.
Modern C++ best practice
Prefer RAII and smart pointers so your destructor can stay empty:

class Baz {
    std::unique_ptr<int[]> data;
}; // destructor auto-cleans

TL;DR
❌ No manual resources → no destructor needed
✅ Owns resources → destructor frees them
⚠️ Base class → make destructor virtual
💡 Modern C++ → let RAII handle it
If you want, you can paste your class and I’ll tell you exactly whether you need a destructor and what should go in it.

### Changes 2
no changes were made -- just clarification