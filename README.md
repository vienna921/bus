# ecs36bproject
ECS 36B WQ 25 Project 2-4

## Project Status

## Known Issues

## Code References
* [std::to_string] https://en.cppreference.com/w/cpp/string/basic_string/to_string
* [std::make_unique] https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
* [auto (C++)] https://learn.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-170
* [std::stoull] https://en.cppreference.com/w/cpp/string/basic_string/stoul
* [std::sort] https://en.cppreference.com/w/cpp/algorithm/sort.html




Student 1: Vienna Tan (925205757)
Student 2: Sophia Chan (925208411)
## Generative AI Use
Student 1 used ChatGPT for Prompt 1, 2, 3, 4, 5

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




### Prompt 4
what does #include <unordered_map> do and how to use it   
### Response 4
🔹 What does #include <unordered_map> do?

It gives you access to the C++ container:

std::unordered_map


An unordered_map is a hash table.

It stores:

key → value


Pairs.

Think of it like:

Python dictionary

Java HashMap

A lookup table

🔹 What’s special about unordered_map?

It allows very fast lookups:

O(1) average time


So instead of searching through a vector one-by-one:

O(n)


You can instantly find something by key.

🔹 Basic Example
#include <unordered_map>
#include <string>
#include <iostream>

int main(){
    std::unordered_map<std::string, int> ages;

    ages["Alice"] = 20;
    ages["Bob"] = 25;

    std::cout << ages["Alice"] << std::endl;  // prints 20
}


Here:

Key type = std::string

Value type = int

🔹 How It Works

Declaration:

std::unordered_map<KeyType, ValueType> name;


Example:

std::unordered_map<int, std::string> stops;

🔹 Common Operations
Insert
map[key] = value;


or

map.insert({key, value});

Access
map[key]


⚠️ Warning:
If the key does NOT exist, map[key] creates it automatically.

If you want safe lookup:

map.at(key)


This throws an exception if not found.

Check if key exists
if(map.find(key) != map.end()){
    // key exists
}

Remove
map.erase(key);

Loop through it
for(const auto &pair : map){
    std::cout << pair.first << " -> " << pair.second << std::endl;
}

🔹 unordered_map vs map
std::map	std::unordered_map
Ordered (sorted keys)	Not ordered
Slower (O(log n))	Faster (O(1) avg)
Uses tree	Uses hash table
### Changes 4
XMLBusSystem understanding on how unordered maps work to be able to use it

### Prompt 5
What does const mean after a function
### Response 5
🔹 What does const mean after a function?

When you write:

int GetValue() const;


The const at the end means:

🔒 This function is not allowed to modify the object.

It promises that the function will not change any member variables.

🔹 Example
class Person {
private:
    int age;

public:
    int GetAge() const {
        return age;
    }
};


Because GetAge() is marked const, it cannot change age.

If you try:

int GetAge() const {
    age = 10;   // ❌ ERROR
    return age;
}


The compiler will stop you.

🔹 Why This Matters

Suppose you have:

const Person p;


You can only call const functions on p.

p.GetAge();   // ✅ OK
p.SetAge(20); // ❌ ERROR if SetAge is not const


So marking functions const allows them to work on const objects.

🔹 What Exactly Is Protected?

Inside a const function:

❌ You cannot change member variables

❌ You cannot call non-const member functions

✅ You can read member variables

✅ You can call other const functions

🔹 How the Compiler Thinks About It

This:

int GetAge() const;


Is basically treated like:

int GetAge(const Person* this);


Meaning the this pointer is const.

So you can’t modify the object through it.

🔹 Very Common in Getter Functions

Most getters should be const:

int Size() const;
bool Empty() const;
std::string Name() const;


Because they don’t modify anything.
### Changes 5
Understanding how to use functions with const

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



### Prompt 3
can you break down what this osm xml means

"<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"


### Response 3
Let’s break this down step by step. You’re looking at **OSM XML**, which is a file format used by **OpenStreetMap** to describe map data like points (nodes), roads (ways), and areas.

