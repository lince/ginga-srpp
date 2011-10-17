
# set the include path found by configure
INCLUDES = $(all_includes)
LIBS =

noinst_PROGRAMS = test

test_SOURCES = test.cpp

test_LDADD =  -ltelemidiautil -lgingaccsystemthread -lgingaccsystemio \
    -lgingaccsystemiocodemap -lgingaccsystemiodfb -lgingaccplayer \
    -lgingaccplaani -lgingaccsysctsimgdfb -lgingaccplaimg \
    -lncl30 -lgingancladaptation -lgingancladapter \
    -lginganclemconverter -lginganclfocus -lginganclmodel -lgingancl \
    -lginganclgfx -lginganclmd -lgingalssm  -lgingacccontextmanager -lgingacccgc -lgingarecommenderminingalgorithm\
    $(COMPONENTS_LIBS) $(TUNER_LIBS) $(TSPARSER_LIBS) $(DATAPROCESSING_LIBS) $(NCLET_LIBS) $(CGC_LIBS) $(RECOMMENDER_LIBS)

test_LDFLAGS = -L/lib/ -L/usr/lib/ -L/usr/local/lib/ \
    -L/usr/local/lib/ginga -L/usr/local/lib/ginga/iocontents \
    -L/usr/local/lib/ginga/players -L../src/
