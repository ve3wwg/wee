//////////////////////////////////////////////////////////////////////
// bindings.hpp -- Key Bindings
// Date: Sat Oct  5 11:14:21 2013   (C) datablocks.net
///////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "term.hpp"

typedef void (*bindproc_t)(int prefix,bool has_prefix);

typedef std::vector<keych_t> keysequ_t;

class Key_Bindings {
	struct s_keynode {
		bindproc_t	proc;
		int		mapref;

		s_keynode() {
			proc = 0;
			mapref = -1;
		}
	};

	typedef std::unordered_map<int,s_keynode> bindmap_t;

	std::vector<bindmap_t>	bindmaps;
	std::unordered_set<int> freemaps;
	bindmap_t		rootmap;

protected:
	int allocmap();
	void freemap(int mapx);

public:	Key_Bindings();
	~Key_Bindings();

	bool bind(const keysequ_t& sequ,bindproc_t proc);
	bool unbind(const keysequ_t& sequ);
};

#endif // BINDINGS_HPP

// End bindings.hpp
