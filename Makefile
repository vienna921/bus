# Define the tools
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR				= ./include
SRC_DIR				= ./src
TESTSRC_DIR			= ./testsrc
BIN_DIR				= ./bin
OBJ_DIR				= ./obj
LIB_DIR				= ./lib
TESTOBJ_DIR			= ./testobj
TESTBIN_DIR			= ./testbin
TESTCOVER_DIR		= ./htmlconv

# Define the flags for compilation/linking
DEFINES				=
INCLUDE				= -I $(INC_DIR)
ARFLAGS				= rcs
CLFAGS				= -Wall
CPPFLAGS			= --std=c++17
LDFLAGS				=

TEST_CFLAGS			= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS		= $(CPPFLAGS) -fno-inline
TEST_LDFLAGS		= $(LDFLAGS) -lgtest -lgtest_main -lpthread

# Define the object files
TEST_SVG_OBJ		= $(TESTOBJ_DIR)/svg.o
TEST_SVG_TEST_OBJ	= $(TESTOBJ_DIR)/SVGTest.o
TEST_OBJ_FILES		= $(TEST_SVG_OBJ) $(TEST_SVG_TEST_OBJ)

TEST_STRSINK_OBJ		= $(TESTOBJ_DIR)/StringDataSink.o
TEST_STRSINK_TEST_OBJ	= $(TESTOBJ_DIR)/StringDataSinkTest.o
TEST_STRSINK_OBJ_FILES	= $(TEST_STRSINK_OBJ) $(TEST_STRSINK_TEST_OBJ)

TEST_STRSRC_OBJ			= $(TESTOBJ_DIR)/StringDataSource.o
TEST_STRSRC_TEST_OBJ 	= $(TESTOBJ_DIR)/StringDataSourceTest.o


# Define the targets
TEST_SVG_TARGET			= $(TESTBIN_DIR)/testsvg
TEST_STRSINK_TARGET 	= $(TESTBIN_DIR)/testteststrdatasink


all: directories run_svgtest run_sinktest gen_html

run_svgtest: $(TEST_SVG_TARGET)
	$(TEST_SVG_TARGET)

run_sinktest: $(TEST_STRSINK_TARGET)
	$(TEST_STRSINK_TARGET)

gen_html:
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)

$(TEST_SVG_TARGET): $(TEST_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_SVG_TARGET)

$(TEST_STRSINK_TARGET): $(TEST_STRSINK_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSINK_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSINK_TARGET)

$(TEST_SVG_OBJ): $(SRC_DIR)/svg.c
	$(CC) $(TEST_CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(TEST_SVG_OBJ)

$(TEST_SVG_TEST_OBJ): $(TESTSRC_DIR)/SVGTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/SVGTest.cpp -o $(TEST_SVG_TEST_OBJ)

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTCOVER_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTCOVER_DIR)

