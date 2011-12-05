/**
 * @file astring.h
 * @brief AString 
 * @author Tom
 * @date 2011-11-25
 * @version 0.2
 * @note
 * A AString is an object that handles the memory management of a C string for you. You can think of it as similar to a Java StringBuffer. In addition to the string itself, GString stores the length of the string, so can be used for binary data with embedded nul bytes. To access the C string managed by the AString string, simply use string->str.
 */
#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <stdlib.h>
#include <string.h>
#include "atype.h"

#define A_STRING_FLAG       101
#define ZERO                '\0'

/**
 * @brief A define.
 * Check if a variable is an AString
 */ 
#define A_IS_STRING(string) (*(auchar *)string == A_STRING_FLAG)

/**
 * @brief The Main Struct
 *
 * The AString struct contains the public fields of a AString.
 */ 
struct _AString{
	auchar              flag; /**< Don't Change it, it used to check a object is AString. */
	volatile aboolean   lock; /**< Don't Change it, it used to keep thread safe. */

	asize               len;  /**< contains the length of the string, not including the terminating nul byte. */
	asize               allocated_len;   /**< contains the length of the string. */
	
	char *              str;  /**< points to the character data. */
};

/**
 * @brief Main Struct
 */
typedef struct _AString AString;

/**
 * @brief AStringArray Struct.
 * 
 * The AStringArray struct contains the public fields of a AStringArray.It only used in a_string_split(). 
 */ 
struct _AStringArray{
	AString **          data; /**< points to the AString data. */
	asize               size; /**< contains the length of the stringarray */
};

/**
 * @brief the array of some AString
 */
typedef struct _AStringArray AStringArray;

/**
 * @brief Create a new AString, initialized with the given string.
 * @param init the initial text to copy into the string.
 * @return a new AString
 * @since 0.1
 */ 
AString *      a_string_new                   (const char *init);

/**
 * @brief Create a new AString with len length.
 *  if strlen(init) is larger than len, the length of new string will be len, otherwise the length will be strlen(init)
 * @param init the initial text to copy into the string.
 * @param len the max length
 * @return a new AString
 * @since 0.1
 */ 
AString *      a_string_new_len               (const char *init, 
                                              asize len);

/**
 * @brief Create a new AString with size allocated_len
 *  This is useful if you are going to add a lot of text to the string and don't want it to be reallocated too often.
 * @param size the default size of the space allocated to hold the string
 * @return a new AString
 * @since 0.1
 */
AString *      a_string_sized_new             (asize size);
/**
 * @brief Copies the bytes from a string into an AString, destroying any previous contents. It is rather like the standard strcpy() function, except that you do not have to worry about having enough space to copy the string.
 * @param string the destination AString. Its current contents are destroyed.
 * @param value the string to copy into string
 * @return string
 * @since 0.1
 */
AString *      a_string_assign                (AString *string,
                                              const char * value);
/**
 * @brief duplicate an AString.
 * @warning You should free the return string when you don't want to use it.
 * @param source the source AString.
 * @return the new AString
 * @since 0.1
 */
AString *      a_string_dup                   (AString *source);
/**
 * @brief duplicate char * from an AString.
 * @warning You should free the return string when you don't want to use it.
 *  When you just want the string to print or view, not change it, you can use source->str instead.

 * @param the source AString.
 * @return the new AString
 * @since 0.1
 */
char    *      a_string_dupstr                (AString *source);

/**
 * @brief Create a new AString from a file.
 * @warning You should free the return string when you don't want to use it.
 * @param filename the file you need to read.
 * @return the new AString
 * @since 0.1
 */
AString *      a_string_get_file_content     (const char * filename);
/**
 * @brief Append a string to An AString.
 * @param string the destination AString
 * @param value the string you want to append
 * @return string
 * @since 0.1
 */
AString *      a_string_append                (AString *string,
                                              const char *value);
/**
 * @brief Append a char to An AString.
 * @param string the destination AString
 * @param c the char you want to append
 * @return string
 * @since 0.1
 */
AString *      a_string_append_c              (AString *string,
                                              char c);
