# CTextTripPlanWriter
## Overview
- CTextTripPlanWriter writes the travel plans that includes the steps of a trip plan to a CDataSink

## Nested Config
### SimpleConfig
- configuration class derived from CTripPlanWriter::SConfig
- manages flags and options
#### Methods
##### bool FlagEnabled(std::string_view flag) const
- parameter
    - std::string_view flag -> flag being checked
- returns if specified flag is enabled

##### void EnableFlag(std::string_view flag)
- parameter
    - std::string_view flag -> flag to enable
- enables specified flag

##### void DisableFlag(std::string_view flag)
- parameter
    - std::string_view flag -> flag to disable
- disables specified flag

##### std::any GetOption(std::string_view) const
- returns value of an option

##### std::unordered_set<std::string> ValidFlags() const
- returns set of valid flags

##### EOptionType GetOptionType(std::string_view) const
- parameter
    - std::string_view -> option name
- returns type of option

##### void SetOption(std::string_view, int)
##### void SetOption(std::string_view, double)
##### void SetOption(std::string_view, const std::string &)
- sets options of different types

##### void ClearOption(std::string_view)
- clears specified option

##### std::unordered_set<std::string> ValidOptions() const
- returns valid options

## Implementation Structure

### SImplementation
- implementation of CTextTripPlanWriter that stores references to CBusSystem and SConfig
#### Members
##### std::shared_ptr<CBusSystem> DBusSystem
- pointer to bus system used for stop and route lookup
##### std::shared_ptr<SConfig> DConfig
- configuration object controlling writer behavior

#### Methods

##### std::shared_ptr<SConfig> Config() const
- returns configuration object

##### bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan)
- parameters
    - std::shared_ptr<CDataSink> sink -> output destination for the text instructions
    - const TTravelPlan &plan -> sequence of travel steps
- returns true if writing formatted travel instructions to sink is successful and false if invalid

#### Main Methods

##### std::shared_ptr<SConfig> Config() const
- returns configuration object for writer
- allows enabling flags

##### bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan)
- parameters
    - std::shared_ptr<CDataSink> sink -> destination to write text output
    - const TTravelPlan &plan -> list of travel steps
- returns true if plan of instructions describing trip: includes actions like boarding, staying, transferring, and getting off a bus, was written successfully

## Example
### Writing a Travel Plan
auto Writer = std::make_shared<CTextTripPlanWriter>(BusSystem);
auto Config = Writer->Config();
Config->EnableFlag(CTextTripPlanWriter::Verbose);
Writer->WritePlan(Sink, TravelPlan);
