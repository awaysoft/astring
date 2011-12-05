#include "astring.h"
#include "astringlist.h"
#include <stdio.h>
#include <string.h>


#define a_new(type, size) (type *)malloc(sizeof(type) * (size))
#define a_free(point) do{free(point);point = NULL;}while(0)

/* Get String's Lock and Lock
 * String */
#define P(lock) while(lock++){--lock;}

/* Unlock String */
#define V(lock) --lock;

#define A_WARING(msg) do{fprintf(stderr, "[WARING] %s: [%s():%d] %s\n", __FILE__, __func__, __LINE__, msg);}while(0)

#define A_WARNING_NOT_STRING_LIST A_WARING("A_IS_STRING_LIST Check falt!")
#define A_WARNING_NOT_STRING      A_WARING("A_IS_STRING Check falt!")


AStringList * a_string_list_new(void)
{
	AStringList *res = NULL;

	res = a_new(AStringList, 1);

	if (res == NULL) return res;

	res->allocated_len = DEFAULT_SIZE;
	res->data = a_new(AString *, res->allocated_len);
	res->length = 0;
	memset(res->data, 0, sizeof(AString *) * res->allocated_len);

	if (res->data == NULL){
		a_free(res);
		return NULL;
	}

	res->flag = A_STRING_LIST_FLAG;
	res->lock = FALSE;

	return res;
}

static AStringList * _a_string_list_clear (AStringList *list)
{
	int i = list->length - 1;
	while(i>0){
		a_string_free(list->data[i]);
		list->data[i] = NULL;
		i--;
	}
	list->length = 0;

	return list;
}

static AStringList * _a_string_list_resize (AStringList *list, asize size)
{
	if (list->length > size){
		int i = size;
		while(i < list->length){
			a_string_free(list->data[i]);
			i++;
		}
		list->length = size;
	}
	list->allocated_len = size;
	list->data = (AString **)realloc(list->data, sizeof(AString *) * list->allocated_len);

	return list;
}

static AStringList * __a_string_list_insert(AStringList *list, asize index, AString *string)
{
	AString *str = a_string_dup(string);
	if (index > list->length) index = list->length;
	if (list->length >= list->allocated_len){
		_a_string_list_resize(list, list->allocated_len + DEFAULT_SIZE);
	}
	if (index == list->length) list->data[index] = str;
	else{
		int i = list->length;
		while(i && i != index){
			list->data[i] = list->data[i-1];
			i--;
		}
		list->data[i] = str;
	}
	list->length ++;

	return list;
}

static AStringList * __a_string_list_add(AStringList *list, AString *string)
{
	if (list->length >= list->allocated_len){
		_a_string_list_resize(list, list->allocated_len + DEFAULT_SIZE);
	}
	list->data[list->length] = a_string_dup(string);
	list->length++;

	return list;
}

static AStringList * _a_string_list_assign (AStringList *list, AString *string)
{
	_a_string_list_clear(list);

	char *p = string->str, *q, *start;
	int len = 0, i;
	AString *str = NULL;
	while(*p){
		len = 0;
		start = p;
		/* find \r\n */
		while(*p && *p!='\n' && *p!='\r'){
			len ++;
			p++;
		}
		if (*p == '\r' && *(p+1) == '\n') p++;

		/* Copy Data */
		str = a_string_sized_new(len + 2);
		i = len;
		q = str->str;
		while(i--){
			*q++ = *start++;
		}
		*q = '\0';
		str->len = len;

		__a_string_list_add(list, str);
		a_string_free(str);

		if (*p) p++;
	}

	return list;
}

static aboolean _a_string_split_cmp(AString *split, char *str)
{
    int i = 0;
    while(i < split->len && *str){
        if (split->str[i] != *str) return FALSE;
        i++; str++;
    }
    if (i == split->len) return TRUE;
    else return FALSE;
}

