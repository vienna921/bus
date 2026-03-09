
#include "StreetMapIndexer.h"
#include "StreetMap.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>

//node x -> lookup in hash map -> return ways instantly

struct CStreetMapIndexer::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap; // original map
    std::vector<std::shared_ptr<CStreetMap::SNode>> DSortedNodes;
    std::vector<std::shared_ptr<CStreetMap::SWay>> DSortedWays;
    std::unordered_map<CStreetMap::TNodeID, std::unordered_set<std::shared_ptr<CStreetMap::SWay>> > DNodeToWays;
    // node 100 -> {way 4, way 9}


    SImplementation(std::shared_ptr<CStreetMap> streetmap){
        DStreetMap = streetmap;
        // copy nodes (from street map & store locally)
        for (size_t i = 0; i < streetmap->NodeCount(); i++){
            DSortedNodes.push_back(streetmap->NodeByIndex(i));
        }
        // copy ways
        for (size_t i = 0; i < streetmap->WayCount(); i++){
            auto way = DStreetMap->WayByIndex(i);
            DSortedWays.push_back(way);
            // build node -> ways index
            for (size_t j = 0; j < way->NodeCount(); j++){
                auto nodeID = way->GetNodeID(j);
                DNodeToWays[nodeID].insert(way);
            }
        }

        // sort nodes by id
        std::sort(DSortedNodes.begin(), DSortedNodes.end(), [](auto a, auto b){return a->ID() < b->ID();});
        // sort ways by id
        std::sort(DSortedWays.begin(), DSortedWays.end(), [](auto a, auto b){return a->ID() < b->ID();});
    }

};


CStreetMapIndexer::CStreetMapIndexer(std::shared_ptr<CStreetMap> streetmap){
    DImplementation = std::make_unique<SImplementation>(streetmap);
}

CStreetMapIndexer::~CStreetMapIndexer(){}

std::size_t CStreetMapIndexer::NodeCount() const noexcept{
    return DImplementation->DSortedNodes.size();
}

std::size_t CStreetMapIndexer::WayCount() const noexcept{
    return DImplementation->DSortedWays.size();
}

std::shared_ptr<CStreetMap::SNode> CStreetMapIndexer::SortedNodeByIndex(std::size_t index) const noexcept{
    if(index >= DImplementation->DSortedNodes.size()){
        return nullptr;
    }
    return DImplementation->DSortedNodes[index];
}



std::shared_ptr<CStreetMap::SWay> CStreetMapIndexer::SortedWayByIndex(std::size_t index) const noexcept{
    if (index >= DImplementation->DSortedWays.size()){
        return nullptr;
    }
    return DImplementation->DSortedWays[index];
}

std::unordered_set<std::shared_ptr<CStreetMap::SWay>>CStreetMapIndexer::WaysByNodeID(CStreetMap::TNodeID node) const noexcept{
    auto it = DImplementation->DNodeToWays.find(node);
    if(it == DImplementation->DNodeToWays.end()){
        return {};
    }
    return it->second;
}

// finds all the ways containing at least one node in box
std::unordered_set<std::shared_ptr<CStreetMap::SWay>> CStreetMapIndexer::WaysInRange(const CStreetMap::SLocation &bottomleft, const CStreetMap::SLocation &topright) const noexcept{
    std::unordered_set<std::shared_ptr<CStreetMap::SWay>> Result;
    for(auto Way: DImplementation->DSortedWays){
        for(size_t i = 0; i< Way->NodeCount(); i++){
            auto nodeID = Way->GetNodeID(i);
            auto node = DImplementation->DStreetMap->NodeByID(nodeID);
            if(!node){
                continue;
            }
            auto location = node->Location();
            if (location.DLatitude >= bottomleft.DLatitude && location.DLatitude <= topright.DLatitude && location.DLongitude >= bottomleft.DLongitude && location.DLongitude <= topright.DLongitude){
                Result.insert(Way);
                break; // one node inside is enough
            }
        }
    }
    return Result;
}