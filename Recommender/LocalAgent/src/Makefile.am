
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommenderlocalagentdir = /usr/local/lib/ginga

libgingarecommenderlocalagent_so_LDADD = -lgingarecommenderdatabase -lgingarecommenderagentlistener -lgingarecommenderthread -lgingaccplayer

libgingarecommenderlocalagent_so_LDFLAGS = -Wl,-soname,libgingarecommenderlocalagent.so \
    --shared -ldl -I/usr/local/include/ginga/ -L./thread

libgingarecommenderlocalagent_PROGRAMS = libgingarecommenderlocalagent.so
libgingarecommenderlocalagent_so_SOURCES = LocalAgent.cpp User.cpp IteractionBase.cpp Iteraction.cpp Program.cpp Channel.cpp Meta.cpp Volume.cpp Key.cpp NclStateMachine.cpp Document.cpp Context.cpp Media.cpp Device.cpp
