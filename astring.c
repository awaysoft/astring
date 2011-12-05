#include "astring.h"
#include <stdio.h>
#include <stdlib.h>


#define a_new(type, size) (type *)malloc(sizeof(type) * (size))

/*  Get String's Lock and Lock
 *  String */
#define P(lock) while(lock++){--lock;}

/*  Unlock String */
#define V(lock) --lock;

#define A_WARING(msg) do{fprintf(stderr, "[WARING] %s: [%s():%d] %s\n", __FILE__, __func__, __LINE__, msg);}while(0)

#define A_WARING_NOT_STRING A_WARING("A_IS_STRING Check falt!")

static AString *a_string_new_empty(asize size)
{
	AString *res = (AString *) malloc (sizeof(AString));
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;
	}
	
	res->flag = A_STRING_FLAG;
	res->len = 0;
	res->allocated_len = size;
	res->str = a_new(char, res->allocated_len);
	res->lock = 0;

	/* Malloc Failed */
	if (NULL == res->str) {
		free (res);
		return NULL;
	}
	
	res->str[0] = ZERO;
	return res;
}

/*  Reallocat String And Keep All Data! */
static void a_string_realloc(AString *string, asize size)
{
	int alloc = (size / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	string->str = (char *)realloc(string->str, alloc);
	string->allocated_len = alloc;
}

/*  Reallocat String And Clear All Data! */
static void a_string_resize(AString *string, asize size)
{
	int alloc = (size / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	free(string->str);
	string->str = a_new(char, alloc);
	string->len = 0;
	string->allocated_len = alloc;
}

static AString *_a_string_assign(AString *string, const char * value)
{
	int len = strlen(value);
	if (string -> allocated_len < len){
		a_string_realloc(string, len);
	}
	strcpy(string->str, value);
	string->str[len] = ZERO;
	string->len = len;
	
	return string;
}

static AString *_a_string_dup(AString *source)
{
	AString *dest = a_string_new_empty(source->allocated_len);
	dest->len = source->len;
	strcpy(dest->str, source->str);
	
	return dest;
}

static char *_a_string_dupstr(AString *source)
{
	char *result = a_new(char, source->len + 1);
   	strcpy(result, source->str);
   	
	return result;
}

static size_t _a_string_file_length(FILE *fp)
{
	unsigned char temp[1000000];
	size_t count = 0;
	long pos;
	pos = ftell(fp);
	/* Pos to End */
	fseek(fp, 0L, 2);
	
	count = ftell(fp);
	
	fseek(fp, pos, 0);
	return count;
}

static AString * _a_string_get_file_content(const char * filename)
{
	FILE *fp;
	size_t flen, fr;
	AString *result = NULL;
	char *p;

	if ((fp = fopen(filename, "rb")) == NULL) return NULL;
	flen = _a_string_file_length(fp);

	fseek(fp, 0L, 0);
	result = a_string_new_empty(flen + 1);
	p = result -> str;
	flen = 0;

	while((fr = fread(p, 1, 1024, fp))){
		p += fr;
		flen += fr;
	}
	*p = '\0';
	result->len = flen;

	return result;
}

static AString *_a_string_append(AString *string, const char *value)
{
	int len = strlen(value);
	if (len + string->len >= string->allocated_len){
		a_string_realloc(string, len+string->len);
	}
	strcat(string->str, value);
	string->len += len;
	return string;
}

static AString *_a_string_append_c(AString *string, char c)
{
	int len = string->len + 1;
	if (len >= string->allocated_len){
		a_string_realloc(string, len);
	}
	string->str[string->len] = c;
	string->len++;
	string->str[string->len] = ZERO;

	return string;
}

static AString *_a_string_append_unichar(AString *string, aunichar wc)
{
	int len = string->len + 4, i;
	if (len >= string->allocated_len){
		a_string_realloc(string, len);
	}
	for (i = 0; i < 4; ++i){
		string->str[string->len] = wc & 255;
		string->len++;
		wc >> 8;
	}
	string->str[string->len] = ZERO;
	
	return string;
}

static AString *_a_string_append_len(AString *string, const char *value, asize len)
{
	int i;
	int slen = strlen(value);
	if (len > slen) len = slen;
	if (string->len + len >= string->allocated_len){
		a_string_realloc(string, string->len + len);
	}
	for (i = 0; i < len; ++i){
		string->str[string->len + i] = value[i];
	}
	string->str[string->len + i] = ZERO;
	string->len += i;
	
	return string;
}

static AString * _a_string_prepend(AString *string, const char *value)
{
	int len = strlen(value);
	/*  Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	/*  Copy data */
	if (string->len + len >= string->allocated_len) a_string_resize(string, len + string->len);
	strcpy(string->str, value);
	strcat(string->str, tempstr);
	free(tempstr);
	string->len += len;
	
	return string;
}

static AString * _a_string_prepend_c(AString *string, char c)
{
	int len = string->len + 1;
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	/*  Copy Data */
	if (len >= string->allocated_len) a_string_resize(string, len);
	string->str[0] = c; string->str[1] = ZERO;
	strcat(string->str, tempstr);
	free(tempstr);
	string->len = len;
	
	return string;
}

static AString * _a_string_prepend_unichar(AString *string, aunichar wc)
{
	int len = string->len + 4, i;
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);

	if (len >= string->allocated_len) a_string_resize(string, len);
	for (i = 0; i < 4; ++i){
		string->str[i] = wc & 255;
		wc >> 8;
	}
	string->str[4] = ZERO;
	strcat(string->str, tempstr);
	free(tempstr);
	string->len = len;
	
	return string;
}

static AString * _a_string_prepend_len(AString *string, const char *value, asize len)
{
	int slen = strlen(value), i;
   	if (slen < len) len = slen;
	
	/* Backup string data */
	char *tempstr = a_new(char, string->len + 1);
	strcpy(tempstr, string->str);
	
	/*  Copy Data */
	if (len + string->len >= string->allocated_len) a_string_resize(string, len + string->len);
	for (i = 0; i < len; ++i){
		string->str[i] = value[i];
	}
	string->str[i] = ZERO;
	strcat(string->str, tempstr);
	free(tempstr);
	string->len += len;
	
	return string;
}

static AString * _a_string_insert(AString *string, asize pos, const char *value)
{
	int len = strlen(value);
	
	if (pos > string->len) pos = string->len;
	if (pos < 0) pos = 0;

	char *tempstr = a_new(char, string->len - pos + 1);
	strcpy(tempstr, &(string->str[pos]));

	/*  Copy Data */
	if (len + string->len >= string->allocated_len) a_string_realloc(string, len + string->len);

	string->str[pos] = ZERO;
	strcat(string->str, value);
	strcat(string->str, tempstr);
	string->len += len;

	free(tempstr);
}

static AString * _a_string_insert_c(AString *string, asize pos, char c)
{
	int len = 1;
	
	if (pos > string->len) pos = string->len;
	if (pos < 0) pos = 0;

	char *tempstr = a_new(char, string->len - pos + 1);
	strcpy(tempstr, &(string->str[pos]));

	/*  Copy Data */
	if (len + string->len >= string->allocated_len) a_string_realloc(string, len + string->len);

	string->str[pos] = c;
	string->str[pos+1] = ZERO;
	strcat(string->str, tempstr);
	string->len += len;

	free(tempstr);
	
	return string;
}

static AString * _a_string_insert_unichar(AString *string, asize pos, aunichar wc)
{
	int len = 4, i;
	
	if (pos > string->len) pos = string->len;
	if (pos < 0) pos = 0;

	char *tempstr = a_new(char, string->len - pos + 1);
	strcpy(tempstr, &(string->str[pos]));

	/*  Copy Data */
	if (len + string->len >= string->allocated_len) a_string_realloc(string, len + string->len);

	for (i = 0; i < 4; ++i){
		string->str[pos + i] = wc & 255;
		wc >> 8;
	}
	string->str[pos+4] = ZERO;
	strcat(string->str, tempstr);
	string->len += len;

	free(tempstr);
	
	return string;
}

static AString * _a_string_insert_len(AString *string, asize pos, const char *value, asize len)
{
	int stlen = strlen(value), i;
	if (stlen < len) len = stlen;
	
	if (pos > string->len) pos = string->len;
	if (pos < 0) pos = 0;

	char *tempstr = a_new(char, string->len - pos + 1);
	strcpy(tempstr, &(string->str[pos]));

	/*  Copy Data */
	if (len + string->len >= string->allocated_len) a_string_realloc(string, len + string->len);

	string->str[pos] = ZERO;
	for (i = 0; i < len; ++i)
		string->str[i + pos] = value[i];
	string->str[i+pos] = '\0';
	strcat(string->str, tempstr);
	string->len += len;

	free(tempstr);
	
	return string;
}
	
static AString * _a_string_overwrite_len(AString *string, asize pos, const char *value, asize len)
{
	int stlen = strlen(value), i;

	if (stlen < len) len = stlen;
	if (len <= 0) {
		V(string->lock);
		return string;
	}

	if (pos > string->len) pos = string->len;
	if (pos < 0) pos = 0;

	if (len + pos >= string->allocated_len) a_string_realloc(string, len + pos);
	
	for (i = 0; i < len; ++i)
		string->str[i + pos] = value[i];

	if (pos + i >= string->len) string->str[i + pos] = ZERO;
	
	if (pos + i > string->len) string->len = pos + i;
	
	return string;
}

static AString * _a_string_erase(AString *string, asize pos, asize len)
{
	int i;
	if (pos >= string->len) {
		V(string->lock);
		return string;
	}
	if (pos < 0) pos = 0;

	if (len + pos > string->len) len = string->len - pos;

	/*  Copy Data */
	char *tempstr = a_new(char, string->len - pos - len + 1);
	strcpy(tempstr, &(string->str[pos+len]));

	string->str[pos] = '\0';
	strcat(string->str, tempstr);
	string->len -= len;

	free(tempstr);
	
	return string;
}
											  
static AString * _a_string_truncate (AString *string, asize len)
{
	if (len >= string->len)	{
		V(string->lock);
		return string;
	}
	if (len < 0) len = 0;

	string->str[len] = ZERO;
	string->len = len;
	
	return string;
}

static AString * _a_string_set_size(AString *string, asize len)
{
	if (len > string->len){
		a_string_realloc(string, len);
	}

	string->str[len] = ZERO;
	string->len = len;
	
	return string;
}

static char * _a_string_get_key(AString *string)
{
	int i = 0;
	while(i < string->len){
		if (string->str[i] == '=') break;
		i++;
	}
	if (i == string->len) return NULL;
	
	char *result = a_new(char, i + 1);
	strncpy(result, string->str, i);
	result[i] = '\0';
	
	return result;
}

static char * _a_string_get_value(AString *string)
{
	int i = 0;
	while(i < string->len){
		if (string->str[i] == '=') break;
		i++;
	}
	if (i == string->len) return NULL;

	char *result = a_new(char, string->len - i);
	strncpy(result, &(string->str[i+1]), string->len - i - 1);
	result[string->len - i - 1] = '\0';

	return result;
}

static AStringArray * _a_string_split(AString *string, const char *splitstr)
{
	char *str = _a_string_dupstr(string);
	char *fstr = str;
	asize len = string->len;
	asize slen = strlen(splitstr);
	asize count = 1, pos = 0;

	AStringArray *result = a_new(AStringArray, 1);
	
	char *p = strstr(str, splitstr);
	while(p){
		memset(p, 0, slen);
		p += slen;
		++count;
		p = strstr(p, splitstr);
	}
	
	result->size = count;
	result->data = a_new(AString *, count);
	AString *tmpstr = NULL;
	p = fstr;
	while(count --){
		tmpstr = a_string_new(p);
		result->data[pos++] = tmpstr;
		p += tmpstr->len + slen;
	}

	free(fstr);
	return result;
}

static AString * _a_string_trim(AString *string)
{
	char *endch = &(string->str[string->len]);
	char *startch = string->str;
	char *p = string->str, *q;
	while(endch != string->str && (*endch == ' ' || *endch == '\t' || *endch == ZERO)){
		--endch;
	}
	++endch;
	
	while(*startch && (*startch == ' ' || *startch == '\t')){
		++startch;
	}

	q = startch;

	while(q!=endch){
		*p++ = *q++;
	}

	string->len = ((int)endch - (int)startch) / (sizeof(char));
	string->str[string->len] = ZERO;
	
	return string;
}

static AString *_a_string_substring(AString *string, asize start, asize end)
{
	if (start >= string->len) return string;
	if (end < start) return string;
	if (end >= string->len) end = string->len - 1;

	char *p = string->str;
	char *q = &(string->str[start]);
	char *ends = &(string->str[end + 1]);
	
	while(q != ends){
		*p++ = *q++;
	}
	*p = '\0';

	string -> len = end - start + 1;

	return string;
}

static char _a_string_get_char(AString *string, asize position)
{
	if (position >= string->len) return ZERO;

	return (string->str[position]);
}

static AString *_a_string_set_char(AString *string, asize position, char ch)
{
	if (position >= string->len) return string;
	string->str[position] = ch;

	return string;
}

static int _a_string_find(AString *string, char *str, asize position)
{
	int result = -1;
	
	if (position > string->len) position = string->len;

	char *p = strstr(&(string->str[position]), str);
	if (p != NULL){
		result = ((int)p - (int)(string->str)) / (sizeof(char));
	}
	
	return result;
}

/* Returns: 0:replace OK, 1:replace Error */
static int _a_string_replace(AString *string, char *findstr, char *replacestr, asize position)
{
	int findlen = strlen(findstr);
	int replacelen = strlen(replacestr);
	
	if (position + findlen > string->len) return 0;	
	int pos = _a_string_find(string, findstr, position);
	
	if (pos != -1){
		_a_string_erase(string, pos, findlen);
		_a_string_insert(string, pos, replacestr);
		return 0;
	}else{
		return 1;
	}
}

static int _a_string_replace_all(AString *string, char *findstr, char *replacestr)
{
	asize pos = 0, pos2 = 0, count = 0;
	int replacelen = strlen(replacestr);
	while((pos2 = _a_string_find(string, findstr, pos)) != -1){
		_a_string_replace(string, findstr, replacestr, pos);
		pos = pos2 + replacelen;
		++ count;
	}
	return count;
}

/*  Using BKDRHash */
static auint _a_string_hash(AString *string)
{
	auint seed = 131;
	auint hash = 0;
	char *tempstr = string->str;

	while(*tempstr){
		hash = hash * seed + (*tempstr++);
	}
	
	return (hash & 0x7FFFFFFF);
}

static aboolean _a_string_equal(AString *string, AString *string2)
{
	aboolean result = FALSE;

	if (string->len != string2->len) result = FALSE;
	else{
		if (strcmp(string->str, string2->str) == 0) result = TRUE;
		else result = FALSE;
	}

	return result;
}

AString *a_string_new (const char *init)
{
	AString *res = a_string_new_empty(DEFAULT_SIZE);
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;	
	}
	
	/*Null String*/
	if (NULL == init) { 
		return res;
	}
	
	res->len = strlen (init);
	res->allocated_len = (res->len / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
	res->str = (char *) realloc (res->str, sizeof(char) * res->allocated_len);	
	
	/*Null String*/
	if (NULL == res->str) { 
		free (res);
		return NULL;
	}
	
	strcpy (res->str, init);
	return res;
}

AString *a_string_new_len (const char *init, asize len)
{
	AString *res = a_string_new_empty(DEFAULT_SIZE);
	
	/* Malloc Failed */
	if (NULL == res) {
		return NULL;	
	}
	
	/*Null String*/
	if (NULL == init) { 
		return res;
	}
	asize tLen = strlen (init);
	if (tLen < len) len = tLen;

	res->len = len;
	res->allocated_len = (res->len / DEFAULT_SIZE + 1) * DEFAULT_SIZE + 1;
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
	while (i < res->allocated_len) res->str[i++] = ZERO;
	return res;
}

AString *a_string_sized_new (asize size) 
{
	return a_string_new_empty(size);
}

AString *a_string_assign(AString *string, const char * value)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_assign(string, value);

	V(string->lock);
	return string;
}

