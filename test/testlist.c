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
		printf("StringList[%d]:|%s|\n", i, list->data[i]->str);
		i++;
	}
	printf("========================================\n");
}

static int a_string_cmp(const void *a, const void *b)
{
	return -strcmp((*(AString **)a)->str, (*(AString **)b)->str);
}

int main()
{
	AStringList *list = a_string_list_new();
	int len , i;
	aboolean booltmp;

	char *str = "Helle World\r\na1=12\nb1=32\nb3=23\rc4=34";
	AString *string = a_string_new(str), *string2 = a_string_new(""), *string3;
	printf("String:%s\n", string->str);
	/*  Test New */

	a_string_list_assign(list, string);
	print(list, 10);

	printf("================================\n");

	a_string_list_assign_split(list, string, a_string_new("="));
	print(list, 10);
	a_string_list_free(list);

	AStringList *tmplist = a_string_list_load_from_file("testlist.c");
	print(tmplist, 100);
	
	/* Test Append */
	printf(">>>Test Append <<<\n\n");
	list = a_string_list_load_from_file("test.c");
	
	a_string_list_append(list, tmplist);
	print(list, 10000);
	
	/* Test prepend */
	printf(">>>Test Prepend <<<\n\n");
	i = list->length - 1;
	while(i > 203){
		a_string_list_delete(list, i);
		--i;
	}
	a_string_list_prepend(list, tmplist);
	print(list, 10000);
	
	/* Test Add */
	printf(">>>Test Add <<<\n\n");
	a_string_list_add(list, string);
	print(list, 10000);
	
	
	/* Test Insert */
	printf(">>>Test Insert <<<\n\n");
	a_string_list_insert(list, 285, string);
	print(list, 10000);
	
	/* Test Delete */
	printf(">>>Test Delete <<<\n\n");
	a_string_list_delete(list, 287);
	print(list, 10000);
		
	/* Test Clear */
	printf(">>>Test Clear <<<\n\n");
	a_string_list_clear(list);
	print(list, 10000);
	
	printf("Change StringList\n\n");
	a_string_assign(string, "1=2\n5=6\n6=7\n7=8\n8=9\n2=3\n4=5\n9=1\n3=4\n1=2\n");
	a_string_list_assign(list, string);
	print(list, 10000);
	
	/* Test Exchange */
	printf(">>>Test Exchange <<<\n\n");
	a_string_list_exchange(list, 0, 7);
	print(list, 10000);
	
	/* Test Find */
	printf(">>>Test Find <<<\n\n");
	i = a_string_list_find(list, a_string_assign(string2, "5=6"), 0);
	printf("Find '5=6':%d\n", i);
	i = a_string_list_find_name(list, a_string_assign(string2, "6"), 0);
	printf("Find Name '6':%d\n", i);
	
	/* Test Get Value */
	printf(">>>Test Get Value <<<\n\n");
	string3 = a_string_list_get_value(list, i);
	printf("Find position %d:%s\n", i, string3->str);
	a_string_free(string3);
	
	/* Test Get */
	printf(">>>Test Get <<<\n\n");
	string3 = a_string_list_get_index(list, i);
	printf("Find position %d:%s\n", i, string3->str);
	
	/* Test Sort */
	printf(">>>Test Sort <<<\n\n");	
	printf("System Sorted:%s\n", a_string_list_sorted(list) == TRUE?"Sorted":"UnSorted");
	printf("User Sorted:%s\n", a_string_list_sorted_custom(list, a_string_cmp) == TRUE?"Sorted":"UnSorted");
	
	a_string_list_sort(list);
	print(list, 10000);	
	printf("System Sorted:%s\n", a_string_list_sorted(list) == TRUE?"Sorted":"UnSorted");
	printf("User Sorted:%s\n", a_string_list_sorted_custom(list, a_string_cmp) == TRUE?"Sorted":"UnSorted");
	
	a_string_list_sort_custom(list, a_string_cmp);
	print(list, 10000);	
	printf("System Sorted:%s\n", a_string_list_sorted(list) == TRUE?"Sorted":"UnSorted");
	printf("User Sorted:%s\n", a_string_list_sorted_custom(list, a_string_cmp) == TRUE?"Sorted":"UnSorted");
	
	/* Test dup */
	printf(">>>Test dup <<<\n\n");
	tmplist = a_string_list_dup(list);
	print(tmplist, 10000);
	a_string_list_free(tmplist);
	
	/* Test Get Text */
	printf(">>>Test Get Text <<<\n\n");
	string3 = a_string_list_get_text(list, 0);
	printf("String Content:%s\n", string3->str);
	
	/* Test Save File */
	printf(">>>Test Save File <<<\n\n");
	booltmp = a_string_list_save_to_file(list, "testlist.out", 0);
	printf("Save File Status:%s\n", booltmp?"OK":"Error");
	
	/* Test Equal */
	printf(">>>Test Equal <<<\n\n");
	tmplist = a_string_list_dup(list);
	booltmp = a_string_list_equal(list, tmplist);
	printf("Equal tmplist booltmp:%s\n", booltmp?"True":"False");
	a_string_list_add(tmplist, string);	
	booltmp = a_string_list_equal(list, tmplist);
	printf("Equal tmplist booltmp:%s\n", booltmp?"True":"False");
	a_string_list_free(tmplist);
	a_string_list_free(tmplist);
	
	
	printf(">>>Test Finished! <<<\n");
	return 0;
}

