# CXMLReader
CXMLReader is a class that reads XML data from data source

# Constructor and Destructor
CXMLReader(std::shared_ptr<CDataSource> source)
- constructs CXMLReader instance with given data source
- parameters:
    - source is a shared pointer to a CDataSource object from which XML data is read

~CXMLReader()
- destructor that cleans up internal resources

# Public Methods
bool End() const noexcept
- checks if reader reached the end of data source
- returns true if no more data to read, else false

bool Get(char &ch) noexcept
- reads the next character from data source
- parameters
    - ch is the reference to a character variable where read character will be stored
- returns true if character successfully read, else false if data is reached at the end

bool Peek(char &ch) noexcept
- peeks at the next character without going to the next read position
- parameters
    - ch is the reference to a character variable where read character will be stored
- returns ture if a character is available, else false if data is reached at the end

bool Read(std::vector<char> &buf, std::size_t count) noexcept
- reads up to count characters from the data source into a buffer
- parameters
    - buf is a vector that stores the read characters
    - count is the maximum number of characters to read
- returns ture if at least one character was read, else false
