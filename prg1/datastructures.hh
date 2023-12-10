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
#include <unordered_set>
#include <map>
#include <memory>

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

    bool operator<(const Coord& other) const {
        int distance1 = x*x + y*y;
        int distance2 = other.x*other.x + other.y*other.y;
        if(distance1 == distance2) return y < other.y;
        return distance1<distance2;
    }

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
/*
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}
*/

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
    // Short rationale for estimate: returns stored value of size
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: depends linearly on the amount of data do be cleared
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear operation of copying all affiliations
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup of element in map is logarithmic and creation of affiliation constant
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup complexity in a map is logarithmic
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup complexity in a map is logarithmic
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: going through all elements in map
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: going through all elements in map
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear time adding links between affiliations and publication
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear time copying elements
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear time copying affiliation element ids
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linearly going through all direct reference elements
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linearly going through all publications of an affiliation
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lookup in map is logarithmic
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: usage of std::sort algorithm on elements
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear chain of all publication references of a publication
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: going through all elements in a tree
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: going through all affiliations O(n) and adding them to a map O(logn)
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: calls destructor that linearly goes through databases and removes references to affiliation
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: going through both publications' parents top to bottom, and comparing them until difference.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: calls destructor that linearly goes through databases and removes references to publication
    bool remove_publication(PublicationID publicationid);


private:

    bool alphabetically_valid;
    bool distance_valid;

    struct Affiliation; // declare Affiliation so it can be used in publication's affiliations_ set.

    struct Publication {
        PublicationID id_;
        Name title_;
        Year publ_year_;
        //children, references
        std::unordered_map<PublicationID, Publication*> references_;
        //parent, referenced by
        Publication* parent_;

        std::unordered_map<AffiliationID, Affiliation*> affiliations_;

        Publication(PublicationID id, Name title, Year year);
        ~Publication();
    };

    struct Affiliation {
        AffiliationID id_;
        Name name_;
        Coord xy_;

        std::unordered_map<PublicationID, Publication*> publications_;

        Affiliation(AffiliationID id, Name name, Coord xy);
        ~Affiliation();
    };

    // Affiliations DB
    std::unordered_map<AffiliationID, Affiliation*> affiliations_database_;
    // Affiliations sorted by Name
    std::map<Name, AffiliationID> affiliations_by_name_;
    // Affiliations sorted by Coord
    std::map<Coord, Affiliation*> affiliations_by_coord_;

    // Publications DB
    std::unordered_map<PublicationID, Publication*> publications_database_;

    std::vector<AffiliationID> stored_alphabetically_;
    std::vector<AffiliationID> stored_distance_;


    Affiliation* get_aff_ptr(const AffiliationID& id);

    Publication* get_publ_ptr(const PublicationID& id);

    // unsigned int distance_from_origin(const AffiliationID& id);

    // unsigned int distance_from_point(const AffiliationID& id, const Coord& xy);

    void references_recursion(PublicationID id, std::vector<PublicationID>& output);

};

#endif // DATASTRUCTURES_HH
