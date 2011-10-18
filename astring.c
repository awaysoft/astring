#include "astring.h"

#define a_new(type, size) (type *)malloc(sizeof(type) * (size))

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

/*  Reallocat String And Keep All Data! */
static void astring_realloc(AString *string, asize size)
{
	int alloc = (size / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	string->str = (char *)realloc(string->str, alloc);
	string->allocated_len = size;
}

/*  Reallocat String And Clear All Data! */
static void astring_resize(AString *string, asize size)
{
	int alloc = (size / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	free(string->str);
	string->str = a_new(char, alloc);
	string->len = 0;
	string->allocated_len = size;
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

char *astring_dumpstr(AString *source)
{
	char *result = a_new(char, source->len + 1);
   	strcpy(result, source->str);
	return result;	
}

AString *astring_append(AString *string, const char *value)
{
	int len = strlen(value);
	if (len + string->len > string->allocated_len){
		astring_realloc(string, len+string->len);
	}
	strcat(string->str, value);
	string->len += len;
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
	int slen = strlen(value);
	if (len > slen) len = slen;
	if (string->len + len > string->allocated_len){
		astring_realloc(string, string->len + len);
	}
	for (i = 0; i < len; ++i){
		string->str[string->len + i] = value[i];
	}
	string->str[string->len + i] = '\0';
	string->len += i;
	return string;
}

AString * astring_prepend(AString *string, const char *value)
{
	int len = strlen(value);
	/*  Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	/*  Copy data */
	if (string->len + len > string->allocated_len) astring_resize(string, len + string->len);
	strcpy(string->str, value);
	strcat(string->str, tempstr);
	free(tempstr);
	string->len += len;

	return string;
}

AString * astring_prepend_c(AString *string, char c)
{
	int len = string->len + 1;
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	/*  Copy Data */
	if (len > string->allocated_len) astring_resize(string, len);
	string->str[0] = c; string->str[1] = '\0';
	strcat(string->str, tempstr);
	free(tempstr);
	string->len = len;

	return string;
}

AString * astring_prepend_unichar(AString *string, aunichar wc)
{
	int len = string->len + 4, i;
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	if (len > string->allocated_len) astring_resize(string, len);
	for (i = 0; i < 4; ++i){
		string->str[i] = wc & 255;
		wc >> 8;
	}
	string->str[4] = '\0';
	strcat(string->str, tempstr);
	free(tempstr);
	string->len = len;

	return string;
}

AString * astring_prepend_len(AString *string, const char *value, asize len)
{
	int slen = strlen(value), i;
   	if (slen < len) len = slen;
	
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);
	
	/*  Copy Data */
	if (len + string->len > string->allocated_len) astring_resize(string, len + string->len);
	for (i = 0; i < len; ++i){
		string->str[i] = value[i];
	}
	string->str[i] = '\0';
	strcat(string->str, tempstr);
	free(tempstr);
	string->len += len;

	return string;
}

AString *      astring_insert                (AString *string,
						                      asize pos,
					                          const char *value)
{

	return string;
}
AString *      astring_insert_c              (AString *string,
						                      asize pos,
						                      char c)
{

	return string;
}
AString *      astring_insert_unichar        (AString *string,
						                      asize pos,
						                      aunichar wc)
{

	return string;
}
AString *      astring_insert_len            (AString *string,
						                      asize pos,
						                      const char *value,
						                      asize len)
{

	return string;
}
						
AString *      astring_overwrite             (AString *string,
                                              asize pos,
                                              const char *value)
{

	return string;
}
AString *      astring_overwrite_len         (AString *string,
                                              asize pos,
                                              const char *value,
                                              asize len)
{

	return string;
}

AString *      astring_erase                 (AString *string,
											  asize pos,
											  asize len)
{

	return string;
}
											  

AString *      astring_truncate              (AString *string,
											  asize len)
{

	return string;
}
											  
											  
AString *      astring_set_size              (AString *string,
											  asize len)
{

	return string;
}

AString *      astring_trim                  (AString *string)
{

	return string;
}


auint          astring_hash                  (AString *string)
{

	return 0;
}

aboolean       astring_equal                 (AString *string,
                                              AString *string2)
{

	return 1;
}


void astring_free (AString *string)
{
	if (string == NULL) return ;
	free(string->str);
	free(string);
}

