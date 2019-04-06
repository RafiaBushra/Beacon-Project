// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <algorithm>

#include <unordered_set>

typedef std::map<BeaconID, info> map_type;

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;
    
    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);
    
    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures(){}

Datastructures::~Datastructures(){}

int Datastructures::beacon_count()
{
    if(map_.empty()) return NO_VALUE;
    return map_.size();
}

void Datastructures::clear_beacons()
{      
    map_.clear();
}

std::vector<BeaconID> Datastructures::all_beacons()
{      
    std::vector<BeaconID> temp_vector;    
    for(map_type::iterator iter = map_.begin(); iter != map_.end(); iter++){
        temp_vector.push_back(iter->first);
    }
    return temp_vector;
}

bool Datastructures::add_beacon(BeaconID id, 
                                const std::string& name, 
                                Coord xy, 
                                Color color)
{      
    if(map_.find(id) != map_.end()) return false;
    info temp_info;
    temp_info.name_ = name;
    temp_info.position_ = xy;
    temp_info.colour_ = color;
    map_[id] = temp_info;
    return true;
}

std::string Datastructures::get_name(BeaconID id)
{      
    if(map_.find(id) != map_.end()) return map_.at(id).name_;
    return NO_NAME;
}

Coord Datastructures::get_coordinates(BeaconID id)
{      
    if(map_.find(id) != map_.end()) return map_.at(id).position_;
    return NO_COORD;
}

Color Datastructures::get_color(BeaconID id)
{      
    if(map_.find(id) != map_.end()) return map_.at(id).colour_;
    return NO_COLOR;
}

// Fills alphabetical_map_. alphabetical_map_ contains the names of the beacons
// as key and their corresponding id as value. Since std::multimap is sorted by keys, 
// alphabetical_map_ is automatically sorted as it is being filled. After that, the values
// (beacon id's) are inserted into a vector and returned.
std::vector<BeaconID> Datastructures::beacons_alphabetically() 
{
    if(map_.empty()) return {};
    alphabetical_map_.clear();
    for(map_type::iterator iter = map_.begin(); iter != map_.end(); iter++){
        alphabetical_map_.insert({iter->second.name_, iter->first});
    }    
    std::vector<BeaconID> temp_vector;
    for(auto iter = alphabetical_map_.begin(); iter != alphabetical_map_.end(); iter++){
        temp_vector.push_back(iter->second);
    }
    return temp_vector;    
}

// A helper function to calculate the brightness of a beacon.
int calculate_brightness(Color colour){
    return colour.r * 3 + colour.g * 6 + colour.b;
}

// Similar to beacons_alphabetically() but fills another multimap (brightness_map_)
// by calculating the brightness of each individual beacon with the help of
// calculate_brightness() function above.
std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
    if(map_.empty()) return {};
    brightness_map_.clear();
    for(map_type::iterator iter = map_.begin(); iter != map_.end(); iter++){
        brightness_map_.insert({calculate_brightness(iter->second.colour_), iter->first});
    }    
    std::vector<BeaconID> temp_vector;    
    for(auto iter = brightness_map_.begin(); iter != brightness_map_.end(); iter++){
        temp_vector.push_back(iter->second);
    }
    return temp_vector;    
}

// Uses std::min_element() along with a lambda function to compare the brightness
// of two beacons and returns the least bright beacon's ID.


// Similar to min_brightness but returns the most bright beacon's ID.
BeaconID Datastructures::max_brightness()
{
    
    if(!map_.empty())
    {
        map_type::value_type temp_beacon = *std::max_element(map_.begin(), 
                                                             map_.end(), 
                                                             [](const map_type::value_type pair1, 
                                                                 const map_type::value_type pair2)
        { return calculate_brightness(pair1.second.colour_) < 
                     calculate_brightness(pair2.second.colour_);
        });
        return temp_beacon.first;
    }
    return NO_ID;
}

// Manually iterates through all the elements in the map to find the beacon(s) with
// the given name and fills temp_vector with their corresponding id's and returns the
// sorted vector.
std::vector<BeaconID> Datastructures::find_beacons(std::string const& name)
{    
    std::vector<BeaconID> temp_vector;
    for ( map_type::iterator iter = map_.begin(); iter != map_.end(); iter++){
        if( iter->second.name_ == name){
            temp_vector.push_back( iter->first);
        }
    }
    std::sort(temp_vector.begin(), temp_vector.end());
    return temp_vector;
}

bool Datastructures::change_beacon_name(BeaconID id, const std::string& newname)
{    
    map_type::iterator temp_iter = map_.find(id);
    if(temp_iter == map_.end())  return false;
    temp_iter->second.name_ = newname;
    return true;
}

bool Datastructures::change_beacon_color(BeaconID id, Color newcolor)
{    
    map_type::iterator temp_iter = map_.find(id);
    if(temp_iter == map_.end())  return false;
    temp_iter->second.colour_ = newcolor;
    return true;
}

