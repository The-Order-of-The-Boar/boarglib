# boarglib - Boar Game Library
# See LICENSE file for copyright and license details.

include config.mk

SRC = algorithms.hpp core.hpp
OBJ = ${SRC:.hpp=.o}

all: boarglib

$(OBJ): config.mk


boarglib: $(OBJ)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SRC) -o $@.o

clean:
	rm -f boarglib $(OBJ) boarglib-$(VERSION).tar.gz

dist: clean
	mkdir -p boarglib-$(VERSION)
	cp -R LICENSE Makefile config.mk\
	README  $(SRC)   boarglib-$(VERSION)
	tar hcf boarglib-$(VERSION).tar boarglib-$(VERSION)
	gzip boarglib-$(VERSION).tar
	rm -rf boarglib-$(VERSION)

.PHONY: all options clean dist install uninstall

