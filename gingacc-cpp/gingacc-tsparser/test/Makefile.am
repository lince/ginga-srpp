
# set the include path found by configure
INCLUDES = $(all_includes)
LIBS =

noinst_PROGRAMS = test

test_SOURCES = test.cpp

test_LDADD = -lgingaccsystemthread -ltelemidiautil -lgingacctuner \
    -lgingacctsparser

test_LDFLAGS = -L/usr/local/lib/ginga -L../src/ -L/usr/local/lib
