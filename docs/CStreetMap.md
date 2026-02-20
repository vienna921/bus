# CStreetMap
## Overview
- CStreetMap is an abstract interface for street map data
- provides access to nodes (SNode) and ways (SWay) without specifying data source
- concrete implementations (like COpenStreetMap) must implement its methods

## Node (SNode) Interface
### virtual TNodeID ID() const noexcept = 0;
- returns the node's unique identifier

### virtual SLocation Location() const noexcept = 0;
- return the latitude and longitude of the node as Slocation

### virtual std::size_t AttributeCount() const noexcept = 0;
- returns the number of attributes stored in the node

### virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
- parameter:
    - std::size_t index -> index of attribute
- returns the attribute key at the specified index or empty string if invalid

### virtual bool HasAttribute(const std::string &key) const noexcept = 0;
- parameter:
    - std::string key -> attribute key
- returns true if the node has an attribute with the given key

### virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
- parameter:
    - std::string key -> attribute key
- returns the value of the attribute or empty string if not foudn


## Way(SWay) Interface
### virtual TWayID ID() const noexcept = 0;
- returns the unique identifier of the way

### virtual std::size_t NodeCount() const noexcept = 0;
- returns the number of nodes contained in the way

### virtual std::size_t AttributeCount() const noexcept = 0;
- returns the number of attributes stored in the way

### virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
- parameter:
    - std::size_t index -> index of attribute
- returns the attribute key at that index or empty string if invalid

### virtual bool HasAttribute(const std::string &key) const noexcept = 0;
- parameter:
    - std::string key -> attribute key
- returns true if the way contains the specified attribute

### virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
- parameter:
    - std::string key -> attribute key
- returns the attribute value or empty string if not found


## CStreetMap Main Methods
### virtual std::size_t NodeCount() const noexcept = 0;
- returns the total number of nodes in the map

### virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;
- parameter:
    - std::size_t index -> node index
- returns a pointer to the node at the given index or nullptr if invalid

### virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;
- parameter:
    - TNodeID id -> node ID
- returns pointer to node with given ID or nullptr if not found

### virtual std::size_t WayCount() const noexcept = 0;
- returns the total number of ways in the map

### virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept =0;
- parameter:
    - std::size_t index -> way index
- returns pointer to way at index or nullptr if invalid

### virtual std::shared_ptr<SWay> WayByID(TWayiD id) const noexcept = 0;
- parameter:
    - TWayID id -> way ID
- returns pointer to way and given ID or nullptr if not found

## side notes
- CStreetMap is abstract so it can't be instantiated directly
- all nodes and ways are represented by their respective interfaces (SNode & SWay)
- implementation slike COpenStreetMap or other map loaders need to provide concrete implementations of all methods

## Exmaple usage (assuming a CStreetMap implementation)
std::shared_ptr<CStreetMap> map = std::make_shared<COpenStreetMap>(OSMReader);
for (std::size_t i = 0; i < map->NodeCount(); i++){
    auto node = map->NodeByIndex(i);
    std::cout<< node->ID()<<": ("<<node->Location().DLatitude<<","<<node->Location().DLongitude<<")"<<std::endl;
}
auto way = map->WayByID(12345);
if(way){
    std::cout<<"way has "<<way->NodeCount()<<" nodes"<<std::endl;
}

