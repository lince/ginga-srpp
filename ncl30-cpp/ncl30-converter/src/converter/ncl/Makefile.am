
# set the include path found by configure
INCLUDES = -I/usr/local/include/directfb/ -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

libncl30connclncmdir = /usr/local/lib/ginga/converters

libncl30connclncm_so_LDFLAGS = -Wl,-soname,libncl30connclncm.so -shared \
    -L/usr/local/lib -L/usr/local/lib/ginga -L../framework -L../framework/ncl \
    -L../ -L../smil/

libncl30connclncm_PROGRAMS = libncl30connclncm.so

libncl30connclncm_so_SOURCES = NclComponentsConverter.cpp \
    NclConnectorsConverter.cpp NclDocumentConverter.cpp \
    NclImportConverter.cpp NclInterfacesConverter.cpp \
    NclLayoutConverter.cpp NclLinkingConverter.cpp \
    NclMetainformationConverter.cpp NclPresentationControlConverter.cpp \
    NclPresentationSpecConverter.cpp NclStructureConverter.cpp \
    NclTransitionConverter.cpp

libncl30connclncm_so_LDADD = -lncl30 -ltelemidiautil -lxerces-c \
    -lncl30confraxml -lncl30confrancl -lncl30consmlncm
