#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

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
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
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

struct Affiliation
{
    AffiliationID affiliation_id;
    std::string affiliation_name;
    Coord location;
    std::vector<PublicationID> publications;

    Affiliation(AffiliationID i, const std::string& n, Coord l, std::vector<PublicationID> p) :
        affiliation_id(i), affiliation_name(n), location(l), publications(p) {}
};

struct Publication
{
    PublicationID publication_id;
    std::string title;
    int publication_year;
    std::vector<AffiliationID> affiliations;
    std::vector<PublicationID> references;
    PublicationID referenced_by;

    Publication(PublicationID i, const std::string t, int y, std::vector<AffiliationID> a, std::vector<PublicationID> r, PublicationID rb) :
        publication_id(i), title(t), publication_year(y), affiliations(a), references(r), referenced_by(rb) {}
};

// This is the class you are supposed to implement

class Datastructures
{
private:
    std::unordered_map<AffiliationID, Affiliation> affiliations;
    std::unordered_map<PublicationID, Publication> publications;
    std::map<Name, std::set<AffiliationID>> affiliations_map_sorted_name;
    std::vector<AffiliationID> affiliations_id_sorted_name;
    bool affiliations_name_sorted = true; 

public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Constant time complexity because it returns the size of the affiliations map
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity because it clears both the affiliations and publications maps.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity, where n is the number of affiliations.
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n logn)
    // Short rationale for estimate: Sorting has a time complexity of O(n logn), where n is the number of affiliations.
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n logn)
    // Short rationale for estimate: Sorting has a time complexity of O(n logn), where n is the number of affiliations.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity, where n is the number of affiliations.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity for map operations.
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity for map operations.
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity for map operations.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity for map operations.
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Logarithmic time complexity for map operations.
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n logn)
    // Short rationale for estimate: The loop over publications has a time complexity of O(n), and sorting has a time complexity of O(n logn).
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear time complexity.
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n²)
    // Short rationale for estimate: The function involves iterations through all references.
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(n logn)
    // Short rationale for estimate: The function involves sorting affiliations based on distance.
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n²)
    // Short rationale for estimate: Quadratic time complexity.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n²)
    // Short rationale for estimate: Quadratic time complexity.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n²)
    // Short rationale for estimate: Quadratic time complexity.
    bool remove_publication(PublicationID publicationid);

};

#endif // DATASTRUCTURES_HH

