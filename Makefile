# wee - Sat Sep  7 16:04:27 2013
# Warren W. Gay VE3WWG

CXX             = g++
INCL            = 
DEFNS		= -DUSE_NCURSES
CXXFLAGS        = -std=c++0x -Wall $(INCL) $(DEFNS)
OPTZ            = -g -O0
#LDFLAGS         = -Wl,--no-warn-search-mismatch -lncurses
LDFLAGS         = -lncurses

OBJS		= wee.o term.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $*.o

all:	wee

wee:	$(OBJS)
	$(CXX) $(OBJS) -o wee $(LDFLAGS)

clean:
	rm -f *.o errs.t

clobber: clean
	rm -f wee core*
	
backup:	clobber
	(cd .. && tar czf wee.tar.gz wee)
	@tar tzvf ../wee.tar.gz
	@(cd .. && ls -ld wee.tar.gz)

term.o:	term.hpp

# End