// Checks that the given id does not already send a lightbeam to another beacon
// and then inserts given id's in different containers which are used in various parts
// of the program.
bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{    
    if(map_.find(sourceid) == map_.end() 
            || map_.find(targetid) == map_.end() 
            || map_.at(sourceid).beamTarget_ != NO_ID) return false;
    map_.at(sourceid).beamTarget_ = targetid;
    map_.at(targetid).beamSources_.push_back(sourceid);
    return true;
}

// beamSources_ contains all the beacons that directly send lightbeams to the given
// id. Sorts the beamSources_ vector and returns it.
std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{    
    if(map_.find(id) == map_.end()) return {{NO_ID}};
    std::sort(map_.at(id).beamSources_.begin(), map_.at(id).beamSources_.end());
    return map_.at(id).beamSources_;
}

// Simply iterates through all the direct and indirect "childs" (beam sources) of the 
// the given beacon and fills the temp_vector.
std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{    
    if(map_.find(id) == map_.end()) return {{NO_ID}};
    std::vector<BeaconID> temp_vector;
    BeaconID temp_id = id;
    temp_vector.push_back(temp_id);
    while(temp_id != NO_ID){
        temp_id = map_.at(temp_id).beamTarget_;
        if(temp_id == NO_ID) break;
        temp_vector.push_back(temp_id);
    }
    return temp_vector;
}

bool Datastructures::remove_beacon(BeaconID id)
{    
    if(map_.find(id) == map_.end()) return false;
    BeaconID temp_id = map_.at(id).beamTarget_;
    if( temp_id != NO_ID){
        auto iter = std::find(map_.at(temp_id).beamSources_.begin(),
                              map_.at(temp_id).beamSources_.end(), id);
        map_.at(temp_id).beamSources_.erase(iter);
    }
    if(!map_.at(id).beamSources_.empty()){
        for(BeaconID temp_id : map_.at(id).beamSources_){
            map_.at(temp_id).beamTarget_ = NO_ID;
        }
    }
    map_.erase(id);
    return true;
}

// A helper function that inserts all the the beacons that let out lightbeams but don't 
// recieve any into children unordered_set.
void longest_path_helper(const map_type& map_, BeaconID most_original_beam, std::unordered_set<BeaconID>& children){
    
    if(map_.at(most_original_beam).beamSources_.empty()) {
        children.insert(most_original_beam);
        return;
    }
    for(BeaconID source_beacon: map_.at(most_original_beam).beamSources_){
        if(!map_.at(source_beacon).beamSources_.empty())
            longest_path_helper(map_, source_beacon, children);
        else children.insert(source_beacon);
    }
}

// Similar to path_outbeam but only creates the vector until the given id.
std::vector<BeaconID> custom_path_outbeam(const map_type& map_, BeaconID first, BeaconID last){
    std::vector<BeaconID> temp_vector;
    BeaconID temp_id = first;
    temp_vector.push_back(temp_id);
    while(temp_id != last){
        temp_id = map_.at(temp_id).beamTarget_;
        temp_vector.push_back(temp_id);
    }
    return temp_vector;
}

// Uses post-order tree traversal method to find all the childrens and then calculates
// the length of each path and returns the longest one.
std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID id)
{    
    if(map_.find(id) == map_.end()) return {{NO_ID}};
    if(map_.at(id).beamSources_.empty()) return {id};
    std::unordered_set<BeaconID> children;
    longest_path_helper(map_, id, children);
    std::map<std::vector<BeaconID>::size_type, BeaconID> path_sizes;
    for(BeaconID child:children){
        std::vector<BeaconID>::size_type temp_size = custom_path_outbeam(map_, child, id).size();
        path_sizes[temp_size] = child;
    }
    std::vector<BeaconID> final_vector = custom_path_outbeam(map_, path_sizes.rbegin()->second, id);
    return final_vector; 
}

Color calculate_total_color(Color colour1, Color colour2){
    return {colour1.r + colour2.r, colour1.g + colour2.g, colour1.b + colour2.b};
}

// Similar to path_inbeam_longest() but simply adds the colours.
void total_color_helper(const map_type& map_, BeaconID most_original_beam, std::pair<Color, int>& total_colour){
    
    if(map_.at(most_original_beam).beamSources_.empty()) {
        total_colour.first = calculate_total_color(map_.at(most_original_beam).colour_, total_colour.first);        
        total_colour.second ++;
        return;
    }
    for(BeaconID child:map_.at(most_original_beam).beamSources_){
        if(!map_.at(child).beamSources_.empty()) total_color_helper(map_, child, total_colour);
        total_colour.first = calculate_total_color(map_.at(child).colour_, total_colour.first);
        total_colour.second ++;
    }
} 

Color Datastructures::total_color(BeaconID id)
{    
    if(map_.find(id) == map_.end()) return NO_COLOR;
    if(map_.at(id).beamSources_.empty()) return map_.at(id).colour_;
    std::pair<Color, int> temp_sum = {{0, 0, 0}, 0};
    total_color_helper(map_, id, temp_sum);
    temp_sum.first.r /= temp_sum.second;
    temp_sum.first.g /= temp_sum.second;
    temp_sum.first.b /= temp_sum.second;
    return temp_sum.first;
}
