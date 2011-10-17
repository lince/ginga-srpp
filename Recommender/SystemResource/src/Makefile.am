
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommendersystemresourcedir = /usr/local/lib/ginga

libgingarecommendersystemresource_so_LDADD = -lgingarecommenderutil

libgingarecommendersystemresource_so_LDFLAGS = -Wl,-soname,libgingarecommendersystemresource.so \
    --shared -ldl -I/usr/local/include/ginga/ -D_GNU_SOURCE -L/usr/local/lib/ginga

libgingarecommendersystemresource_PROGRAMS = libgingarecommendersystemresource.so
libgingarecommendersystemresource_so_SOURCES = SystemResource.cpp

