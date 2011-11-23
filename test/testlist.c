#include <stdio.h>
#include "../astringlist.h"

void print(AStringList *list)
{
	if (!A_IS_STRING_LIST(list)) return;
	printf("========================================\n");
	printf("StringList Info\n");
	printf("StringList->length:%u\n", list->length);
	printf("StringList->allocated_len:%d\n", list->allocated_len);
	printf("========================================\n");
}

int main()
{
	AStringList *str = NULL;
	/*  Test New */


	return 0;
}
