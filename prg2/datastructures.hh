// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <deque>


// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection, Distance>>;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
  int x = NO_VALUE;
  int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); } // Not strictly necessary

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

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
  double dist_a_sq = (c1.x * c1.x) + (c1.y * c1.y);
  double dist_b_sq = (c2.x * c2.x) + (c2.y * c2.y);
  return dist_a_sq == dist_b_sq ? c1.y < c2.y : dist_a_sq < dist_b_sq;
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

struct Connection
{
  AffiliationID aff1 = NO_AFFILIATION;
  AffiliationID aff2 = NO_AFFILIATION;
  Weight weight = NO_WEIGHT;
  bool operator==(const Connection &c1) const
  {
    return (aff1 == c1.aff1) && (aff2 == c1.aff2) && (weight == c1.weight);
  }
};
const Connection NO_CONNECTION{NO_AFFILIATION, NO_AFFILIATION, NO_WEIGHT};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
  NotImplemented() : msg_{} {}
  explicit NotImplemented(std::string const &msg) : msg_{msg + " not implemented"} {}

  virtual const char *what() const noexcept override
  {
    return msg_.c_str();
  }

private:
  std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
  Datastructures();
  ~Datastructures();

  // Estimate of performance:
  // Short rationale for estimate:
  unsigned int get_affiliation_count();

  // Estimate of performance:
  // Short rationale for estimate:
  void clear_all();

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<AffiliationID> get_all_affiliations();

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_affiliation(AffiliationID id, Name const &name, Coord xy);

  // Estimate of performance:
  // Short rationale for estimate:
  Name get_affiliation_name(AffiliationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  Coord get_affiliation_coord(AffiliationID id);

  // We recommend you implement the operations below only after implementing the ones above

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<AffiliationID> get_affiliations_alphabetically();

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<AffiliationID> get_affiliations_distance_increasing();

  // Estimate of performance:
  // Short rationale for estimate:
  AffiliationID find_affiliation_with_coord(Coord xy);

  // Estimate of performance:
  // Short rationale for estimate:
  bool change_affiliation_coord(AffiliationID id, Coord newcoord);

  // We recommend you implement the operations below only after implementing the ones above

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_publication(PublicationID id, Name const &name, Year year, const std::vector<AffiliationID> &affiliations);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<PublicationID> all_publications();

  // Estimate of performance:
  // Short rationale for estimate:
  Name get_publication_name(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  Year get_publication_year(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<AffiliationID> get_affiliations(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_reference(PublicationID id, PublicationID parentid);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<PublicationID> get_direct_references(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<PublicationID> get_publications(AffiliationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  PublicationID get_parent(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<PublicationID> get_referenced_by_chain(PublicationID id);

  // Non-compulsory operations

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<PublicationID> get_all_references(PublicationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

  // Estimate of performance:
  // Short rationale for estimate:
  bool remove_affiliation(AffiliationID id);

  // Estimate of performance:
  // Short rationale for estimate:
  PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

  // Estimate of performance:
  // Short rationale for estimate:
  bool remove_publication(PublicationID publicationid);

  // PRG 2 functions:

  // Estimate of performance: O(n)
  // Short rationale for estimate: Iterate through unordered_map of adjacent affiliations
  std::vector<Connection> get_connected_affiliations(AffiliationID id);

  // Estimate of performance: O(n^2)
  // Short rationale for estimate: Iterate through all adjacents of all affiliations
  std::vector<Connection> get_all_connections();

  // Estimate of performance: O(n^2)
  // Short rationale for estimate: depth first search through all possible path is quadratic
  Path get_any_path(AffiliationID source, AffiliationID target);

  // PRG2 optional functions

  // Estimate of performance:
  // Short rationale for estimate:
  Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

  // Estimate of performance:
  // Short rationale for estimate:
  Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

  // Estimate of performance:
  // Short rationale for estimate:
  PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);

  // Helper functions
  void postorder_traversal(PublicationID root_id, std::vector<PublicationID> &store, bool isOriginalRoot);
  std::vector<AffiliationID> dfs(AffiliationID source, AffiliationID target, std::vector<AffiliationID>& path_nodes, std::unordered_set<AffiliationID>& path_nodes_set);


private:
  struct Affiliation
  {
    AffiliationID id;
    Name name;
    Coord xy;
    std::vector<PublicationID> publications;
    std::unordered_map<AffiliationID, Weight> connected_affiliations;
  };
  struct Publication
  {
    PublicationID id;
    Name name;
    Year year;
    std::vector<AffiliationID> affiliations;
    PublicationID parent_id;
    std::vector<PublicationID> children_ids;
  };
  std::unordered_map<AffiliationID, Affiliation> affiliations_map;
  std::unordered_map<PublicationID, Publication> publications_map;
  std::vector<AffiliationID> affiliations_id;
  std::map<Name, std::set<AffiliationID>> affiliations_map_sorted_name;
  std::vector<AffiliationID> affiliations_id_sorted_name;
  std::map<Coord, AffiliationID> affiliations_map_sorted_coord;
  std::vector<AffiliationID> affiliations_id_sorted_coord;
  bool affiliations_name_sorted = true;
  bool affiliations_coord_sorted = true;

  std::unordered_map<AffiliationID, std::unordered_map<AffiliationID, Weight>> all_connections;
};

#endif // DATASTRUCTURES_HH
