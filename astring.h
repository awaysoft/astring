#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <stdlib.h>
#include <string.h>
#include "atype.h"

#define A_STRING_FLAG       101
#define DEFAULT_SIZE        100
#define ZERO                '\0'
#define TRUE                1
#define FALSE               0

#define A_IS_STRING(string) (*(auchar *)string == A_STRING_FLAG)

typedef struct _AString{
	auchar              flag; /* Check is a_string */
	volatile aboolean   lock;

	char *              str;
	asize               len;
	asize               allocated_len;
}AString;

typedef struct _AStringArray{
	AString **          data;
	asize               size;
}AStringArray;

/* Alloc a new AString */
AString *      a_string_new                   (const char *init);
AString *      a_string_new_len               (const char *init, 
						                      asize len);
AString *      a_string_sized_new             (asize size);
AString *      a_string_assign                (AString *string,
						                      const char * value);
AString *      a_string_dup                   (AString *source);
char    *      a_string_dupstr                (AString *source);

/* Get String From a file */
AString *      a_string_get_file_content			 (const char * filename);

/* Append Something */
AString *      a_string_append                (AString *string,
						                      const char *value);
AString *      a_string_append_c              (AString *string,
						                      char c);
AString *      a_string_append_unichar        (AString *string,
					                          aunichar wc);
AString *      a_string_append_len            (AString *string,
					                          const char *value,
					                          asize len);

/* Prepend Something */
AString *      a_string_prepend               (AString *string,
						                      const char *value);
AString *      a_string_prepend_c             (AString *string,
					                          char c);
AString *      a_string_prepend_unichar       (AString *string,
						                      aunichar wc);
AString *      a_string_prepend_len           (AString *string,
						                      const char *value,
						                      asize len);						

/* Insert Something */
AString *      a_string_insert                (AString *string,
						                      asize pos,
					                          const char *value);
AString *      a_string_insert_c              (AString *string,
						                      asize pos,
						                      char c);
AString *      a_string_insert_unichar        (AString *string,
						                      asize pos,
						                      aunichar wc);
AString *      a_string_insert_len            (AString *string,
						                      asize pos,
						                      const char *value,
						                      asize len);	
char *         a_string_get_key               (AString *string);
char *         a_string_get_value             (AString *string);
						
/* Overwrite Something */
AString *      a_string_overwrite             (AString *string,
                                              asize pos,
                                              const char *value);
AString *      a_string_overwrite_len         (AString *string,
                                              asize pos,
                                              const char *value,
                                              asize len);

/* Erase Something */
AString *      a_string_erase                 (AString *string,
											  asize pos,
											  asize len);
											  

/* Truncate Something 
 * Leave the first len char 
 * */
AString *      a_string_truncate              (AString *string,
											  asize len);
											  
											  
/* Set Size */
AString *      a_string_set_size              (AString *string,
											  asize len);

AString *      a_string_trim                  (AString *string);

AStringArray * a_string_split                 (AString *string,
											  const char *splitstr);

/* Get SubString */
AString *	   a_string_substring			(AString *string, asize start, asize end);

AString *	   a_string_substring_new	    (AString *string, asize start, asize end);

/* Get Chat */
char		   a_string_get_char				(AString *string, asize position);

AString *	   a_string_set_char				(AString *string, asize position, char ch);

/* Find and Replace */
int 	       a_string_find              	(AString *string,
											char *str,
											asize position);

int		 	  a_string_replace				(AString *string,
											char *findstr,
											char *replacestr,
											asize position);

int 		  a_string_replace_all			(AString *string,
											char *findstr,
											char *replacestr);


/* Free this string */
void           a_string_free                  (AString *string);

/* calc a hash code */
auint          a_string_hash                  (AString *string);

/* return if two string is equal */
aboolean       a_string_equal                 (AString *string,
                                              AString *string2);

#endif
