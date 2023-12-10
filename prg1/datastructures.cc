#include "datastructures.hh"

#include <random>
#include <cmath>

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

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations.size();
}

void Datastructures::clear_all()
{
    affiliations.clear();
    publications.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> all_affiliations;
    all_affiliations.reserve(affiliations.size());

    for(const auto& pair : affiliations) {
        all_affiliations.push_back(pair.first);
    }
    return all_affiliations;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    auto it = affiliations.find(id);

    if(it == affiliations.end()) {
        std::vector<PublicationID> no_publications;
        affiliations.insert(std::pair{id, Affiliation(id, name, xy, no_publications)});
        return true;
    } else {
        return false;
    }
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = affiliations.find(id);

    if(it == affiliations.end()) {
        return NO_NAME;
    } else {
        return it->second.affiliation_name;
    }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = affiliations.find(id);

    if(it == affiliations.end()) {
        return NO_COORD;
    } else {
        return it->second.location;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    std::vector<std::pair<AffiliationID, Affiliation>> vectorPairs(affiliations.begin(), affiliations.end());

    std::sort(vectorPairs.begin(), vectorPairs.end(), [](const auto& a, const auto& b) {
        return a.second.affiliation_name < b.second.affiliation_name;
    });

    std::vector<AffiliationID> all_affiliations;
    all_affiliations.reserve(vectorPairs.size());

    for (const auto& pair : vectorPairs) {
        all_affiliations.push_back(pair.first);
    }

    return all_affiliations;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    std::vector<std::pair<AffiliationID, Affiliation>> vectorPairs(affiliations.begin(), affiliations.end());

    std::sort(vectorPairs.begin(), vectorPairs.end(), [](const auto& a, const auto& b) {
        double distanceA = std::sqrt(a.second.location.x * a.second.location.x + a.second.location.y * a.second.location.y);
        double distanceB = std::sqrt(b.second.location.x * b.second.location.x + b.second.location.y * b.second.location.y);

        if (distanceA != distanceB) {
            return distanceA < distanceB;
        }

        return a.second.location.y < b.second.location.y;
    });

    std::vector<AffiliationID> all_affiliations;
    all_affiliations.reserve(vectorPairs.size());

    for (const auto& pair : vectorPairs) {
        all_affiliations.push_back(pair.first);
    }

    return all_affiliations;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto it = std::find_if(affiliations.begin(), affiliations.end(),
                           [xy](const std::pair<AffiliationID, Affiliation>& pair) {
                               return pair.second.location == xy;
                           });

    if (it == affiliations.end()) {
        return NO_AFFILIATION;
    } else {
        return it->first;
    }
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = affiliations.find(id);

    if(it == affiliations.end()) {
        return false;
    } else {
        it->second.location = newcoord;
        return true;
    }
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations_id)
{
    auto it = publications.find(id);

    if(it != publications.end()) {
        return false;

    } else {
        std::vector<PublicationID> references;
        publications.insert(std::pair{id, Publication(id, name, year, affiliations_id, references, NO_PUBLICATION)});

        for(auto a : affiliations_id) {
            auto it = affiliations.find(a);
            it->second.publications.push_back(id);
        }
        return true;
    }
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> all_publications;
    all_publications.reserve(publications.size());

    for(auto publication : publications) {
        all_publications.push_back(publication.first);
    }
    return all_publications;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        return NO_NAME;
    } else {
        return it->second.title;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        return NO_YEAR;
    } else {
        return it->second.publication_year;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        std::vector<AffiliationID> no_affiliations;
        no_affiliations.push_back(NO_AFFILIATION);
        return no_affiliations;
    } else {
        return it->second.affiliations;
    }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it = publications.find(id);
    auto parent_it = publications.find(parentid);

    if(it == publications.end() or parent_it == publications.end()){
        return false;
    } else {
        parent_it->second.references.push_back(id);
        it->second.referenced_by = parentid;
        return true;
    }
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        std::vector<PublicationID> no_publications;
        no_publications.push_back(NO_PUBLICATION);
        return no_publications;
    } else {
        std::vector<PublicationID> direct_references = it->second.references;
        return direct_references;
    }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto it_publication = publications.find(publicationid);

    auto it_affiliation = affiliations.find(affiliationid);

    if(it_publication == publications.end() or it_affiliation == affiliations.end()){
        return false;
    } else {
        it_publication->second.affiliations.push_back(affiliationid);
        it_affiliation->second.publications.push_back(publicationid);
        return true;
    }
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    auto it = affiliations.find(id);

    if(it == affiliations.end()) {
        std::vector<PublicationID> no_publications;
        no_publications.push_back(NO_PUBLICATION);
        return no_publications;
    } else {
        return it->second.publications;
    }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        return NO_PUBLICATION;
    } else {
        return it->second.referenced_by;
    }
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    auto it = affiliations.find(affiliationid);

    if(it == affiliations.end()) {
        std::vector<std::pair<Year, PublicationID>> no_publications;
        no_publications.push_back(std::pair{NO_YEAR, NO_PUBLICATION});
        return no_publications;

    } else {
        std::vector<std::pair<Year, PublicationID>> publications_after;

        for(auto p : it->second.publications) {
            auto it_publication = publications.find(p);

            if(it_publication->second.publication_year >= year) {
                publications_after.push_back(std::pair{it_publication->second.publication_year, p});
            }
        }
        return publications_after;
    }
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        std::vector<PublicationID> no_publications;
        no_publications.push_back(NO_PUBLICATION);
        return no_publications;
    } else {
        std::vector<PublicationID> referenced_by;

        while(it->second.referenced_by != NO_PUBLICATION) {
            PublicationID parentid = it->second.referenced_by;
            referenced_by.push_back(parentid);
            auto parent_it = publications.find(parentid);
            it = parent_it;
        }
        return referenced_by;
    }
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    auto it = publications.find(id);

    if(it == publications.end()) {
        std::vector<PublicationID> no_publications;
        no_publications.push_back(NO_PUBLICATION);
        return no_publications;
    } else {
        std::set<PublicationID> all_references;
        std::vector<PublicationID> references = it->second.references;

        for(auto r : references) {
            all_references.insert(r);
        }

        auto it_set = all_references.begin();
        while(it_set != all_references.end()) {
            auto it_refrence = publications.find(*it_set);
            std::vector<PublicationID> new_references = it_refrence->second.references;

            for(auto r : new_references) {
                all_references.insert(r);
            }
            ++it_set;
        }

        std::vector<PublicationID> result;
        for(auto p : all_references) {
            result.push_back(p);
        }
        return result;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    std::vector<std::pair<AffiliationID, double>> affiliation_distances;

    for (const auto& affiliation : affiliations) {
        double distance = std::sqrt(std::pow(xy.x - affiliation.second.location.x, 2) +
                                    std::pow(xy.y - affiliation.second.location.y, 2));
        affiliation_distances.emplace_back(affiliation.first, distance);
    }

    std::sort(affiliation_distances.begin(), affiliation_distances.end(),
              [this](const auto& a, const auto& b) {
                  if (a.second != b.second) {
                      return a.second < b.second;
                  }
                  auto it_a = affiliations.find(a.first);
                  auto it_b = affiliations.find(b.first);
                  if (it_a != affiliations.end() && it_b != affiliations.end()) {
                      return it_a->second.location.y < it_b->second.location.y;
                  }
                  return false;
              });

    std::vector<AffiliationID> result;
    result.reserve(affiliation_distances.size());

    for (const auto& pair : affiliation_distances) {
        result.push_back(pair.first);
    }

    return result.size() >= 3 ? std::vector<AffiliationID>{result.begin(), result.begin() + 3} : result;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto it_affiliation = affiliations.find(id);

    if (it_affiliation == affiliations.end()) {
        return false;
    } else {
        for (auto pub_id : it_affiliation->second.publications) {
            auto it_pub = publications.find(pub_id);
            if (it_pub != publications.end()) {
                auto& pub_affiliations = it_pub->second.affiliations;
                auto it_pub_affiliation = std::find(pub_affiliations.begin(), pub_affiliations.end(), id);
                if (it_pub_affiliation != pub_affiliations.end()) {
                    pub_affiliations.erase(it_pub_affiliation);
                }
            }
        }

        affiliations.erase(it_affiliation);

        return true;
    }
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto it1 = publications.find(id1);
    auto it2 = publications.find(id2);

    if(it1 == publications.end() or it2 == publications.end()) {
        return NO_PUBLICATION;
    } else {
        std::vector<PublicationID> referenced_by_1;
        std::vector<PublicationID> referenced_by_2;

        while(it1->second.referenced_by != NO_PUBLICATION) {
            PublicationID parentid = it1->second.referenced_by;
            referenced_by_1.push_back(parentid);
            auto parent_it = publications.find(parentid);
            it1 = parent_it;
        }

        while(it2->second.referenced_by != NO_PUBLICATION) {
            PublicationID parentid = it2->second.referenced_by;
            referenced_by_2.push_back(parentid);
            auto parent_it = publications.find(parentid);
            it2 = parent_it;
        }

        auto iter = referenced_by_1.begin();
                while (iter != referenced_by_1.end()) {
                    auto finder = find(referenced_by_2.begin(), referenced_by_2.end(), *iter);
                    if (finder != referenced_by_2.end()) {
                        return *iter;
                    } else {
                        ++iter;
                    }
                }
                return NO_PUBLICATION;
    }
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto it = publications.find(publicationid);

    if (it == publications.end()) {
        return false;
    } else {
        for (auto a : it->second.affiliations) {
            auto it_a = affiliations.find(a);
            if (it_a != affiliations.end()) {
                auto& aff_pub = it_a->second.publications;
                auto aff_pub_it = std::find(aff_pub.begin(), aff_pub.end(), publicationid);
                if (aff_pub_it != aff_pub.end()) {
                    aff_pub.erase(aff_pub_it);
                }
            }
        }

        for (auto r : it->second.references) {
            auto it_ref = publications.find(r);
            if (it_ref != publications.end()) {
                it_ref->second.referenced_by = NO_PUBLICATION;
            }
        }


        PublicationID referenced_by = it->second.referenced_by;
        if (referenced_by != NO_PUBLICATION) {
            auto it_rb = publications.find(referenced_by);
            if (it_rb != publications.end()) {
                auto& rb_references = it_rb->second.references;
                auto it_rb_references = std::find(rb_references.begin(), rb_references.end(), publicationid);
                if (it_rb_references != rb_references.end()) {
                    rb_references.erase(it_rb_references);
                }
            }
        }

        publications.erase(it);

        return true;
    }
}