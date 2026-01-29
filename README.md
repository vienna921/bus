# ecs36bproject
ECS 36B WQ 25 Project 2-4

## Project Status

## Known Issues

## Code References

Student 1: Vienna Tan (925205757)
## Generative AI Use
I used ChatGPT for Prompt 1

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

Student 2: Sophia Chan (925208411)
## Generative AI Use
I used ChatGPT for Prompt 1

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