AString *a_string_get_file_content (const char * filename)
{
	return _a_string_get_file_content(filename);
}

AString *a_string_dup(AString *source)
{
	if (!A_IS_STRING(source)) {
		A_WARING_NOT_STRING;
		return NULL;
	}
	P(source->lock);

	AString *dest = _a_string_dup(source);

	V(source->lock);
	return dest;
}

char *a_string_dupstr(AString *source)
{
	if (!A_IS_STRING(source)) {
		A_WARING_NOT_STRING;
		return NULL;
	}
	P(source->lock);

	char *result = _a_string_dupstr(source);	
	
	V(source->lock);
	return result;	
}

AString *a_string_append(AString *string, const char *value)
{ 
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_append(string, value);

	V(string->lock);
	return string;
}

AString *a_string_append_c(AString *string, char c)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_append_c(string, c);

	V(string->lock);
	return string;
}

AString *a_string_append_unichar(AString *string, aunichar wc)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_append_unichar(string, wc);

	V(string->lock);
	return string;
}

AString *a_string_append_len(AString *string, const char *value, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_append_len(string, value, len);

	V(string->lock);
	return string;
}

AString * a_string_prepend(AString *string, const char *value)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_prepend(string, value);

	V(string->lock);
	return string;
}

AString * a_string_prepend_c(AString *string, char c)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_prepend_c(string, c);

	V(string->lock);
	return string;
}