Here’s the cleaned-up version of your XML (without the C-style string formatting):

```xml
<osm version="0.6" generator="osmconvert 0.8.5">
  <node id="1" lat="38.5" lon="-121.7"/>
  <node id="2" lat="38.5" lon="-121.8"/>
  <way id="1000">
    <nd ref="1"/>
    <nd ref="2"/>
  </way>
</osm>
```

---

## 1️⃣ `<osm>` — The Root Element

```xml
<osm version="0.6" generator="osmconvert 0.8.5">
```

This is the **container** for all map data inside the file.

* `version="0.6"` → OSM XML format version
* `generator="osmconvert 0.8.5"` → The tool that created the file (osmconvert)

Everything inside belongs to this map dataset.

---

## 2️⃣ `<node>` — Map Points

```xml
<node id="1" lat="38.5" lon="-121.7"/>
<node id="2" lat="38.5" lon="-121.8"/>
```

A **node** is a single geographic point defined by latitude and longitude.

Each node has:

* `id` → unique identifier
* `lat` → latitude
* `lon` → longitude

So here we have:

| Node ID | Latitude | Longitude |
| ------- | -------- | --------- |
| 1       | 38.5     | -121.7    |
| 2       | 38.5     | -121.8    |

These are two points that lie horizontally apart (same latitude, different longitude).

If you plotted them, they’d form a short horizontal line west-east.

---

## 3️⃣ `<way>` — A Line (or Road)

```xml
<way id="1000">
```

A **way** is an ordered list of nodes. It usually represents:

* A road
* A path
* A boundary
* Or the outline of an area

The `id="1000"` is just the unique identifier for this way.

---

## 4️⃣ `<nd ref="..."/>` — Node References

Inside the way:

```xml
<nd ref="1"/>
<nd ref="2"/>
```

These mean:

* Use node with ID 1
* Then use node with ID 2

So this way connects:

Node 1 → Node 2

That forms a straight line between those two coordinates.

---

## 🔎 What This Entire File Represents

This file defines:

* Two geographic points
* One line connecting them

Visually:

```
(38.5, -121.7)  --------  (38.5, -121.8)
     Node 1                    Node 2
```

So effectively: **a single straight line segment between two map points.**

---

## 🧠 Conceptual Summary

In OpenStreetMap:

* **Node = point**
* **Way = ordered list of points**
* Ways reference nodes using `<nd ref="ID"/>`
* The `<osm>` tag wraps everything

This is the simplest possible example of a road-like structure.

---

If you'd like, I can also explain:

* How areas (polygons) work
* What relations are
* How tags (like highway=primary) get added
* Or how you would parse this in C++

Just tell me what direction you want to go 🚀



### Changes 3
just needed clarification on file formatting





### Prompt 4
does unordered_map scramble things

### Response 4
Yes — **`std::unordered_map` absolutely “scrambles” things** (but in a very specific way).

It does **not** keep things in the order you inserted them.

Let’s break it down clearly.

---

## 🔹 What `unordered_map` Actually Does

`std::unordered_map` is implemented using a **hash table**.

That means:

1. Your key goes into a **hash function**
2. The hash determines which **bucket** it goes into
3. Internally, elements are arranged by hash, not insertion order

So when you iterate:

```cpp
std::unordered_map<std::string, int> m;
m["apple"] = 1;
m["banana"] = 2;
m["cherry"] = 3;

for (auto &pair : m) {
    std::cout << pair.first << "\n";
}
```

You might get:

```
banana
apple
cherry
```

Or something else entirely.

The order is:

* ❌ Not sorted
* ❌ Not insertion order
* ✅ Based on hash bucket placement

---

## 🔹 Does It Randomize Every Time?

Important detail:

It is **not randomly shuffled each time**.

The order is:

* Deterministic for a given run
* But depends on the hash function and bucket layout
* Can change if the table rehashes

