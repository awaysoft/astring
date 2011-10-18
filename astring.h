#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 100

typedef unsigned long asize;
typedef unsigned int aunichar;
typedef unsigned char aboolean;
typedef unsigned int auint;

typedef struct{
	char *str;
	asize len;
	asize allocated_len;
}AString;

/* Alloc a new AString */
AString *      astring_new                   (const char *init);
AString *      astring_new_len               (const char *init, 
						                      asize len);
AString *      astring_sized_new             (asize size);
AString *      astring_assign                (AString *string,
						                      const char * value);
AString *      astring_dump                  (AString *source);
char    *      astring_dumpstr               (AString *source);

/* Append Something */
AString *      astring_append                (AString *string,
						                      const char *value);
AString *      astring_append_c              (AString *string,
						                      char c);
AString *      astring_append_unichar        (AString *string,
					                          aunichar wc);
AString *      astring_append_len            (AString *string,
					                          const char *value,
					                          asize len);

/* Prepend Something */
AString *      astring_prepend               (AString *string,
						                      const char *value);
AString *      astring_prepend_c             (AString *string,
					                          char c);
AString *      astring_prepend_unichar       (AString *string,
						                      aunichar wc);
AString *      astring_prepend_len           (AString *string,
						                      const char *value,
						                      asize len);						

/* Insert Something */
AString *      astring_insert                (AString *string,
						                      asize pos,
					                          const char *value);
AString *      astring_insert_c              (AString *string,
						                      asize pos,
						                      char c);
AString *      astring_insert_unichar        (AString *string,
						                      asize pos,
						                      aunichar wc);
AString *      astring_insert_len            (AString *string,
						                      asize pos,
						                      const char *value,
						                      asize len);	
						
/* Overwrite Something */
AString *      astring_overwrite             (AString *string,
                                              asize pos,
                                              const char *value);
AString *      astring_overwrite_len         (AString *string,
                                              asize pos,
                                              const char *value,
                                              asize len);

/* Erase Something */
AString *      astring_erase                 (AString *string,
											  asize pos,
											  asize len);
											  

/* Truncate Something */
AString *      astring_truncate              (AString *string,
											  asize len);
											  
											  
/* Set Size */
AString *      astring_set_size              (AString *string,
											  asize len);

AString *      astring_trim                  (AString *string);

/* Free this string */
void           astring_free                  (AString *string);

/* calc a hash code */
auint          astring_hash                  (AString *string);

/* return if two string is equal */
aboolean       astring_equal                 (AString *string,
                                              AString *string2);

#endif
