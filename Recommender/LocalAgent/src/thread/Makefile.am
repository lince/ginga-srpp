
# set the include path found by configure
INCLUDES = $(all_includes)
LIBS =
# the library search path.
localLibdir = /usr/local/lib/ginga

libgingarecommenderthread_so_LDFLAGS = -Wl,-soname,libgingarecommenderthread.so \
    -shared

localLib_PROGRAMS = libgingarecommenderthread.so
libgingarecommenderthread_so_SOURCES = Thread.cpp
libgingarecommenderthread_so_LDADD = -lpthread
