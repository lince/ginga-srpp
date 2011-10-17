
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingacccgcdir = /usr/local/lib/ginga

libgingacccgc_so_LDFLAGS = -Wl,-soname,libgingacccgc.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingacccgc_so_LDADD = -lncl30 -lncl30generator -lgingacctsparser

libgingacccgc_PROGRAMS = libgingacccgc.so
libgingacccgc_so_SOURCES = ProgramRecorder.cpp ProgramHeader.cpp
