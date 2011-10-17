
# set the include path found by configure
INCLUDES = -I/usr/local/include/ -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

libncl30confraxmldir = /usr/local/lib/ginga/converters

libncl30confraxml_so_LDFLAGS = -Wl,-soname,libncl30confraxml.so -shared \
    -L/usr/local/lib -L/usr/local/lib/ginga

libncl30confraxml_PROGRAMS = libncl30confraxml.so

libncl30confraxml_so_SOURCES = DOMTreeErrorReporter.cpp \
    DocumentParser.cpp XMLParsing.cpp

libncl30confraxml_so_LDADD = -lncl30 -ltelemidiautil -lxerces-c
