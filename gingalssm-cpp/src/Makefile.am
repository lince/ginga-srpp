
# set the include path found by configure
INCLUDES = -I/usr/local/include/directfb/ -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

libgingalssmdir = /usr/local/lib/ginga

libgingalssm_so_LDFLAGS = -L/lib/ -L/usr/lib/ -L/usr/local/lib/ \
    -L/usr/local/lib/ginga -L/usr/local/lib/ginga/iocontents \
    -L/usr/local/lib/ginga/players -Wl,-soname,libgingalssm.so -shared

libgingalssm_PROGRAMS = libgingalssm.so

libgingalssm_so_SOURCES = PresentationEngineManager.cpp CommonCoreManager.cpp \
    DataWrapperListener.cpp StcWrapper.cpp

libgingalssm_so_LDADD = -ltelemidiautil -lpthread -lgingaccsystemthread \
    -lgingaccsystemiocodemap -lncl30 $(COMPONENTS_LIBS) \
    $(TUNER_LIBS) $(TSPARSER_LIBS) $(DATAPROCESSING_LIBS) $(NCLET_LIBS) $(CGC_LIBS) $(RECOMMENDER_LIBS)
