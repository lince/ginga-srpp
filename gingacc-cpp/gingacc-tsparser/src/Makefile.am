
# set the include path found by configure
INCLUDES = -I/usr/local/include/ginga/ $(all_includes)

LIBS =

localLibdir = /usr/local/lib/ginga

libgingacctsparser_so_LDFLAGS = -Wl,-soname,libgingacctsparser.so \
	-shared -L/usr/local/lib -L/usr/local/lib/ginga

localLib_PROGRAMS = libgingacctsparser.so
libgingacctsparser_so_SOURCES = Demuxer.cpp FrontendFilter.cpp \
    PipeFilter.cpp PSIFilter.cpp \
    PesFilter.cpp TSPacket.cpp \
	TSSection.cpp si/Pat.cpp si/Pmt.cpp si/EventInfo.cpp \
	si/ServiceInfo.cpp si/descriptors/ExtendedEventDescriptor.cpp \
	si/descriptors/LogoTransmissionDescriptor.cpp \
	si/descriptors/ShortEventDescriptor.cpp

libgingacctsparser_so_LDADD = -ltelemidiautil -lpthread -lgingaccsystemthread \
    -lgingacctuner
