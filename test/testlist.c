#include <stdio.h>
#include "../astringlist.h"

void print(AStringList *list, int len)
{
	int i = 0;
	if (!A_IS_STRING_LIST(list)) return;
	printf("========================================\n");
	printf("StringList Info\n");
	printf("StringList->length:%u\n", list->length);
	printf("StringList->allocated_len:%d\n", list->allocated_len);
	while(i < len && i < list->length){
		printf("StringList[%d]:%s\n", i, list->data[i]->str);
		i++;
	}
	printf("========================================\n");
}

int main()
{
	AStringList *list = a_string_list_new();

	char *str = "Helle World\r\na1=12\nb1=32\nb3=23\rc4=34";
	AString *string = a_string_new(str);
	printf("String:%s\n", string->str);
	/*  Test New */

	a_string_list_assign(list, string);
	print(list, 10);

	printf("================================\n");

	a_string_list_assign_split(list, string, a_string_new("="));
	print(list, 10);
	a_string_list_clear(list);

	AStringList *tmplist = a_string_list_load_from_file("testlist.c");
	print(tmplist, 100);


	return 0;
}
