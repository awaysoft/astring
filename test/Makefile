all:test testlist 
astring:../astring.c ../astring.h ../atype.h
	gcc -c ../astring.c -g

astringlist:../astringlist.c ../astringlist.h astring
	gcc -c ../astringlist.c -g


test:test.c astring
	gcc -o test test.c astring.o -g

testlist:testlist.c astring astringlist
	gcc -o testlist testlist.c astringlist.o astring.o -g

clean:
	rm *.o
