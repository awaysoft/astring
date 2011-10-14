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

static void astring_realloc(AString *string, asize size)
{
	int alloc = (size / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	string->str = (char *)realloc(string->str, alloc);
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

AString *astring_assign(AString *string, const char * value)
{
	int len = strlen(value);
	if (string -> allocated_len < len){
		astring_realloc(string, len);
	}
	strcpy(string->str, value);
	string->str[len] = '\0';
	string->len = len;
	return string;
}

AString *astring_dump(AString *source)
{
	AString *dest = astring_new_empty(source->allocated_len);
	dest->len = source->len;
	strcpy(dest->str, source->str);
	return dest;
}

AString *astring_append(AString *string, const char *value)
{
	int len = strlen(value);
	if (len + string->len > string->allocated_len){
		astring_realloc(string, len+string->len);
	}
	strcat(string->str, value);
	return string;
}

AString *astring_append_c(AString *string, char c)
{
	int len = string->len + 1;
	if (len > string->allocated_len){
		astring_realloc(string, len);
	}
	string->str[string->len] = c;
	string->len++;
	string->str[string->len] = '\0';
	return string;
}

AString *astring_append_unichar(AString *string, aunichar wc)
{
	int len = string->len + 4, i;
	if (len > string->allocated_len){
		astring_realloc(string, len);
	}
	for (i = 0; i < 4; ++i){
		string->str[string->len] = wc & 255;
		string->len++;
		wc >> 8;
	}
	string->str[string->len] = '\0';
	return string;
}

AString *astring_append_len(AString *string, const char *value, asize len)
{
	int i;
	if (string->len + len > string->allocated_len){
		astring_realloc(string, astring->len + len);
	}
	for (i = 0; i < len; ++i){
		if (value[i] == '\0') break;
		string->str[string->len + i] = value[i];
	}
	string->str[string->len + i] = '\0';
	string->len = string->len + i;
	return string;
}

void astring_free (AString *string)
{
	if (string == NULL) return ;
	free(string->str);
	free(string);
}
