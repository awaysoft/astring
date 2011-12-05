all:astring astringlist
astring:astring.c astring.h atype.h
	gcc -c astring.c -g

astringlist:astringlist.c astringlist.h astring
	gcc -c astringlist.c -g