static AStringList * _a_string_list_assign_split (AStringList *list, AString *string, AString *split)
{
    _a_string_list_clear(list);

	char *p = string->str, *q, *start;
	int len = 0, i;
	AString *str = NULL;
	while(*p){
		len = 0;
		start = p;
		/* find \r\n */
		while(*p && *p!='\n' && *p!='\r' && !_a_string_split_cmp(split, p)){
			len ++;
			p++;
		}
		if (*p == '\r' && *(p+1) == '\n') p++;

		/* Copy Data */
		str = a_string_sized_new(len + 2);
		i = len;
		q = str->str;
		while(i--){
			*q++ = *start++;
		}
		*q = '\0';
		str->len = len;

		__a_string_list_add(list, str);
		a_string_free(str);

		if (*p && (*p == '\n'||*p == '\r')) p++;
		else p += split->len;
	}

    return list;
}

static AStringList * _a_string_list_load_from_file(const char *filename)
{
	AString *string = a_string_get_file_content(filename);
	AStringList *list = a_string_list_new();
	_a_string_list_assign(list, string);

	a_string_free(string);

	return list;
}

static AStringList * _a_string_list_append(AStringList *list, AStringList *list2)
{
	asize len = list->length + list2->length, i = 0;
	if (len > list->allocated_len){
		_a_string_list_resize(list, len);
	}
	while(i < list2->length){
		list->data[i+list->length] = a_string_dup(list2->data[i]);
		i++;
	}
	list->length = len;

	return list;
}

static AStringList * _a_string_list_prepend(AStringList *list, AStringList *list2)
{
	asize len = list->length + list2->length, i = list->length;
	if (len > list->allocated_len){
		_a_string_list_resize(list, len);
	}
	while(i--){
		list->data[i+list2->length] = list->data[i];
	}
	i = list2->length;
	while(i--){
		list->data[i] = a_string_dup(list2->data[i]);
	}
	list->length = len;

	return list;
}

static AStringList * _a_string_list_add(AStringList *list, AString *string)
{
	AStringList *tmplist = a_string_list_new();
	a_string_list_assign(tmplist, string);
	
	_a_string_list_append(list, tmplist);
	
	a_string_list_free(tmplist);	

	return list;
}

static AStringList * _a_string_list_insert(AStringList *list, asize index, AString *string)
{
	AStringList *tmplist = a_string_list_new();
	int i;
	a_string_list_assign(tmplist, string);
	
	if (index > list->length) index = list->length;
	while (list->length + tmplist->length >= list->allocated_len){
		_a_string_list_resize(list, list->allocated_len + DEFAULT_SIZE);
	}
	
	for (i = 0; i < tmplist->length; ++i){
		__a_string_list_insert(list, index + i, a_string_dup(tmplist->data[i]));
	}
	
	a_string_list_free(tmplist);

	return list;
}

static AStringList * _a_string_list_delete(AStringList *list, asize index)
{
	if (index >= list->length) return list;

	a_string_free(list->data[index]);

	int i = index;
	while(i < list->length - 1){
		list->data[i] = list->data[i+1];
		i++;
	}

	list->length --;

	return list;
}

static AStringList * _a_string_list_exchange(AStringList *list, asize index1, asize index2)
{
	if (index1 >= list->length || index2 >= list->length) return list;
	AString *str = list->data[index1];
	list->data[index1] = list->data[index2];
	list->data[index2] = str;

	return list;
}

static int _a_string_list_find(AStringList *list, AString *string, asize index)
{
	if (index >= list->length) return -1;
	while(index < list->length){
		if (a_string_equal(string, list->data[index])) return index;
		index ++;
	}
	return -1;
}

static int _a_string_list_find_name(AStringList *list, AString *string, asize index)
{
	if (index >= list->length) return -1;
	while(index < list->length){
		char *key = a_string_get_key(list->data[index]);
		if (strcmp(key, string->str) == 0){
			free(key);
			return index;
		}
		free(key);
		index++;
	}
	return -1;
}

static AString * _a_string_list_get_value(AStringList *list, asize index)
{
	if (index >= list->length) return NULL;
	char *tmpstr = a_string_get_value(list->data[index]);
	AString *result = a_string_new(tmpstr);
	free(tmpstr);

	return result;
}

static AString * _a_string_list_get(AStringList *list, asize index)
{
	if (index >= list->length) return NULL;

	return list->data[index];
}

static int a_string_cmp2(AString *a, AString *b)
{
	return strcmp(a->str, b->str);
}

static int a_string_cmp(const void *a, const void *b)
{
	return a_string_cmp2(*(AString **)a, *(AString **)b);
}

