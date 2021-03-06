///////////////////////////////////////////////////////////////////////
// registry.cpp -- Registry Object
// Date: Thu Oct 24 20:17:20 2013   (C) Warren Gay VE3WWG
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "registry.hpp"

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////

Registry::Registry() {
	next_id = 1;			// Next ID value to be allocated
}

//////////////////////////////////////////////////////////////////////
// Create a new symbol
//////////////////////////////////////////////////////////////////////

regid_t
Registry::create(const std::string& name) {
	auto it = fwdmap.find(name);
	if ( it != fwdmap.end() )
		return it->second;	// Return existing ID

	regid_t id = next_id++;		// Allocate an ID value
	fwdmap[std::string(name)] = id;	// Create a new entry
	revmap[id] = name;		// Create a reverse map entry
	return id;
}

//////////////////////////////////////////////////////////////////////
// Lookup a symbol
//////////////////////////////////////////////////////////////////////

regid_t
Registry::lookup(const std::string& name) const {
	auto it = fwdmap.find(name);
	if ( it == fwdmap.end() )
		return 0;		// Not found
	return it->second;		// Return it's ID
}

//////////////////////////////////////////////////////////////////////
// Symbol reverse lookup
//////////////////////////////////////////////////////////////////////

const std::string&
Registry::reverse(regid_t id) const {
	auto it = revmap.find(id);
	assert(it != revmap.end());
	return it->second;
}

//////////////////////////////////////////////////////////////////////
// Erase a symbol
//////////////////////////////////////////////////////////////////////

void
Registry::erase(const std::string& name) {
	auto it = fwdmap.find(name);
	assert(it != fwdmap.end());
	regid_t id = it->second;
	fwdmap.erase(name);
	revmap.erase(id);
}

//////////////////////////////////////////////////////////////////////
// Erase a symbol by id
//////////////////////////////////////////////////////////////////////

void
Registry::erase(regid_t id) {
	auto it = revmap.find(id);
	assert(it != revmap.end());
	std::string& name = it->second;
	fwdmap.erase(name);
	revmap.erase(id);
}

//////////////////////////////////////////////////////////////////////
// Get a beginning iterator
//////////////////////////////////////////////////////////////////////

Registry::iter_t
Registry::begin() {
	return fwdmap.begin();
}

//////////////////////////////////////////////////////////////////////
// Get an ending iterator
//////////////////////////////////////////////////////////////////////

Registry::iter_t
Registry::end() {
	return fwdmap.end();
}

//////////////////////////////////////////////////////////////////////
// Lookup symbol with []  (aborts if non-existant)
//////////////////////////////////////////////////////////////////////

regid_t
Registry::operator[](const std::string& name) const {
	regid_t id = lookup(name);
	assert(id != 0);
	return id;
}

//////////////////////////////////////////////////////////////////////
// Lookup a symbol by id  (aborts if non-existant)
//////////////////////////////////////////////////////////////////////

const std::string&
Registry::operator[](regid_t id) const {
	return reverse(id);
}

//////////////////////////////////////////////////////////////////////
// Clear the registry
//////////////////////////////////////////////////////////////////////

void
Registry::clear() {
	fwdmap.clear();
	revmap.clear();
	next_id = 1;
}

// End registry.cpp