If you insert more elements and it resizes, the internal layout may change → iteration order may change.

---

## 🔹 Why This Matters

If your code depends on order like:

```cpp
for (auto it = DAttributes.begin(); it != DAttributes.end(); ++it)
```

You CANNOT assume:

* First inserted comes first
* Alphabetical order
* Any predictable ordering

This is especially important if you're:

* Printing attributes
* Comparing output
* Writing deterministic tests

---

## 🔹 If You Need Order

### Keep insertion order?

Use:

```cpp
std::map
```

(ordered by key)

### Keep sorted by key?

Use:

```cpp
std::map
```

(sorted automatically)

### Keep insertion order specifically?

C++ standard library doesn’t provide this directly — you’d need:

* `std::vector + std::unordered_map`
* Or a custom structure

---

## 🔹 Quick Mental Model

Think of:

* `std::map` → organized bookshelf (sorted)
* `std::unordered_map` → tossed into labeled bins by hash
* `std::vector` → stack in order added

---

If you’re debugging something with `DAttributes` (which looks like an `unordered_map`), tell me what weird behavior you're seeing and we’ll dissect it like engineers 🧠💻


### Changes 4
i was having trouble with insertion order so this helped clear it up
i switched it to vector pair





### Prompt 5
how does .find() work


### Response 5

`.find()` is used to **search for something inside a container** and tell you **where it is** (or if it exists). The exact behavior depends on the language or container you're using. Since you’ve been working with **C++ `unordered_map` and `map`** recently, I’ll explain that version first.

---

# 1. `.find()` in C++ `map` / `unordered_map`

For containers like:

```cpp
std::unordered_map<KeyType, ValueType> myMap;
```

`.find(key)` **searches for the key**.

### Syntax

```cpp
auto it = myMap.find(key);
```

### What it returns

It returns an **iterator**.

* If the key **exists** → iterator points to the element
* If the key **does not exist** → iterator equals `myMap.end()`

---

### Example

```cpp
std::unordered_map<std::string, int> ages;

ages["Alice"] = 20;
ages["Bob"] = 25;

auto it = ages.find("Alice");
```

Now `it` points to:

```
("Alice", 20)
```

---

### Checking if something exists

```cpp
auto it = ages.find("Alice");

if (it != ages.end()) {
    std::cout << "Found it!\n";
}
```

Explanation:

* `.end()` means **past the last element**
* If `.find()` returns `.end()` → it **wasn't found**

---

# 2. Accessing the value

If you find something:

```cpp
auto it = ages.find("Alice");

if (it != ages.end()) {
    std::cout << it->second;
}
```

Why `second`?

Because a map stores **pairs**:

```
(key, value)
```

Internally:

```
pair<string, int>
```

So:

| Part         | Meaning |
| ------------ | ------- |
| `it->first`  | key     |
| `it->second` | value   |

Example:

```cpp
std::cout << it->first;  // Alice
std::cout << it->second; // 20
```

---

# 3. Why `.find()` is useful

Using `[]` can accidentally **create new entries**.

Example:

```cpp
ages["Charlie"];
```

This **adds Charlie with value 0**.

But `.find()` **does NOT create anything**.

So it's safer for searching.

---

# 4. Time Complexity (what O(1) means)

You also asked about `O(1)` earlier.

| Container       | `.find()` speed  |
| --------------- | ---------------- |
| `unordered_map` | **O(1)** average |
| `map`           | **O(log n)**     |

Meaning:

* **O(1)** → constant time (very fast)
* **O(log n)** → slightly slower but still efficient

`unordered_map` uses **hash tables**, which is why lookups are fast.

---

# 5. Visual example

If your map is:

```
{
  "A" → 10
  "B" → 20
  "C" → 30
}
```

Then:

```cpp
auto it = myMap.find("B");
```

`it` points to:

```
("B", 20)
```

So:

```cpp
it->first  = "B"
it->second = 20
```

