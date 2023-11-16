// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
  auto range = end - start;
  ++range;

  auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(rand_engine);

  return static_cast<Type>(start + num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
  // Write any initialization you need here
}

Datastructures::~Datastructures()
{
  // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
  return affiliations_map.size();
}

void Datastructures::clear_all()
{
  affiliations_map.clear();
  publications_map.clear();

  affiliations_map_sorted_name.clear();
  affiliations_id_sorted_name.clear();
  affiliations_id_sorted_name.shrink_to_fit();

  affiliations_map_sorted_coord.clear();
  affiliations_id_sorted_coord.clear();
  affiliations_id_sorted_coord.shrink_to_fit();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
  std::vector<AffiliationID> affiliation_ids;
  affiliation_ids.reserve(affiliations_map.size());

  for (const auto &affiliation : affiliations_map)
  {
    affiliation_ids.push_back(affiliation.first);
  }
  return affiliation_ids;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
  // std::unordered_map<std::string,double>::const_iterator
  auto it = affiliations_map.find(id);

  if (it == affiliations_map.end())
  {
    affiliations_map.insert({id, {id, name, xy, {}}});
    auto it_name = affiliations_map_sorted_name.find(name);
    if (it_name != affiliations_map_sorted_name.end())
    {
      it_name->second.insert(id);
    }
    else
    {
      affiliations_map_sorted_name.insert({name, {id}});
    }
    affiliations_map_sorted_coord.insert({xy, id});
    affiliations_name_sorted = false;
    affiliations_coord_sorted = false;
    return true;
  }
  return false;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
  auto it = affiliations_map.find(id);
  return (it != affiliations_map.end()) ? it->second.name : NO_NAME;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
  auto it = affiliations_map.find(id);
  return (it != affiliations_map.end()) ? it->second.xy : NO_COORD;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
  if (!affiliations_name_sorted)
  {
    affiliations_id_sorted_name.clear();
    affiliations_id_sorted_name.reserve(affiliations_map.size());
    for (const auto &aff : affiliations_map_sorted_name)
    {
      for (const auto &id : aff.second)
      {
        affiliations_id_sorted_name.push_back(id);
      }
    }
    affiliations_name_sorted = true;
  }

  return affiliations_id_sorted_name;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
  if (!affiliations_coord_sorted)
  {
    affiliations_id_sorted_coord.clear();
    affiliations_id_sorted_coord.reserve(affiliations_map.size());
    for (const auto &aff : affiliations_map_sorted_coord)
    {
      affiliations_id_sorted_coord.push_back(aff.second);
    }
    affiliations_coord_sorted = true;
  }

  return affiliations_id_sorted_coord;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
  auto it = affiliations_map_sorted_coord.find(xy);
  return it != affiliations_map_sorted_coord.end() ? it->second : NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
  auto it = affiliations_map.find(id);
  if (it == affiliations_map.end())
  {
    return false;
  }

  auto it_coord = affiliations_map_sorted_coord.find(it->second.xy);
  affiliations_map_sorted_coord.erase(it->second.xy);
  affiliations_map_sorted_coord[newcoord] = id;
  affiliations_coord_sorted = false;

  it->second.xy = newcoord;

  return true;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
  auto it = publications_map.find(id);

  if (it == publications_map.end())
  {
    publications_map.insert({id, {id, name, year, affiliations, NO_PUBLICATION, {}}});
    return true;
  }
  return false;
}

std::vector<PublicationID> Datastructures::all_publications()
{
  std::vector<PublicationID> publication_ids;
  publication_ids.reserve(publications_map.size());
  for (const auto &publication : publications_map)
  {
    publication_ids.push_back(publication.first);
  }
  return publication_ids;
}

