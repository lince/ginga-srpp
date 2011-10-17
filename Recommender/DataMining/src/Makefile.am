
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommenderdataminingdir = /usr/local/lib/ginga

libgingarecommenderdatamining_so_LDADD = -lpthread -lgingarecommenderminingalgorithm -lgingarecommenderdatabase

libgingarecommenderdatamining_so_LDFLAGS = -Wl,-soname,libgingarecommenderdatamining.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommenderdatamining_PROGRAMS = libgingarecommenderdatamining.so
libgingarecommenderdatamining_so_SOURCES = DataMining.cpp
