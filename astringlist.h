#ifndef __A_STRING_LIST_H__
#define __A_STRING_LIST_H__


#include "astring.h"

#define A_STRING_LIST_FLAG       102

#define A_IS_STRING_LIST(string) (*(auchar *)string == A_STRING_LIST_FLAG)


struct _AStringList {
	auchar             flag;
	volatile aboolean  lock;

	asize              length;
	asize              allocated_len;

	AString **     data;
};

typedef int (*A_CMP)(const void *a, const void *b);
typedef struct _AStringList AStringList;
typedef int (*AStringCMP) (const AString *a, const AString *b);

AStringList *   a_string_list_new          (void);

AStringList *   a_string_list_assign       (AStringList *list,
                                            AString *string);

AStringList *   a_string_list_load_from_file(const char * filename);

AStringList *   a_string_list_append       (AStringList *list,
                                            AStringList *list2);

AStringList *   a_string_list_prepend      (AStringList *list,
                                            AStringList *list2);

AStringList *   a_string_list_add          (AStringList *list, 
                                            AString *string);

AStringList *   a_string_list_insert       (AStringList *list,
                                            asize index,
											AString *string);

AStringList *   a_string_list_delete       (AStringList *list, 
                                            asize index);

AStringList *   a_string_list_clear        (AStringList *list);

AStringList *   a_string_list_exchange     (AStringList *list,
                                            asize index1,
											asize index2);

asize           a_string_list_find         (AStringList *list,
                                            AString *string,
											asize index);

asize			a_string_list_find_name    (AStringList *list,
                                            AString *string,
											asize index);

AString *       a_string_list_get_value    (AStringList *list,
                                            asize index);

AStringList *   a_string_list_sort         (AStringList *list);

AStringList *   a_string_list_sort_custom  (AStringList *list,
											A_CMP cmp);

aboolean        a_string_list_sorted       (AStringList *list);

AStringList *   a_string_list_dup          (AStringList *list);

/* returnchar:0:\n,1:\r,2:\r\n */
AString *       a_string_list_get_text     (AStringList *list, auchar returnchar);

aboolean        a_string_list_save_to_file (AStringList *list,
                                            const char * filename,
                                            auchar returnchar);

aboolean        a_string_list_equal       (AStringList *list1,
                                            AStringList *list2);

void            a_string_list_free         (AStringList *list);

#endif
