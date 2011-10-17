
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommenderutildir = /usr/local/lib/ginga

libgingarecommenderutil_so_LDADD =

libgingarecommenderutil_so_LDFLAGS = -Wl,-soname,libgingarecommenderutil.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommenderutil_PROGRAMS = libgingarecommenderutil.so
libgingarecommenderutil_so_SOURCES = Utils.cpp