Name Datastructures::get_publication_name(PublicationID id)
{
  auto it = publications_map.find(id);
  return (it != publications_map.end()) ? it->second.name : NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
  auto it = publications_map.find(id);
  return (it != publications_map.end()) ? it->second.year : NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
  auto it = publications_map.find(id);

  if (it != publications_map.end())
  {
    return it->second.affiliations;
  }
  else
  {
    std::vector<AffiliationID> affiliationNotFound = {NO_AFFILIATION};
    return affiliationNotFound;
  }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
  auto it1 = publications_map.find(id);
  auto it2 = publications_map.find(parentid);

  if (it1 != publications_map.end() && it2 != publications_map.end())
  {
    it1->second.parent_id = parentid;
    it2->second.children_ids.push_back(id);
    return true;
  }
  return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
  auto it = publications_map.find(id);

  if (it != publications_map.end())
  {
    return it->second.children_ids;
  }
  else
  {
    std::vector<PublicationID> publicationNotFound = {NO_PUBLICATION};
    return publicationNotFound;
  }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
  auto it1 = publications_map.find(publicationid);
  auto it2 = affiliations_map.find(affiliationid);

  if (it1 != publications_map.end() && it2 != affiliations_map.end())
  {
    it1->second.affiliations.push_back(affiliationid);
    it2->second.publications.push_back(publicationid);
    return true;
  }
  return false;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
  auto it = affiliations_map.find(id);
  if (it != affiliations_map.end())
  {
    return it->second.publications;
  }
  else
  {
    std::vector<PublicationID> publicationNotFound = {NO_PUBLICATION};
    return publicationNotFound;
  }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
  auto it = publications_map.find(id);
  return it != publications_map.end() ? it->second.parent_id : NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID>> Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
  auto it = affiliations_map.find(affiliationid);
  if (it != affiliations_map.end())
  {
    std::map<Year, std::set<PublicationID>> publications_map_sorted_year;
    for (const auto &pub_id : it->second.publications)
    {
      auto it_pub = publications_map.find(pub_id);
      Year year_after = it_pub->second.year;
      if (year_after >= year)
      {
        auto it_year = publications_map_sorted_year.find(year_after);
        if (it_year != publications_map_sorted_year.end())
        {
          it_year->second.insert(pub_id);
        }
        else
        {
          publications_map_sorted_year.insert({year_after, {pub_id}});
        }
      }
    }
    std::vector<std::pair<Year, PublicationID>> years;
    years.reserve(it->second.publications.size());
    for (const auto &pair : publications_map_sorted_year)
    {
      for (const PublicationID &id : pair.second)
      {
        years.push_back(std::make_pair(pair.first, id));
      }
    }
    publications_map_sorted_year.clear();
    return years;
  }
  return {{NO_YEAR, NO_PUBLICATION}};
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
  auto it = publications_map.find(id);
  if (it != publications_map.end())
  {
    std::vector<PublicationID> parents_chain;
    parents_chain.reserve(publications_map.size());
    while (it->second.parent_id != NO_PUBLICATION)
    {
      PublicationID parent_id = it->second.parent_id;
      parents_chain.push_back(parent_id);
      it = publications_map.find(parent_id);
    }
    parents_chain.shrink_to_fit();
    return parents_chain;
  }
  return {NO_PUBLICATION};
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
  auto it = publications_map.find(id);
  if (it == publications_map.end())
  {
    return {NO_PUBLICATION};
  }
  std::vector<PublicationID> store;
  store.reserve(publications_map.size());
  postorder_traversal(id, store, true);
  store.shrink_to_fit();
  return store;
}

void Datastructures::postorder_traversal(PublicationID root_id, std::vector<PublicationID> &store, bool isOriginalRoot)
{
  auto it = publications_map.find(root_id);
  for (const PublicationID &id : it->second.children_ids)
  {
    postorder_traversal(id, store, false);
  }
  if (!isOriginalRoot) store.push_back(root_id);
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_affiliations_closest_to()");
}

bool Datastructures::remove_affiliation(AffiliationID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("remove_affiliation()");
}

PublicationID Datastructures::get_closest_common_parent(PublicationID /*id1*/, PublicationID /*id2*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_closest_common_parent()");
}

bool Datastructures::remove_publication(PublicationID /*publicationid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("remove_publication()");
}
