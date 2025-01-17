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

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

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
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

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

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
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

    // Estimate of performance: O(1)
    // Short rationale for estimate: std::vector::size is constant
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::clear is linear 
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::push_back is constant once
    // reserved adequate memories and called n times
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: std::map::find and insert are logarithmic
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::push_back is constant once
    // reserved adequate memories and called n times running a for loop of n through a map
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::push_back is constant once
    // reserved adequate memories and called n times running a for loop of n through a map
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: std::map::find are logarithmic
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: std::map::find are logarithmic
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::insert is constant but
    // in hash collision case then it would be linear
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::push_back is constant once
    // reserved adequate memories and called n times
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: std::unordered_map::find is constant
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: std::unordered_map::find is constant
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is constant but
    // in hash collision case then it would be linear
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: std::unordered_map::find is constant
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: std::map::find and insert are logarithmic 
    // and run n times, therefore linearithmic
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: while loop until finding the root node is linear at worst
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

    // Helper functions
    void postorder_traversal(PublicationID root_id, std::vector<PublicationID> &store, bool isOriginalRoot);


private:
    struct Affiliation {
      AffiliationID id;
      Name name;
      Coord xy;
      std::vector<PublicationID> publications;
    };
    struct Publication {
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
};

#endif // DATASTRUCTURES_HH