AString * a_string_prepend_unichar(AString *string, aunichar wc)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_prepend_unichar(string, wc);

	V(string->lock);
	return string;
}

AString * a_string_prepend_len(AString *string, const char *value, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_prepend_len(string, value, len);

	V(string->lock);
	return string;
}

AString * a_string_insert(AString *string, asize pos, const char *value)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_insert(string, pos, value);

	V(string->lock);
	return string;
}

AString * a_string_insert_c(AString *string, asize pos, char c)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_insert_c(string, pos, c);

	V(string->lock);
	return string;
}

AString * a_string_insert_unichar(AString *string, asize pos, aunichar wc)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_insert_unichar(string, pos, wc);

	V(string->lock);
	return string;
}

AString * a_string_insert_len(AString *string, asize pos, const char *value, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_insert_len(string, pos, value, len);

	V(string->lock);
	return string;
}

char * a_string_get_key(AString *string)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return NULL;
	}
	P(string->lock);

	char *result = _a_string_get_key(string);

	V(string->lock);
	return result;
}

char * a_string_get_value(AString *string)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return NULL;
	}
	P(string->lock);

	char *result = _a_string_get_value(string);

	V(string->lock);
	return result;
}

AString * a_string_overwrite(AString *string, asize pos, const char *value)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);
	
	int len = strlen(value);
	_a_string_overwrite_len(string, pos, value, len);
	
	V(string->lock);
	return string;
}

