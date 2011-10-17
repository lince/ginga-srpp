
# set the include path found by configure
INCLUDES = $(all_includes)
LIBS =

noinst_PROGRAMS = test

test_SOURCES = test.cpp

test_LDADD = -ltelemidiautil -lncl30 $(COMPONENTS_LIBS) -ldl

test_LDFLAGS = -L/usr/local/lib -L/usr/local/lib/ginga \
    -L../src/converter/framework -L../src/converter/framework/ncl \
    -L../src/converter/ncl -L../src/converter/smil
