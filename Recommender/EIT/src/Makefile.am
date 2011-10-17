
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommendereitdir = /usr/local/lib/ginga

libgingarecommendereit_so_LDADD = 

libgingarecommendereit_so_LDFLAGS = -Wl,-soname,libgingarecommendereit.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommendereit_PROGRAMS = libgingarecommendereit.so
libgingarecommendereit_so_SOURCES = EIT.cpp
