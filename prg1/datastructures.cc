// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <algorithm>
#include <QDebug>

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

Datastructures::Datastructures()
{
    affiliations_database_ = {};
    affiliations_by_name_ = {};
    affiliations_by_coord_ = {};
    publications_database_ = {};

    stored_alphabetically_ = {};
    stored_distance_ = {};

    alphabetically_valid = false;
    distance_valid = false;
}

Datastructures::~Datastructures()
{
    // qDebug() << "destructing datastructure\n";

    for(const auto& affiliation : affiliations_database_)
    {
        delete affiliation.second;
    }

    for(const auto& publication : publications_database_)
    {
        delete publication.second;
    }

    clear_all();
}

Datastructures::Publication::Publication(PublicationID id, Name title, Year year) :
    id_(id), title_(title), publ_year_(year)
{
    references_ = {};
    parent_ = nullptr;
    affiliations_ = {};
}

Datastructures::Publication::~Publication()
{
    // qDebug() << "destructing " << QString::fromStdString(title_) <<"\n";

    Affiliation* ptr = nullptr;

    // remove publication from affiliations
    for(const auto& affiliation : affiliations_)
    {
        ptr = affiliation.second;
        ptr->publications_[id_] = nullptr;
        ptr->publications_.erase(id_);
    }
    affiliations_.clear();
    ptr = nullptr;

    // if parent exists, remove
    if(parent_ != nullptr)
    {
        parent_->references_.erase(id_);
        parent_ = nullptr;
    }

    // remove parenthood from references map :D
    for(std::pair<PublicationID, Publication*> reference : references_)
    {
        reference.second->parent_ = nullptr;
        reference.second = nullptr;
    }
    references_.clear();
}

Datastructures::Affiliation::Affiliation(AffiliationID id, Name name, Coord xy) :
    id_(id), name_(name), xy_(xy)
{
    publications_ = {};
}

Datastructures::Affiliation::~Affiliation()
{
    // qDebug() << "destructing " << QString::fromStdString(name_) <<"\n";

    Publication* ptr = nullptr;

    for(std::pair<PublicationID, Publication*> publication : publications_)
    {
        ptr = publication.second;
        ptr->affiliations_[id_] = nullptr;
        ptr->affiliations_.erase(id_);
    }

    ptr = nullptr;

    publications_.clear();
}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations_database_.size();
}

