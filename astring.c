#include "astring.h"

static AString *astring_new_empty(asize size)
{
	AString *res = (AString *) malloc (sizeof(AString));
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;
	}
	
	res->len = 0;
	res->allocated_len = size;
	res->str = (char *) malloc (sizeof(char) * res->allocated_len);
	
	/* Malloc Failed */
	if (NULL == res->str) {
		free (res);
		return NULL;
	}
	
	res->str[0] = '\0';
	return res;
}

AString *astring_new (const char *init)
{
	AString *res = astring_new_empty(DEFAULT_SIZE);
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;	
	}
	
	/*Null String*/
	if (NULL == init) { 
		return res;
	}
	
	res->len = strlen (init);
	res->allocated_len += res->len / DEFAULT_SIZE * DEFAULT_SIZE;	
	res->str = (char *) realloc (res->str, sizeof(char) * res->allocated_len);	
	
	/*Null String*/
	if (NULL == res->str) { 
		free (res);
		return NULL;
	}
	
	strcpy (res->str, init);
	return res;
}

AString *astring_new_len (const char *init, asize len)
{
	AString *res = astring_new_empty(DEFAULT_SIZE);
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;	
	}
	
	/*Null String*/
	if (NULL == init) { 
		return res;
	}
	asize tLen = strlen (init);
	res->len = len + 1;
	res->allocated_len += res->len;	
	res->str = (char *) realloc (res->str, sizeof(char) * res->allocated_len);	
	
	/*Null String*/
	if (NULL == res->str) { 
		free (res);
		res = NULL;
		return NULL;
	}
	if (tLen > len) {
		tLen = len; 
	}
	strncpy (res->str, init, tLen);
	asize i = tLen;
	while (i < res->allocated_len) res->str[i++] = '\0';
	return res;
}

AString *astring_sized_new (asize size) 
{
	return astring_new_empty(size);
}

void astring_free (AString *string)
{
	if (string == NULL) return ;
	free(string->str);
	free(string);
}