/**
 * @brief Append a char to An AString.
 * @param string the destination AString
 * @param wc the wide char you want to append
 * @return string
 * @since 0.1
 */
AString *      a_string_append_unichar        (AString *string,
                                              aunichar wc);
/**
 * @brief Append a char to An AString.
 * @param string the destination AString
 * @param value the string you want to append
 * @param len the max char length to append
 * @return string
 * @since 0.1
 */
AString *      a_string_append_len            (AString *string,
                                              const char *value,
                                              asize len);
/**
 * @brief Prepend a string to An AString.
 * @param string the destination AString
 * @param value the string you want to prepend
 * @return string
 * @since 0.1
 */
AString *      a_string_prepend               (AString *string,
                                              const char *value);
/**
 * @brief Prepend a char to An AString.
 * @param string the destination AString
 * @param c the char you want to prepend
 * @return string
 * @since 0.1
 */
AString *      a_string_prepend_c             (AString *string,
                                              char c);
/**
 * @brief Prepend a char to An AString.
 * @param string the destination AString
 * @param wc the wide char you want to prepend
 * @return string
 * @since 0.1
 */
AString *      a_string_prepend_unichar       (AString *string,
                                              aunichar wc);
/**
 * @brief Prepend a char to An AString.
 * @param string the destination AString
 * @param value the string you want to prepend
 * @param len the max char length to prepend
 * @return string
 * @since 0.1
 */
AString *      a_string_prepend_len           (AString *string,
                                              const char *value,
                                              asize len);
/**
 * @brief Insert a string to An AString.
 * @param string the destination AString
 * @param pos the position where do you want to insert
 * @param value the string you want to insert
 * @return string
 * @since 0.1
 */
AString *      a_string_insert                (AString *string,
                                              asize pos,
                                              const char *value);
/**
 * @brief Insert a char to An AString.
 * @param string the destination AString
 * @param pos the position where do you want to insert
 * @param c the char you want to insert
 * @return string
 * @since 0.1
 */
AString *      a_string_insert_c              (AString *string,
                                              asize pos,
                                              char c);
/**
 * @brief Insert a char to An AString.
 * @param string the destination AString
 * @param pos the position where do you want to insert
 * @param wc the wide char you want to insert
 * @return string
 * @since 0.1
 */
AString *      a_string_insert_unichar        (AString *string,
                                              asize pos,
                                              aunichar wc);
/**
 * @brief Insert a char to An AString.
 * @param string the destination AString
 * @param pos the position where do you want to insert
 * @param value the string you want to insert
 * @param len the max char length to insert
 * @return string
 * @since 0.1
 */
AString *      a_string_insert_len            (AString *string,
                                              asize pos,
                                              const char *value,
                                              asize len);
/**
 * @brief Get the key of the string
 *  When a string is like key=value,it return key
 * @warning You should free the return string when you don't want to use it.
 * @param string the source AString
 * @return a new string
 * @since 0.2
 */
char *         a_string_get_key               (AString *string);
/**
 * @brief Get the value of the string
 *  When a string is like key=value,it return value
 * @warning You should free the return string when you don't want to use it.
 * @param string the source AString
 * @return a new string
 * @since 0.2
 */
char *         a_string_get_value             (AString *string);
/**
 * @brief overwrite an AString from pos
 * @param string the AString you want to overwrite
 * @param pos the overwrite position
 * @param value the string you want to overwrite to
 * @return string
 * @since 0.1
 */
AString *      a_string_overwrite             (AString *string,
                                              asize pos,
                                              const char *value);
/**
 * @brief overwrite an AString from pos, and overwrite the most len char
 * @param string the AString you want to overwrite
 * @param pos the overwrite position
 * @param value the string you want to overwrite to
 * @param len the max char to overwrite
 * @return string
 * @since 0.1
 */
AString *      a_string_overwrite_len         (AString *string,
                                              asize pos,
                                              const char *value,
                                              asize len);
/**
 * @brief Erase something from an AString.
 * @param string the AString you want to erase
 * @param pos the erase start
 * @param len How long you want to erase
 * @return string
 * @since 0.1
 */
