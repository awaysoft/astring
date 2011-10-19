/*
 * =====================================================================================
 *
 *       Filename:  testtrim.c
 *
 *    Description:  testtrim
 *
 *        Version:  1.0
 *        Created:  10/19/2011 01:19:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "../astring.h"

void testtrim(char *str)
{
	AString *string = astring_new(str);
	astring_trim(string);
	printf("|%s| Trim From: |%s|\n", string->str, str);
}

int main()
{
	testtrim("                    	   		Hello World!	   	  	");
	testtrim("Hello World!");
	testtrim(" Hello World!");
	testtrim("Hello World! ");
	testtrim("	Hello World!	");

	return 0;
}
