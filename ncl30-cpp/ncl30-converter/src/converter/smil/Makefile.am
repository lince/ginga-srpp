
# set the include path found by configure
INCLUDES = -I/usr/local/include/directfb/ -I/usr/local/include/ginga/ \
$(all_includes)

LIBS =

libncl30consmlncmdir = /usr/local/lib/ginga/converters

libncl30consmlncm_so_LDFLAGS = -Wl,-soname,libncl30consmlncm.so -shared \
    -L/usr/local/lib -L/usr/local/lib/ginga -L../framework -L../framework/ncl

libncl30consmlncm_PROGRAMS = libncl30consmlncm.so

libncl30consmlncm_so_SOURCES = SMILNCMDocumentConverter.cpp

libncl30consmlncm_so_LDADD = -lncl30 -ltelemidiautil -lxerces-c \
    -lncl30confraxml -lncl30confrancl
