
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingaexportxmldir = /usr/local/lib/ginga

libgingaexportxml_so_LDADD = -lxerces-c -lgingarecommenderdatabase -lgingarecommenderutil -lgingacccontextmanager -lgingacccm

libgingaexportxml_so_LDFLAGS = -Wl,-soname,libgingaexportxml.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingaexportxml_PROGRAMS = libgingaexportxml.so
libgingaexportxml_so_SOURCES = createXML.cpp