static AStringList * _a_string_list_sort(AStringList *list)
{
	qsort(list->data, list->length, sizeof(AString *), a_string_cmp);
	return list;
}

static AStringList * _a_string_list_sort_custom(AStringList *list, A_CMP cmp)
{
	qsort(list->data, list->length, sizeof(AString *), cmp);
	return list;
}

static aboolean _a_string_list_sorted(AStringList *list)
{
	int i = 0;
	while(i < list->length - 2){
		if (a_string_cmp2(list->data[i], list->data[i+1]) != a_string_cmp2(list->data[i+1], list->data[i+2]))
			return FALSE;
		i++;
	}
	return TRUE;
}

static aboolean _a_string_list_sorted_custom(AStringList *list, A_CMP cmp)
{
	int i = 0;
	while(i < list->length - 2){
		if (cmp(&(list->data[i]), &(list->data[i+1])) != cmp(&(list->data[i+1]), &(list->data[i+2])))
			return FALSE;
		i++;
	}
	return TRUE;
}

static AStringList * _a_string_list_dup(AStringList *list)
{
	AStringList *result = a_string_list_new();
	int i = 0;

	_a_string_list_resize(result, list->allocated_len);
	result->length = list->length;
	while(i < result->length){
		result->data[i] = a_string_dup(list->data[i]);
		i++;
	}

	return result;
}

static AString * _a_string_list_get_text(AStringList *list, auchar returnchar)
{
	asize len = 0, i = 0;
	while(i < list->length){
		len += list->data[i]->len;
		++len;
		if (returnchar == 2) ++len;
		i++;
	}
	AString *result = a_string_sized_new(len + 1);
	i = 0;
	while(i < list->length){
		a_string_append(result, list->data[i]->str);
		switch(returnchar){
			case 0:a_string_append_c(result, '\n');
				   break;
			case 1:a_string_append_c(result, '\r');
				   break;
			case 2:a_string_append(result, "\r\n");
				   break;
			default:
				   break;
		}
		i++;
	}

	return result;
}

static aboolean _a_string_list_save_to_file(AStringList *list, const char *filename, auchar returnchar)
{
	FILE *fp;
	size_t fr;

	if ((fp = fopen(filename, "wb")) == NULL) return FALSE;
	AString *string = _a_string_list_get_text(list, returnchar);
	char *p = string->str;
	asize len = string->len;
	int wsize = 1024;

	fseek(fp, 0L, 0);
	while(len > 0){
		wsize = len >= 1024?1024:len;
		fwrite(p, 1, wsize, fp);
		p += wsize;
		len -= wsize;
	}

	fclose(fp);
	a_string_free(string);

	return TRUE;
}

static aboolean _a_string_list_equal(AStringList *list1, AStringList *list2)
{
	if (list1->length != list2->length) return FALSE;

	asize i = 0;
	while(i < list1->length){
		if (!a_string_equal(list1->data[i], list2->data[i])) return FALSE;
		i++;
	}
	return TRUE;
}

static void _a_string_list_free(AStringList *list)
{
	asize i = 0;
	list->flag = 0;
	while(i < list->length){
		a_string_free(list->data[i]);
		i++;
	}
	a_free(list->data);
	free(list);
}







AStringList * a_string_list_assign (AStringList *list, AString *string)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return list;
	}

	P(list->lock);

	_a_string_list_assign(list, string);

	V(list->lock);

	return list;
}

AStringList * a_string_list_assign_split (AStringList *list, AString *string, AString *split)
{
    if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}
	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return list;
	}
	if (!A_IS_STRING(split)){
		A_WARNING_NOT_STRING;
		return list;
	}

	P(list->lock);

    _a_string_list_assign_split(list, string, split);

	V(list->lock);
}


AStringList * a_string_list_load_from_file(const char * filename)
{
	return _a_string_list_load_from_file(filename);
}

AStringList * a_string_list_append (AStringList *list, AStringList *list2)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}
	if (!A_IS_STRING_LIST(list2)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);
	P(list2->lock);

	_a_string_list_append(list, list2);

	V(list2->lock);
	V(list->lock);

	return list;

}

