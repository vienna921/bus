# CStreetMapIndexer

## Overview
- CStreetMapIndexer provides fast access to CStreetMap data
- it indexes nodes and ways, allowing lookup by index, ID, or spatial location
- internally, it maintains sorted vectors of nodes and ways and a hash map of node ID → ways

## Constructor and Destructor

### CStreetMapIndexer::CStreetMapIndexer(std::shared_ptr<CStreetMap> streetmap)
- parameters:
  - std::shared_ptr<CStreetMap> streetmap → pointer to the original street map to index
- copies nodes and ways from the street map, builds a node → ways index, and sorts nodes and ways by ID

### CStreetMapIndexer::~CStreetMapIndexer()
- destructor. Cleans up internal resources.

## Public Methods

### std::size_t NodeCount() const noexcept
- returns the total number of nodes in the indexed street map.

### std::size_t WayCount() const noexcept
- returns the total number of ways in the indexed street map.

### std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept
- parameter:
  - std::size_t index → index of node in sorted list.
- returns pointer to node at the given index or nullptr if index is out of range

### std::shared_ptr<CStreetMap::SWay> SortedWayByIndex(std::size_t index) const noexcept
- parameter:*
  - std::size_t index → index of way in sorted list.
- returns pointer to way at the given index or nullptr if index is out of range

### std::unordered_set<std::shared_ptr<CStreetMap::SWay>> WaysByNodeID(CStreetMap::TNodeID node) const noexcept
- parameter:
  - CStreetMap::TNodeID node → node ID to lookup.
- returns a set of ways that contain the given node. Returns empty set if node not found.

### std::unordered_set<std::shared_ptr<CStreetMap::SWay>> WaysInRange(const CStreetMap::SLocation &bottomleft, const CStreetMap::SLocation &topright) const noexcept
- parameters:
  - CStreetMap::SLocation &bottomleft → bottom-left corner of bounding box
  - CStreetMap::SLocation &topright → top-right corner of bounding box
- returns a set of ways containing at least one node inside the specified bounding box

## Private Members
- std::shared_ptr<CStreetMap> DStreetMap → original street map.
- std::vector<std::shared_ptr<CStreetMap::SNode>> DSortedNodes → nodes sorted by ID.
- std::vector<std::shared_ptr<CStreetMap::SWay>> DSortedWays → ways sorted by ID.
- std::unordered_map<CStreetMap::TNodeID, std::unordered_set<std::shared_ptr<CStreetMap::SWay>>> DNodeToWays → mapping of node IDs to ways containing them.

## Examples

### Construction
```cpp
auto StreetMap = std::make_shared<CStreetMap>();
// populate StreetMap with nodes and ways

CStreetMapIndexer Indexer(StreetMap);


### Access Nodes
for (std::size_t i = 0; i < Indexer.NodeCount(); i++) {
    auto Node = Indexer.SortedNodeByIndex(i);
    std::cout << "Node ID: " << Node->ID() << std::endl;
}

### Access Ways
for (std::size_t i = 0; i < Indexer.WayCount(); i++) {
    auto Way = Indexer.SortedWayByIndex(i);
    std::cout << "Way ID: " << Way->ID() << std::endl;
}

### Lookup Ways By NodeID
auto NodeWays = Indexer.WaysByNodeID(42);
for (auto Way : NodeWays) {
    std::cout << "Way containing node 42: " << Way->ID() << std::endl;
}

### Find Ways in a Bounding Box

CStreetMap::SLocation BL{38.5, -121.8};
CStreetMap::SLocation TR{38.6, -121.7};
auto WaysInBox = Indexer.WaysInRange(BL, TR);

for (auto Way : WaysInBox) {
    std::cout << "Way in range: " << Way->ID() << std::endl;
}