void Datastructures::clear_all()
{
    affiliations_database_.clear();
    publications_database_.clear();
    affiliations_by_name_.clear();
    affiliations_by_coord_.clear();

    alphabetically_valid = false;
    distance_valid = false;

    return;
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> vecAffils;
    vecAffils.reserve(affiliations_database_.size());

    for(const auto& affiliation : affiliations_database_) {
        vecAffils.push_back(affiliation.first);
    }
    return vecAffils;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    if(get_aff_ptr(id) != nullptr) return false;

    alphabetically_valid = false;
    distance_valid = false;

    Affiliation* new_affiliation = new Affiliation(id, name, xy);
    affiliations_by_name_.insert({name, id});
    affiliations_by_coord_.insert({xy, new_affiliation});
    return affiliations_database_.insert({id, new_affiliation}).second;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    Affiliation* aff = get_aff_ptr(id);
    if(aff == nullptr) return NO_NAME;

    return aff->name_;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    Affiliation* aff = get_aff_ptr(id);
    if(aff == nullptr) return NO_COORD;

    return aff->xy_;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    if(alphabetically_valid) return stored_alphabetically_;

    std::vector<AffiliationID> vecAffils;
    vecAffils.reserve(affiliations_by_name_.size());

    for(const auto& pair : affiliations_by_name_)
    {
        vecAffils.push_back(pair.second);
    }
    stored_alphabetically_ = vecAffils;
    alphabetically_valid = true;
    return vecAffils;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    if(distance_valid) return stored_distance_;

    std::vector<AffiliationID> vecAffils;
    vecAffils.reserve(affiliations_by_coord_.size());

    for(const auto& pair : affiliations_by_coord_)
    {
        vecAffils.push_back(pair.second->id_);
    }

    stored_distance_ = vecAffils;
    distance_valid = true;
    return vecAffils;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto aff_it = affiliations_by_coord_.find(xy);
    if(aff_it == affiliations_by_coord_.end()) return NO_AFFILIATION;
    return aff_it->second->id_;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    Affiliation* aff = get_aff_ptr(id);
    if(aff == nullptr) return false;

    affiliations_by_coord_[newcoord] = aff;
    affiliations_by_coord_.erase(aff->xy_);

    aff->xy_ = newcoord;

    distance_valid = false;
    return true;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    if(get_publ_ptr(id) != nullptr) return false;

    Publication* new_publication = new Publication(id, name, year); // create new publication

    publications_database_.insert({id, new_publication}); // add new publication

    // add links between publication and each affiliation
    if(affiliations.size() > 0) {
        for(const AffiliationID& aff_id : affiliations) {
            add_affiliation_to_publication(aff_id, id);
            }
        }
    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> vecPubl;
    vecPubl.reserve(publications_database_.size());

    for(const auto& publication : publications_database_)
    {
        vecPubl.push_back(publication.first);
    }
    return vecPubl;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    Publication* publ = get_publ_ptr(id);
    if(publ == nullptr) return NO_NAME;

    return publ->title_;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    Publication* publ = get_publ_ptr(id);
    if(publ == nullptr) return NO_YEAR;

    return publ->publ_year_;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    Publication* publ = get_publ_ptr(id);
    std::vector<AffiliationID> vecAffils;

    if(publ == nullptr) {vecAffils.push_back(NO_AFFILIATION); return vecAffils;}

    for(const auto& affiliation : publ->affiliations_)
    {
        vecAffils.push_back(affiliation.first);
    }
    return vecAffils;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    Publication* parent = get_publ_ptr(parentid);
    Publication* child = get_publ_ptr(id);

    if(parent == nullptr || child == nullptr) return false;

    parent->references_.insert({id, child});
    child->parent_ = parent;

    return true;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    Publication* publ = get_publ_ptr(id);
    std::vector<PublicationID> vecPubls;

    if(publ == nullptr) {vecPubls.push_back(NO_PUBLICATION); return vecPubls;}

    std::unordered_map<PublicationID, Publication*> references = publications_database_[id]->references_;

    for(const auto& reference : references)
    {
        vecPubls.push_back(reference.first);
    }
    return vecPubls;
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    Affiliation* aff = get_aff_ptr(affiliationid);
    Publication* publ = get_publ_ptr(publicationid);

    if(aff == nullptr || publ == nullptr) return false;

    // 2-way pointer links
    aff->publications_.insert({publicationid, publ});
    publ->affiliations_.insert({affiliationid, aff});

    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    Affiliation* aff = get_aff_ptr(id);
    std::vector<PublicationID> pubAffils;

    if(aff == nullptr) {pubAffils.push_back(NO_PUBLICATION); return pubAffils;}

    for(const auto& publication : aff->publications_)
    {
        pubAffils.push_back(publication.first);
    }
    return pubAffils;

}

