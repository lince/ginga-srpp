
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommenderschedulerdir = /usr/local/lib/ginga

libgingarecommenderscheduler_so_LDADD =

libgingarecommenderscheduler_so_LDFLAGS = -Wl,-soname,libgingarecommenderscheduler.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommenderscheduler_PROGRAMS = libgingarecommenderscheduler.so
libgingarecommenderscheduler_so_SOURCES = Scheduler.cpp
