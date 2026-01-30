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