AString *      a_string_erase                 (AString *string,
                                              asize pos,
                                              asize len);
/**
 * @brief Truncate an AString, leave the first len char.
 * @param string the AString you want to truncate
 * @param len How long you want to leave
 * @return string
 * @since 0.1
 */
AString *      a_string_truncate              (AString *string,
                                              asize len);
/**
 * @brief Set string's length
 *  if len is smaller than string->len, it'll be truncate,else it'll fill with undefined char.
 * @param string the AString you want to set.
 * @param len the new length
 * @return string
 * @since 0.1
 */
AString *      a_string_set_size              (AString *string,
                                              asize len);
/**
 * @brief wipe off the head and tail's space or tab.
 *  "  abc  " will be "abc"
 * @param string the AString you want to trim
 * @return string
 * @since 0.1
 */
AString *      a_string_trim                  (AString *string);
/**
 * @brief Splits a string into array
 * @param string the AString you want to split
 * @param splitstr the split string
 * @return string
 * @since 0.2
 */
AStringArray * a_string_split                 (AString *string,
                                              const char *splitstr);
/**
 * @brief change an AString to it's substring.
 * @param string the AString you want to sub
 * @param start where do you want to start
 * @param end where do you want to end
 * @return string
 *
 * @note Example:
 * @note
 * @note  str = astring_new("01234567890");
 * @note  
 * @note  astring_substring(str, 2, 5);
 * @note  
 * @note  printf("%s\n", str->str);
 * @note  
 * @note  Run:    2345
 * @since 0.1
 */
AString *      a_string_substring             (AString *string, asize start, asize end);
/**
 * @brief Like substring, but it'll create a new AString to return.
 *  string will not to be changed.
 * @warning You should free the return string when you don't want to use it.
 * @param string the AString you want to sub
 * @param start where do you want to start
 * @param end where do you want to end
 * @return string
 * @since 0.1
 */
AString *      a_string_substring_new         (AString *string, asize start, asize end);
/**
 * @brief Get the char in an AString.
 * @param string the source AString
 * @param position the position you want to get
 * @return the char in position
 * @since 0.1
 */
char           a_string_get_char              (AString *string, asize position);
/**
 * @brief Set the char in an AString.
 * @param string the source AString
 * @param position the position you want to get
 * @param the char you want to change to
 * @return the char in position
 * @since 0.1
 */
AString *      a_string_set_char              (AString *string, asize position, char ch);
/**
 * @brief Find a string from an AString
 * @param string the source AString
 * @param str the string you want to find
 * @param asize the position where you want to find
 * @return the position where the str at, return -1 when no found.
 * @since 0.1
 */
int            a_string_find                  (AString *string,
                                              char *str,
                                              asize position);
/**
 * @brief Replace a string from an AString
 * @param string the source AString
 * @param findstr the string you want to find
 * @param replacestr the string you want to replace
 * @param asize the position where you want to replace
 * @return the position where the str at, return -1 when no found.
 * @since 0.1
 */
int            a_string_replace               (AString *string,
                                              char *findstr,
                                              char *replacestr,
                                              asize position);
/**
 * @brief Replace all string from an AString
 * @param string the source AString
 * @param findstr the string you want to find
 * @param replacestr the string you want to replace
 * @return the count you have replace.
 * @see a_string_replace
 * @since 0.1
 */
int            a_string_replace_all           (AString *string,
                                              char *findstr,
                                              char *replacestr);
/**
 * @brief Frees the memory allocated for the AString.
 * @param string the AString you want to free
 * @since 0.1
 */
void           a_string_free                  (AString *string);
/**
 * @brief Get the hashcode from a string.
 * @param string the AString you want to hash
 * @return the hashcode of the string
 * @note using BKDRHash
 * @since 0.1
 */
auint          a_string_hash                  (AString *string);
/**
 * @brief compare the two AStrings
 * @param string the first AString
 * @param string2 the second AString
 * @return TRUE or FALSE
 * @since 0.1
 */
aboolean       a_string_equal                 (AString *string,
                                              AString *string2);

#endif


