all:astring.o astringlist.o
astring:astring.c astring.h atype.h astringlist
	gcc -c astring.c -g

astringlist:astringlist.c astringlist.h astring.o
	gcc -c astringlist.c astring.o -g

