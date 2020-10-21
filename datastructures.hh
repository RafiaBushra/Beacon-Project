// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

// Type for beacon IDs.
using BeaconID = std::string;

// Return value for cases where required beacon was not found.
BeaconID const NO_ID = "----------";

// Return value for cases where integer values were not found.
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found.
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y).
struct Coord
{
  int x = NO_VALUE;
  int y = NO_VALUE;
};

// Defining == operator for Coord so that it can be used as key for std::unordered_map/set, if needed.
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); }

// Defining hash function for Coord so that it can be used as key for std::unordered_map/set, if needed.
struct CoordHash
{
  std::size_t operator()(Coord xy) const
  {
    auto hasher = std::hash<int>();
    auto xhash = hasher(xy.x);
    auto yhash = hasher(xy.y);
    // Combine hash values (magic!)
    return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
  }
};

// Defining < for Coord so that it can be used as key for std::map/set.
inline bool operator<(Coord c1, Coord c2)
{
  if (c1.y < c2.y)
  {
    return true;
  }
  else if (c2.y < c1.y)
  {
    return false;
  }
  else
  {
    return c1.x < c2.x;
  }
}

// Return value for cases where coordinates were not found.
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for color (RGB).
struct Color
{
  int r = NO_VALUE;
  int g = NO_VALUE;
  int b = NO_VALUE;
};

// Defining == and != operator for Colors.
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1 == c2); }

// Return value for cases where color was not found.
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost.
using Cost = int;

// Return value for cases where cost is unknown.
Cost const NO_COST = NO_VALUE;

// Type for beacon information.
struct info
{
  info()
  {
    name_ = "";
    position_ = NO_COORD;
    colour_ = NO_COLOR;
    beamTarget_ = NO_ID;
  }
  std::string name_;
  Coord position_;
  Color colour_;
  BeaconID beamTarget_;
  std::vector<BeaconID> beamSources_;
};

class Datastructures
{
public:
  Datastructures();
  ~Datastructures();

  // Estimate of performance: O(1)
  // Short rationale for estimate: Uses std::map<> member function size().
  int beacon_count();

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::map<> member function clear().
  void clear_beacons();

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses a single for loop to iterate through all the elements and insert them into the return vector.
  std::vector<BeaconID> all_beacons();

  // Estimate of performance: O(log(N))
  // Short rationale for estimate: Uses std::map<> operator [] to insert elements.
  bool add_beacon(BeaconID id, std::string const &name, Coord xy, Color color);

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::find() to validate the given id.
  std::string get_name(BeaconID id);

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::find() to validate the given id.
  Coord get_coordinates(BeaconID id);

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::find() to validate the given id.
  Color get_color(BeaconID id);

  // Estimate of performance: O(Nlog(N))
  // Short rationale for estimate: Uses std::multimap<> insertion method functions which have logarithmic complexity but then iterates through the map to create a vector which has O(N) complexity.
  std::vector<BeaconID> beacons_alphabetically();

  // Estimate of performance: O(Nlog(N))
  // Short rationale for estimate: Similar to beacons_alphabetically().
  std::vector<BeaconID> beacons_brightness_increasing();

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::min_element().
  BeaconID min_brightness();

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses std::max_element().
  BeaconID max_brightness();

  // Estimate of performance: O(Nlog(N))
  // Short rationale for estimate: Uses std::sort().
  std::vector<BeaconID> find_beacons(std::string const &name);

  // Estimate of performance: O(log(N))
  // Short rationale for estimate: Uses std::map.find().
  bool change_beacon_name(BeaconID id, std::string const &newname);

  // Estimate of performance: O(log(N))
  // Short rationale for estimate: Uses std::map.find().
  bool change_beacon_color(BeaconID id, Color newcolor);

  // Estimate of performance: O(log(N))
  // Short rationale for estimate: Uses std::map.find().
  bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

  // Estimate of performance: O(log(N))
  // Short rationale for estimate: Uses std::map.find().
  std::vector<BeaconID> get_lightsources(BeaconID id);

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses a single while() loop to fill the return vector.
  std::vector<BeaconID> path_outbeam(BeaconID id);

  // Estimate of performance: O(N)
  // Short rationale for estimate: Uses a single for() loop to empty the beamSources_ vector.
  bool remove_beacon(BeaconID id);

  // Estimate of performance: O(N^2)
  // Short rationale for estimate: Uses a nested for loop to fill the return vector.
  std::vector<BeaconID> path_inbeam_longest(BeaconID id);

  // Estimate of performance: O(Nlog(N))
  // Short rationale for estimate: Uses post-order tree traversal algorithm.
  Color total_color(BeaconID id);

private:
  std::map<BeaconID, info> map_;
  std::multimap<std::string, BeaconID> alphabetical_map_;
  std::multimap<int, BeaconID> brightness_map_;
};

#endif // DATASTRUCTURES_HH