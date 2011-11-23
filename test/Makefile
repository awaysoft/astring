all:test testlist
test:test.c ../astring.o
	gcc -o test test.c ../astring.o -g
testlist:testlist.c ../astringlist.o ../astring.o
	gcc -o testlist testlist.c ../astringlist.o ../astring.o -g

