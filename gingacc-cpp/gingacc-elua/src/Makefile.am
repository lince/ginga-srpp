
# set the include path found by configure
INCLUDES = -I/usr/local/include/ginga/ -I/usr/local/include/directfb/ \
	$(all_includes)

LIBS = 

localLibdir = /usr/local/lib/ginga/players/

libgingaccplaelua_so_LDFLAGS = -Wl,-E,-soname,libgingaccplaelua.so -shared \
	-L/usr/local/lib -L/usr/local/lib/ginga \
	-L../../../ -L/usr/local/lib/ginga/players -L../ -L/usr/local/lib/lua/5.1 \
	-L/usr/local/lib/lua/5.1/socket

localLib_PROGRAMS = libgingaccplaelua.so

libgingaccplaelua_so_SOURCES = LuaCanvas.cpp LuaEvent.cpp LuaPlayer.cpp LuaCGCPlayer.cpp

libgingaccplaelua_so_LDADD = -llua -lcore -ltelemidiautil \
     -lgingaccsystemthread -lgingaccsystemiocodemap -lgingaccplayer \
     -lgingaccplaimg $(COMPONENTS_LIBS)
