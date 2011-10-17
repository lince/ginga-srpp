
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommendersdtdir = /usr/local/lib/ginga

libgingarecommendersdt_so_LDADD =

libgingarecommendersdt_so_LDFLAGS = -Wl,-soname,libgingarecommendersdt.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommendersdt_PROGRAMS = libgingarecommendersdt.so
libgingarecommendersdt_so_SOURCES = SDT.cpp