AString * a_string_overwrite_len(AString *string, asize pos, const char *value, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_overwrite_len(string, pos, value, len);

	V(string->lock);
	return string;
}

AString * a_string_erase(AString *string, asize pos, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_erase(string, pos, len);

	V(string->lock);
	return string;
}

AString * a_string_truncate (AString *string, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_truncate(string, len);

	V(string->lock);
	return string;
}

AString * a_string_set_size(AString *string, asize len)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);
	
	_a_string_set_size(string, len);

	V(string->lock);
	return string;
}

AStringArray * a_string_split(AString *string, const char *splitstr)
{
	if (!A_IS_STRING(string)){
		A_WARING_NOT_STRING;
		return NULL;
	}

	P(string->lock);

	AStringArray *result = _a_string_split(string, splitstr);

	V(string->lock);

	return result;
}

AString * a_string_trim(AString *string)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_trim(string);

	V(string->lock);
	return string;
}

AString * a_string_substring(AString *string, asize start, asize end)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_substring(string, start, end);

	V(string->lock);
	return string;
}
 
AString * a_string_substring_new(AString *string, asize start, asize end)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);
	
	AString *result = _a_string_dup(string);
	_a_string_substring(result, start, end);

	V(string->lock);
	return result;
}

char a_string_get_char(AString *string, asize position)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return ZERO;
	}
	P(string->lock);

	char result = _a_string_get_char(string, position);

	V(string->lock);
	return result;
}