---

# 6. In your StreetMapIndexer context

You might see something like:

```cpp
auto search = NodeIndex.find(nodeID);

if(search != NodeIndex.end()) {
    location = search->second;
}
```

Meaning:

1. Look for `nodeID`
2. If it exists
3. Get the stored value (`second`)

---

✅ **Simple way to remember**

```
.find(key)

returns → iterator
iterator->first  = key
iterator->second = value
```

---

If you want, I can also explain **how `.find()` actually works internally in an `unordered_map` (hash tables)** — that’s super useful for understanding your **StreetMapIndexer project**.


### Changes 5
used it to check if a given node id exists in the DNodeToWays map & retrieve its associated set of ways



### Prompt 6
what is big O and how do you use it in c++


### Response 6

## 1. What Big O Is

**Big O notation** (`O(...)`) describes **how the runtime of your code grows as the input grows**.

* It **ignores constants** and focuses on the trend for large inputs.
* It helps compare **algorithms**, e.g., searching, sorting, etc.

### Common Big O Examples

| Big O        | Name         | Example                       | Notes                                     |
| ------------ | ------------ | ----------------------------- | ----------------------------------------- |
| `O(1)`       | Constant     | Accessing `arr[5]`            | Takes the same time, no matter array size |
| `O(log n)`   | Logarithmic  | Binary search                 | Time grows slowly as input increases      |
| `O(n)`       | Linear       | Loop through array once       | Time grows directly with input size       |
| `O(n log n)` | Linearithmic | Merge sort, quicksort average | Typical efficient sorts                   |
| `O(n^2)`     | Quadratic    | Nested loops                  | Time grows fast as input increases        |
| `O(2^n)`     | Exponential  | Recursive Fibonacci           | Very slow for large `n`                   |

---

## 2. How to Think About Big O in C++

You **don’t “use” Big O directly** in code—it's **a way to analyze your code**.

Instead, you **look at your loops, recursion, or data structures**:

* **Single loop over `n` elements:** `O(n)`
* **Nested loops:** `O(n^2)`
* **Map lookups (`map` or `unordered_map`)**:

  * `unordered_map.find(key)` → `O(1)` average, `O(n)` worst case
  * `map.find(key)` → `O(log n)` (binary search tree)

---

### Examples

#### Example 1: Linear search

```cpp
std::vector<int> v = {1,2,3,4,5};
for(int i = 0; i < v.size(); i++) {
    if(v[i] == 3) { 
        std::cout << "Found!\n"; 
    }
}
```

* **Big O:** `O(n)`
* Reason: You may have to check every element.

---

#### Example 2: Map lookup

```cpp
std::unordered_map<int, int> m;
m[5] = 10;

auto it = m.find(5);
```

* **Big O:** `O(1)` average (hash table lookup)
* **It’s very fast**, even if the map has millions of entries.

---

#### Example 3: Nested loops

```cpp
for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        // do something
    }
}
```

* **Big O:** `O(n^2)`
* Reason: For each `i`, you run `n` iterations of `j`.

---

## 3. Why Big O Matters in C++

* Helps **choose the right data structure** (vector vs. map vs. unordered_map)
* Helps **predict performance** for large datasets
* Makes code **scalable**, especially for projects like StreetMapIndexer

Example:

```cpp
std::unordered_map<int, Node> NodeIndex;

// Lookup by nodeID
auto search = NodeIndex.find(nodeID);
```

* `.find()` → `O(1)` → very fast
* If you used `std::vector` and looped → `O(n)` → slower for big maps

---

## 4. Tips to Estimate Big O

1. **Count loops**: Each loop over `n` → `O(n)`, nested → multiply
2. **Look at recursion**: Recursion depth matters
3. **Check data structure operations**:

   * `vector push_back` → O(1) amortized
   * `map insert/find` → O(log n)
   * `unordered_map insert/find` → O(1) average


### Changes 6
just needed clarification & helped me choose correct data structure