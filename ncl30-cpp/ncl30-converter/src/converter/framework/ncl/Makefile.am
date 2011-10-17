
# set the include path found by configure
INCLUDES = -I/usr/local/include/ -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

libncl30confrancldir = /usr/local/lib/ginga/converters

libncl30confrancl_so_LDFLAGS = -Wl,-soname,libncl30confrancl.so -shared \
    -L/usr/local/lib -L/usr/local/lib/ginga -L../

libncl30confrancl_PROGRAMS = libncl30confrancl.so

libncl30confrancl_so_SOURCES = NclComponentsParser.cpp \
    NclConnectorsParser.cpp NclDocumentParser.cpp \
    NclImportParser.cpp NclInterfacesParser.cpp \
    NclLayoutParser.cpp NclLinkingParser.cpp \
    NclMetainformationParser.cpp NclPresentationControlParser.cpp \
    NclPresentationSpecificationParser.cpp \
    NclStructureParser.cpp NclTransitionParser.cpp

libncl30confrancl_so_LDADD = -lncl30 -ltelemidiautil -lxerces-c -lncl30confraxml