AStringList * a_string_list_prepend (AStringList *list, AStringList *list2)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}
	if (!A_IS_STRING_LIST(list2)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);
	P(list2->lock);

	_a_string_list_prepend(list, list2);

	V(list->lock);
	V(list2->lock);

	return list;
}

AStringList * a_string_list_add (AStringList *list, AString *string)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}
	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return list;
	}

	P(list->lock);

	_a_string_list_add(list, string);

	V(list->lock);

	return list;
}

AStringList * a_string_list_insert (AStringList *list, asize index, AString *string)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}
	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return list;
	}

	P(list->lock);

	_a_string_list_insert(list, index, string);

	V(list->lock);

	return list;
}

AStringList * a_string_list_delete (AStringList *list, asize index)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	_a_string_list_delete(list, index);

	V(list->lock);

	return list;
}

AStringList * a_string_list_clear (AStringList *list)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	_a_string_list_clear(list);

	V(list->lock);

	return list;
}

AStringList * a_string_list_exchange (AStringList *list, asize index1, asize index2)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	_a_string_list_exchange(list, index1, index2);

	V(list->lock);

	return list;
}

asize a_string_list_find (AStringList *list, AString *string, asize index)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return -1;
	}
	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return -1;
	}

	P(list->lock);

	asize result = _a_string_list_find(list, string, index);

	V(list->lock);

	return result;
}

asize a_string_list_find_name (AStringList *list, AString *string, asize index)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return -1;
	}
	if (!A_IS_STRING(string)){
		A_WARNING_NOT_STRING;
		return -1;
	}

	P(list->lock);

	asize result = _a_string_list_find_name(list, string, index);

	V(list->lock);

	return result;
}

AString * a_string_list_get_value (AStringList *list, asize index)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return NULL;
	}

	P(list->lock);

	AString *result = _a_string_list_get_value(list, index);

	V(list->lock);

	return result;
}

AString * a_string_list_get_index (AStringList *list, asize index)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return NULL;
	}

	P(list->lock);

	AString *result = _a_string_list_get(list, index);

	V(list->lock);

	return result;
}

AStringList * a_string_list_sort (AStringList *list)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	_a_string_list_sort(list);

	V(list->lock);

	return list;
}

AStringList * a_string_list_sort_custom (AStringList *list, A_CMP cmp)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	_a_string_list_sort_custom(list, cmp);

	V(list->lock);

	return list;
}

aboolean a_string_list_sorted (AStringList *list)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return FALSE;
	}

	P(list->lock);

	aboolean result = _a_string_list_sorted(list);

	V(list->lock);

	return result;
}

aboolean a_string_list_sorted_custom (AStringList *list, A_CMP cmp)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return FALSE;
	}

	P(list->lock);

	aboolean result = _a_string_list_sorted_custom(list, cmp);

	V(list->lock);

	return result;
}

AStringList * a_string_list_dup (AStringList *list)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return list;
	}

	P(list->lock);

	AStringList *result = _a_string_list_dup(list);

	V(list->lock);

	return result;
}

/* returnchar:0:\n,1:\r,2:\r\n */
AString * a_string_list_get_text (AStringList *list, auchar returnchar)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return NULL;
	}

	P(list->lock);

	AString *result = _a_string_list_get_text(list, returnchar);

	V(list->lock);

	return result;
}

aboolean a_string_list_save_to_file (AStringList *list, const char * filename, auchar returnchar)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return FALSE;
	}

	P(list->lock);

	aboolean result = _a_string_list_save_to_file(list, filename, returnchar);

	V(list->lock);

	return result;
}

aboolean a_string_list_equal (AStringList *list1, AStringList *list2)
{
	if (!A_IS_STRING_LIST(list1)){
		A_WARNING_NOT_STRING_LIST;
		return FALSE;
	}
	if (!A_IS_STRING_LIST(list2)){
		A_WARNING_NOT_STRING_LIST;
		return FALSE;
	}

	P(list1->lock);
	P(list2->lock);

	aboolean result = _a_string_list_equal(list1,list2);

	V(list1->lock);
	V(list2->lock);

	return result;
}

void a_string_list_free (AStringList *list)
{
	if (!A_IS_STRING_LIST(list)){
		A_WARNING_NOT_STRING_LIST;
		return ;
	}

	_a_string_list_free(list);
}

