#include <stdio.h>
#include "../astring.h"

void testtrim(char *str)
{
	AString *string = a_string_new(str);
	a_string_trim(string);
	printf("|%s| Trim From: |%s|\n", string->str, str);
}

void testfind(AString *string, char *findstr)
{
	int pos = a_string_find(string, findstr, 1);
	printf("%s Find %s:%d\n", string->str, findstr, pos);
}

void testreplace(AString *string, char *findstr, char *replacestr)
{
	printf("%s ", string->str);
	int pos = a_string_replace(string, findstr, replacestr, 0);
	printf("Find %s Replace %s:%d:%s\n", findstr, replacestr, pos, string->str);
}

void testreplaceall(AString *string, char *findstr, char *replacestr)
{
	printf("%s ", string->str);
	int pos = a_string_replace_all(string, findstr, replacestr);
	printf("Find %s Replace %s:%d:%s\n", findstr, replacestr, pos, string->str);
}

void print(AString *string)
{
	if (!A_IS_STRING(string)) return;
	printf("========================================\n");
	printf("String Info\n");
	printf("string->str:%s\n", string->str);
	printf("string->len:%d\n", string->len);
	printf("string->allocated_len:%d\n", string->allocated_len);
	printf("========================================\n");
}

int main()
{
	AString *str = NULL;
	/*  Test New */
	str = a_string_new("");
	print(str);
	a_string_free(str);

	str = a_string_new("Hellowalewjke");
	print(str);
	a_string_free(str);

	str = a_string_new_len("Helloskljflsadjlkfask", 21);
	print(str);
	a_string_free(str);

	str = a_string_sized_new(10000);
	print(str);
	a_string_free(str);

	str = a_string_new("helalklksakldfkasd");
	a_string_assign(str, "Hello");
	print(str);
	a_string_free(str);

	str = a_string_new("hellklasjdf");
	AString *str2 = a_string_dup(str);
	print(str2);
	a_string_free(str);
	a_string_free(str2);

	/*  Test append */
	printf("\n>>Testing Append<<\n");
	str = a_string_new("abcdefagaslksadjlkfjaskllkfaklsdflkalksvnalksjdflkjasdlkfjlaksjlkfajsldkfjladsjlkfsdjalkjdklsjakla");
	a_string_append(str, "ncdi");
	print(str);

	a_string_append_c(str, 'o');
	print(str);

	a_string_append_len(str, "1234567890", 5);
	print(str);
	a_string_free(str);

	/*  Test Trim */
	printf(">>Testing Trim<<\n");
	testtrim("                    	   		Hello World!	   	  	");
	testtrim("Hello World!");
	testtrim(" Hello World!");
	testtrim("Hello World! ");
	testtrim("	Hello World!	");
	
	/*  Test find */
	printf("\n>>Testing Find<<\n");
	str = a_string_new("HelloWhljaklslknvclasioerjlkasfdc");
	testfind(str, "ello");
	testfind(str, "jak");
	testfind(str, "Hell");
	testfind(str, "asio");
	testfind(str, "dkfaslj");
	testfind(str, "sfdc");
	a_string_free(str);

	/*  Test Replace */
	printf("\n>>Testing Replace<<\n");
	str = a_string_new("HelloHelloHelloHello");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	a_string_free(str);

	/*  Test Replace All */
	printf("\n>>Testing Replace All<<\n");
	str = a_string_new("HelloHelloHelloHello");
	testreplaceall(str, "Hello", "AWA");
	str = a_string_new("aHellobHelloHelloHello");
	testreplaceall(str, "Hello", "AWA");
	a_string_free(str);

	/*  Test Prepend */
	str = a_string_new("Hellowlkjalksdjkfa");
	a_string_prepend(str, "abc");
	print(str);

	a_string_prepend_c(str, 'o');
	print(str);

	a_string_prepend_len(str, "lkajdlkfjalksdfklajskldfklasjkflajlsdjflkasdjlkfjaslkjflksadjlkfjdsakljlfdskklfdj", 50);
	print(str);

	a_string_free(str);

	str = a_string_new("Hello\bWorld!\n");
	printf(">>>>>>%d\n", a_string_find(str, "\b", 0));
	
	/* Test Load File */
	printf("\n>>Testing Load File<<\n");
	str = a_string_get_file_content("test.c");
	print(str);
	a_string_free(str);

	/* Test SubString */
	printf("\n>>Testing SubString<<\n");
	str = a_string_new("012345678901234567890123456789");
	print(str);
	a_string_substring(str, 0, 28);
	print(str);
	str2 = a_string_substring_new(str, 10, 16);
	print(str2);
	a_string_substring(str2, 1, 3);
	print(str2);
	a_string_free(str);
	a_string_free(str2);

	a_string_assign(str, "Hello");
	print(str);

	/* Test Char */
	printf("\n>>Testing char<<\n");
	str = a_string_new("01234567890");
	printf(">>%c<<\n", a_string_get_char(str, 5));
	printf(">>%c<<\n", a_string_get_char(str, 11));
	a_string_set_char(str, 5, 'b');
	print(str);
	a_string_free(str);
	
	str = a_string_new("012345678901234567890");
	a_string_truncate(str, 5);
	print(str);
	a_string_free(str);

	/* Test Key/Value */
	printf("\n>>Testing Key/Value<<\n");
	str = a_string_new("hello=12");
	printf("String:|%s|\n", str->str);
	char *strc = a_string_get_key(str);
	printf("Key:|%s|\n", strc);
	free(strc);
	strc = a_string_get_value(str);
	printf("value:|%s|\n", strc);
	free(strc);
	a_string_free(str);
	
	/* Test Split */
	printf("\n>>Testing Split<<\n");
	str = a_string_new("/1hello//2ald///3sadfljsa///4sd/s//////5sdfs////6sdfs////7sfs/dfds//8sfdsfsd////a");
	printf("String:%s\n", str->str);
	AStringArray *asa = a_string_split(str, "//");
	printf("StringArray:\nSize:%d\n", asa->size);
	int i = 0;
	while(i < asa->size){
		printf("%d:%s\n", i, asa->data[i]->str);
		a_string_free(asa->data[i]);
		++i;
	}
	free(asa);
	a_string_free(str);


	return 0;
}