AString *a_string_set_char(AString *string, asize position, char ch)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return string;
	}
	P(string->lock);

	_a_string_set_char(string, position, ch);

	V(string->lock);
	return string;
}

int a_string_find(AString *string, char *str, asize position)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return -1;
	}
	P(string->lock);
	
	int result = _a_string_find(string, str, position);
	
	V(string->lock);
	return result;
}

int a_string_replace(AString *string, char *findstr, char *replacestr, asize position)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return -1;
	}
	P(string->lock);

	int result = _a_string_replace(string, findstr, replacestr, position);

	V(string->lock);
	return result;
}


int a_string_replace_all(AString *string, char *findstr, char *replacestr)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return -1;
	}
	P(string->lock);

	int result = _a_string_replace_all(string, findstr, replacestr);

	V(string->lock);
	return result;
}

auint a_string_hash(AString *string)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return 0;
	}
	P(string->lock);

	auint hash = _a_string_hash(string);

	V(string->lock);
	return hash;
}

aboolean a_string_equal(AString *string, AString *string2)
{
	if (!A_IS_STRING(string)) {
		A_WARING_NOT_STRING;
		return FALSE;
	}
	if (!A_IS_STRING(string2)) {
		A_WARING_NOT_STRING;
		return FALSE;
	}
	P(string->lock);
	
	aboolean result = _a_string_equal(string, string2);

	V(string->lock);
	return result;
}


void a_string_free (AString *string)
{
	if (!A_IS_STRING(string)) return ;
	string->flag = 0;
	free(string->str);
	free(string);
}

