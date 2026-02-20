# COpenStreetMap
## Overview
- COpenStreetMap class implements the CStreetMap interface
- it parses OpenStreetMap XML (<osm>) containing nodes (<node>) and ways (<way>)

## Constructor and Destructor
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src)
- parameter: 
    - std::shared_ptr<CXMLReader> src -> XML source containing OSM data
- parses <osm> XML, reading <node> and <way> elements
- stores nodes and ways in internal containers by index and ID

COpenStreetMap::~COpenStreetMap()
- destructor - cleans up internal resources

## Public Methods
### std::size_t NodeCount() const;
- returns the total number of nodes in the map

### std::shared_ptr<SNode> NodeByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index of node
- returns pointer to node at specific index or nullptr if invalid

### std::shared_ptr<SNode> NodeByID(TNodeID id) const;
- parameter:
    - TNodeID id -> node ID
- returns pointer to node with given ID or nullptr if not found

### std::size_t WayCount() const;
- returns the number of ways in the map

### std::shared_ptr<SWay> WayByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index of way
- returns pointer to way at specified index or nullptr if invalid

### std::shared_ptr<SWay> WayByID(TWayID id) const;
- parameter:
    - TWayID id -> way ID
- returns pointer to way with given ID or nullptr if not found

## Node (SNode) Methods
### TNode ID ID() const noexcept;
- returns the node's unique ID

### SLocation Location() const noexcept;
- returns the node's latitude and longitude as SLocation

### std::size_t AttributeCount() const noexcept;
- returns the number of attributes stored for the node

### std::string GetAttributeKey(std::size_t index) const noexcept;
- parameter:
    - std::size_t index -> attribute index
- returns the key at the specified index or empty string if invalid

### bool HasAttribute(const std::string &key) const noexcept;
- parameter:
    - std::string key -> attribute key
- returns true if node has an attribute with the given key

### std::string GetAttribute(const std::string &key) const noexcept;
- parameter:
    - std::string key -> attribute key
- returns the value of the attribute or empty string if not found

## Way (SWay) Methods
### TWayID ID() const noexcept;
- returns the way's unique ID

### std::size_t NodeCount() const noexcept;
- returns the number of nodes in the way

### TNodeID GetNodeID(std::size_t index) const noexcept;
- parameter:
    - std::size_t index -> node index within way
- returns node ID at index or 0 if invalid

### std::size_t AttributeCount() const  noexcept;
- returns the number of attributes stored for the way

### std::string GetAttributeKey(std:;size_t index) const noexcept;
- parameter:
    - std::size_t index -> attribute index
- returns attribute key or empty string if invalid

### bool HasAttribute(const std::string &key) const noexcept;
- parameter:
    - std::string key -> attribute key
- returns true if way has an attribute with the given key

std::string GetAttribute(const std::string &key) const noexcept;
- parameter:
    - std::string key -> attribute key
- returns attribute value or empty string if not found

## Private Methods (Implenentation)
### bool FindStartTag(std::shared_ptr<CXMLReader> xmlsource, const std::string &starttag);
- finds the start tag <starttag> in the XML stream

### bool FindEndTag(std::shared_ptr<CXMLReader> xmlsource, const std::string &starttag);
- finds the matching end tag </starttag> in the XML stream

### bool ParseOSM(std::shared_ptr<CXMLReader> src);
- parses the <osm> XML, reading nodes & ways
- stores nodes in DNodesByIndex/DNodesByID
- stores ways in DWaysByIndex/DWaysByID


## Example Usage
auto OSMReader = std::make_shared<CXMLReader>(source);
COpenStreetMap OSM(OSMReader);

auto node = OSM.NodeByID(123456);
if(node && node->HasAttribute("banana)){
    std::cout<<"banana type: "<<node->GetAttribute("banana)<<std::endl;

}
auto way = OSM.WayByIndex(0);
std::cout<<"way has "<<way->NodeCount()<<" nodes"<<std::endl;