PublicationID Datastructures::get_parent(PublicationID id)
{
    Publication* publ = get_publ_ptr(id);
    if(publ == nullptr) return NO_PUBLICATION;

    Publication* parent = publ->parent_;

    return parent == nullptr ? NO_PUBLICATION : parent->id_;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    std::vector<PublicationID> publications = get_publications(affiliationid);
    std::vector<std::pair<Year, PublicationID>> vecPairs;

    if(publications.size() > 0 && publications[0] == NO_PUBLICATION) {vecPairs.push_back({NO_YEAR, NO_PUBLICATION}); return vecPairs;}

    for (const PublicationID& id : publications) {
        if (publications_database_[id]->publ_year_ >= year) {
            vecPairs.emplace_back(publications_database_[id]->publ_year_, id);
        }
    }

    std::sort(vecPairs.begin(), vecPairs.end());

    return vecPairs;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> vecPubls;

    if(get_publ_ptr(id) == nullptr) {vecPubls.push_back(NO_PUBLICATION); return vecPubls;}

    PublicationID parentid = get_parent(id);

    while(parentid != NO_PUBLICATION)
    {
        vecPubls.push_back(parentid);
        parentid = get_parent(parentid);
    }
    return vecPubls;
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> vecPubls;

    if(get_publ_ptr(id) == nullptr) {vecPubls.push_back(NO_PUBLICATION); return vecPubls;}

    references_recursion(id, vecPubls);
    return vecPubls;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{   
    int return_amount = 3;

    std::map<Coord, AffiliationID> resultmap;
    Coord temp;

    for(const auto& pair : affiliations_by_coord_)
    {
        temp = pair.first;
        // qDebug() << QString::fromStdString(pair.second->id_) << " before " << temp.x << ' ' << temp.y << "\n";

        temp.x = temp.x - xy.x;

        temp.y = temp.y - xy.y;

        // qDebug() << QString::fromStdString(pair.second->id_) << " after " << temp.x << ' ' << temp.y << "\n";

        resultmap.insert({temp, pair.second->id_});
    }

    std::vector<AffiliationID> returnVec;

    int amount = 0;
    for(const auto& pair : resultmap)
    {
        returnVec.push_back(pair.second);
        if(++amount >= return_amount) break;
    }

    return returnVec;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    Affiliation* aff = get_aff_ptr(id);
    if(aff == nullptr) return false;

    affiliations_by_name_.erase(aff->name_);
    affiliations_by_coord_.erase(aff->xy_);

    delete aff;

    affiliations_database_[id] = nullptr;
    affiliations_database_.erase(id);

    aff = nullptr;

    alphabetically_valid = false;
    distance_valid = false;

    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    if(get_publ_ptr(id1) == nullptr || get_publ_ptr(id2) == nullptr) return NO_PUBLICATION;

    std::vector<PublicationID> parents1 = get_referenced_by_chain(id1);
    std::vector<PublicationID> parents2 = get_referenced_by_chain(id2);

    auto it_r_1 = parents1.rbegin();
    auto it_r_2 = parents2.rbegin();

    auto end_1 = parents1.rend();
    auto end_2 = parents2.rend();

    PublicationID closest = NO_PUBLICATION;

    while(it_r_1 != end_1 && it_r_2 != end_2)
    {
        if(*it_r_1 != *it_r_2) break;
        closest = *it_r_1;
        it_r_1++;
        it_r_2++;
    }
    return closest;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    Publication* publ = get_publ_ptr(publicationid);
    if(publ == nullptr) return false;

    delete publ;

    publications_database_[publicationid] = nullptr;
    publications_database_.erase(publicationid);

    publ = nullptr;

    return true;
}

// ------------------ own private functions -----------------------

/*
unsigned int Datastructures::distance_from_origin(const AffiliationID& id)
{
    Coord xy = affiliations_database_[id]->xy_;

    int x = xy.x;
    int y = xy.y;
    return x * x + y * y;
}

unsigned int Datastructures::distance_from_point(const AffiliationID& id, const Coord& xy)
{
    Coord xy_point = affiliations_database_[id]->xy_;

    int x = xy_point.x - xy.x;
    int y = xy_point.y - xy.x;
    return x * x + y * y;
}
*/

Datastructures::Affiliation* Datastructures::get_aff_ptr(const AffiliationID& id)
{
    auto aff = affiliations_database_.find(id);
    if(aff == affiliations_database_.end()) return nullptr;
    return aff->second;
}

Datastructures::Publication* Datastructures::get_publ_ptr(const PublicationID& id)
{
    auto publ = publications_database_.find(id);
    if(publ == publications_database_.end()) return nullptr;
    return publ->second;
}

void Datastructures::references_recursion(PublicationID id, std::vector<PublicationID>& output)
{
    // gets all the children of a node and puts them in a vector, does the same recursively to children
    std::vector<PublicationID> children = get_direct_references(id);

    for(const PublicationID& child : children)
    {
        output.push_back(child);
        references_recursion(child, output);
    }
}
