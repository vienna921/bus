# CTripPlanner.md
## Overview
- CTripPlanner computes the travel plans between bus stops
- It determines direct routes between two stops, routes with one transfer, and optimal trips based on leave and arrival time
## Constructor
### CTripPlanner(std::shared_ptr<CBusSystem> bussystem)
- Parameter
    - std::shared_ptr<CBusSystem> bussystem -> a pointer to the bus system that contains stops, routes, and schedules
- creates trip planner and builds index of bus system for fast route search
## Data Types
### STravelStep
- represents a single step in a travel plan
#### Members
##### TStopTime DTime
- time associated with the step

##### TStopID DStopID
- stop ID where the step occurs

##### std::string DRouteName
- Name of bus route used for the step or "" for getting off bus

### TTravelPlan
- ordered list of travel steps where each step has the stop, the time, and the route being used

## Main Methods
### std::shared_ptr<CBusSystemIndexer> BusSystemIndexer() const
- returns pointer to CBusSystemIndexer
- allows acces to indexed bus system used for route

### std::shared_ptr<SRoute> FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const
- parameters
    - TStopID src -> starting stop ID
    - TStopID dest -> destination stop ID
    - TStopTime leaveat -> earliest time passenger wants to leave
- returns pointer to route that can take passenger directly from src to dest or nullptr if no direct route exists

### std::shared_ptr<SRoute> FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const
- parameters
    - TStopID src -> starting stop ID
    - TStopID dest -> destination stop ID
    - TStopTime arriveby -> latest acceptable arrival time
- returns pointer to a direct route or nullptr if no route can arrive by the specified time

### bool FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const
- parameters
    - TStopID src -> starting stop ID
    - TStopID dest -> Destination stop ID
    - TStopTime leaveat -> Earliest time the passenger wants to leave
    - TTravelPlan &plan -> output parameter where travel plan is stored
- returns true if valid route is found else false

### bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const
- parameters
    - TStopID src -> starting stop ID
    - TStopID dest -> destination stop ID
    - TStopTime arriveby -> latest acceptable arrival time
    - TTravelPlan &plan -> output parameter that stores the resulting plan
- returns true if valid route is found else false
## Example
### Find a route leaving at a specific time
auto Planner = std::make_shared<CTripPlanner>(BusSystem);
CTripPlanner::TTravelPlan Plan;
if(Planner->FindRouteLeaveTime(28, 82, TStopTime(8:00AM"), Plan)){
    for(auto &step:Plan){
        std::cout << step.DStopID << " " << step.DRouteName << std::endl;
    }
}