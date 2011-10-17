
# set the include path found by configure
INCLUDES = -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

gingadir = /usr/local/sbin
ginga_PROGRAMS = ginga
ginga_SOURCES = main.cpp
ginga_LDADD = -ltelemidiautil -lgingacccgc -lgingaexportxml -lgingancladaptation \
              -lgingarecommenderminingalgorithm -lgingarecommenderscheduler $(COMPONENTS_LIBS) 

