#include <stdio.h>
#include "../astring.h"

void testtrim(char *str)
{
	AString *string = astring_new(str);
	astring_trim(string);
	printf("|%s| Trim From: |%s|\n", string->str, str);
}

void testfind(AString *string, char *findstr)
{
	int pos = astring_find(string, findstr, 1);
	printf("%s Find %s:%d\n", string->str, findstr, pos);
}

void testreplace(AString *string, char *findstr, char *replacestr)
{
	printf("%s ", string->str);
	int pos = astring_replace(string, findstr, replacestr, 0);
	printf("Find %s Replace %s:%d:%s\n", findstr, replacestr, pos, string->str);
}

void testreplaceall(AString *string, char *findstr, char *replacestr)
{
	printf("%s ", string->str);
	int pos = astring_replace_all(string, findstr, replacestr);
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
	str = astring_new("");
	print(str);
	astring_free(str);

	str = astring_new("Hellowalewjke");
	print(str);
	astring_free(str);

	str = astring_new_len("Helloskljflsadjlkfask", 21);
	print(str);
	astring_free(str);

	str = astring_sized_new(10000);
	print(str);
	astring_free(str);

	str = astring_new("helalklksakldfkasd");
	astring_assign(str, "Hello");
	print(str);
	astring_free(str);

	str = astring_new("hellklasjdf");
	AString *str2 = astring_dump(str);
	print(str2);
	astring_free(str);
	astring_free(str2);

	/*  Test append */
	printf("\n>>Testing Append<<\n");
	str = astring_new("abcdefagaslksadjlkfjaskllkfaklsdflkalksvnalksjdflkjasdlkfjlaksjlkfajsldkfjladsjlkfsdjalkjdklsjakla");
	astring_append(str, "ncdi");
	print(str);

	astring_append_c(str, 'o');
	print(str);

	astring_append_len(str, "1234567890", 5);
	print(str);
	astring_free(str);

	/*  Test Trim */
	printf(">>Testing Trim<<\n");
	testtrim("                    	   		Hello World!	   	  	");
	testtrim("Hello World!");
	testtrim(" Hello World!");
	testtrim("Hello World! ");
	testtrim("	Hello World!	");
	
	/*  Test find */
	printf("\n>>Testing Find<<\n");
	str = astring_new("HelloWhljaklslknvclasioerjlkasfdc");
	testfind(str, "ello");
	testfind(str, "jak");
	testfind(str, "Hell");
	testfind(str, "asio");
	testfind(str, "dkfaslj");
	testfind(str, "sfdc");
	astring_free(str);

	/*  Test Replace */
	printf("\n>>Testing Replace<<\n");
	str = astring_new("HelloHelloHelloHello");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	testreplace(str, "Hello", "AWA");
	astring_free(str);

	/*  Test Replace All */
	printf("\n>>Testing Replace All<<\n");
	str = astring_new("HelloHelloHelloHello");
	testreplaceall(str, "Hello", "AWA");
	str = astring_new("aHellobHelloHelloHello");
	testreplaceall(str, "Hello", "AWA");
	astring_free(str);

	/*  Test Prepend */
	str = astring_new("Hellowlkjalksdjkfa");
	astring_prepend(str, "abc");
	print(str);

	astring_prepend_c(str, 'o');
	print(str);

	astring_prepend_len(str, "lkajdlkfjalksdfklajskldfklasjkflajlsdjflkasdjlkfjaslkjflksadjlkfjdsakljlfdskklfdj", 50);
	print(str);

	astring_free(str);

	str = astring_new("Hello\bWorld!\n");
	printf(">>>>>>%d\n", astring_find(str, "\b", 0));
	
	/* Test Load File */
	printf("\n>>Testing Load File<<\n");
	str = astring_get_file_content("test.c");
	print(str);
	astring_free(str);

	/* Test SubString */
	printf("\n>>Testing SubString<<\n");
	str = astring_new("012345678901234567890123456789");
	print(str);
	astring_substring(str, 0, 28);
	print(str);
	str2 = astring_substring_new(str, 10, 16);
	print(str2);
	astring_substring(str2, 1, 3);
	print(str2);
	astring_free(str);
	astring_free(str2);

	astring_assign(str, "Hello");
	print(str);

	/* Test Char */
	printf("\n>>Testing char<<\n");
	str = astring_new("01234567890");
	printf(">>%c<<\n", astring_get_char(str, 5));
	printf(">>%c<<\n", astring_get_char(str, 11));
	astring_set_char(str, 5, 'b');
	print(str);
	astring_free(str);


	return 0;
}
