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

Student 2: Sophia Chan
## Generative AI Use
I used ChatGPT for Prompt 1

### Prompt 1

### Response 1

### Changes 1

