
# set the include path found by configure
INCLUDES = $(all_includes)
LIBS =

noinst_PROGRAMS = test

test_SOURCES = main.cpp

test_LDADD = -lgingarecommenderdatabase -lpthread -lgingarecommenderminingalgorithm

test_LDFLAGS = -L/usr/local/lib/ginga -L../include/ -L../src/ -L/usr/local/lib
