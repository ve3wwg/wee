//////////////////////////////////////////////////////////////////////
// registry.hpp -- Registry Object
// Date: Thu Oct 24 20:09:18 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "types.hpp"

#include <string>
#include <unordered_map>


class Registry {
	typedef std::unordered_map<std::string,id_t>  fwdmap_t;
	typedef std::unordered_map<id_t,std::string>  revmap_t;
	typedef fwdmap_t::iterator iter_t;

	regid_t		next_id;	// Next ID value to be allocated
	fwdmap_t	fwdmap;		// string => id map
	revmap_t	revmap;		// id => string map

public:	Registry();

	void clear();					// Clear registry
	regid_t create(const std::string& name);		// Create symbol, returns id
	regid_t lookup(const std::string& name) const;	// Lookup symbol name
	const std::string& reverse(regid_t id) const;	// Lookup id to return symbol name
	void erase(const std::string& name);		// Destroy by symbol name
	void erase(regid_t id);				// Destroy by id value

	regid_t operator[](const std::string& name) const;	// Lookup by name
	const std::string& operator[](regid_t id) const;	// Lookup by id

	iter_t begin();					// Begin iteration
	iter_t end();					// End iterator

	inline regid_t get_next_id() { return next_id; }
};

#endif // REGISTRY_HPP

// End registry.hpp
