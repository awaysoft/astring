/**
 * @file astringlist.h
 * @brief AStringList
 * @author Tom
 * @date 2011-11-25
 * @version 0.2
 * @note
 * A AString is an object that handles the memory management of a C string for you. You can think of it as similar to a Java StringBuffer. In addition to the string itself, GString stores the length of the string, so can be used for binary data with embedded nul bytes. To access the C string managed by the AString string, simply use string->str.
 */
#ifndef __A_STRING_LIST_H__
#define __A_STRING_LIST_H__

#include "astring.h"

#define A_STRING_LIST_FLAG       102
#define A_IS_STRING_LIST(string) (*(auchar *)string == A_STRING_LIST_FLAG)

/**
 * @brief AStringList struct
 */
struct _AStringList {
	auchar             flag;  /**< Don't Change it, it used to check a object is AStringList. */
	volatile aboolean  lock;  /**< Don't Change it, it used to keep thread safe. */

	asize              length;  /**< contains the length of the stringlist. */
	asize              allocated_len;  /**< contains the length of the stringlist. */

	AString **         data;  /**< points to the string data. */
};

/**
 * @brief a function to be variable in qsort 
 */
typedef int (*A_CMP)(const void *a, const void *b);
/**
 * @brief StringList Main Struct 
 */
typedef struct _AStringList AStringList;

/**
 * @brief Create a new AStringList with none string
 * @return A new AStringList
 * @since 0.2
 */
AStringList *   a_string_list_new          (void);
/**
 * @brief Copies string from an AString into an AStringList, destroying any previous contents.
 * It alias a_string_list_set_text;
 * @param list the AStringList to opera
 * @param string the AString to assign
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_assign       (AStringList *list,
                                            AString *string);

/**
 * @brief It's like assign,but,split will be \n.
 * @param list the AStringList to opera
 * @param string the AString to assign
 * @param split the split string
 * @return list
 * @see a_string_list_assign
 * @since 0.2
 */
AStringList *   a_string_list_assign_split (AStringList *list,
                                            AString *string,
                                            AString *split);
/**
 * @brief Create an AStringList from a file.
 * @param filename the filename you want to load
 * @return a new AStringList
 * @since 0.2
 */
AStringList *   a_string_list_load_from_file(const char * filename);
/**
 * @brief Append AStringList to exists AStringList
 * @param list the destination AStringList
 * @param list2 the list you want to append
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_append       (AStringList *list,
                                            AStringList *list2);
/**
 * @brief Prepend AStringList to exists AStringList
 * @param list the destination AStringList
 * @param list2 the list you want to prepend
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_prepend      (AStringList *list,
                                            AStringList *list2);
/**
 * @brief Add an AString to the end of AStringList
 * @param list the destination AStringList
 * @param string the string you want to add
 * @return list
 * @see a_string_list_append
 * @since 0.2
 */
AStringList *   a_string_list_add          (AStringList *list,
                                            AString *string);
/**
 * @brief Insert an AString to AStringList
 * @param list the destination AStringList
 * @param index the position you want to insert
 * @param string the string you want to insert
 * @return list
 * @see a_string_list_add
 * @since 0.2
 */
AStringList *   a_string_list_insert       (AStringList *list,
                                            asize index,
											AString *string);
/**
 * @brief Delete an AString from an AStringList
 * @param list the destination AStringList
 * @param index the position you want to delete
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_delete       (AStringList *list,
                                            asize index);
/**
 * @brief Clear the AStringList
 * @param list the AStringList you want to clear
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_clear        (AStringList *list);
/**
 * @brief Exchange two lines in an AStringList
 * @param list the destination AStringList
 * @param index1 the first position
 * @param index2 the second position
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_exchange     (AStringList *list,
                                            asize index1,
											asize index2);
/**
 * @brief Find an AString from an AStringList
 * @param list An AStringList
 * @param string the string to find
 * @param index the position you want to find start
 * @return the position, return -1 when doesn't find it.
 * @since 0.2
 */
asize           a_string_list_find         (AStringList *list,
                                            AString *string,
											asize index);
/**
 * @brief Find a key from an AStringList
 *  When the stringlist is like key=value, find the key in the list
 * @param list An AStringList
 * @param string the string to find
 * @param index the position you want to find start
 * @return the position, return -1 when doesn't find it.
 * @since 0.2
 */
asize			a_string_list_find_name    (AStringList *list,
                                            AString *string,
											asize index);
/**
 * @brief Get a value from an AStringList
 *  When the stringlist is like key=value, get the value from a string
 * @param list An AStringList
 * @param index the position you want to get value
 * @return a new AString
 * @see a_string_list_find_name
 * @since 0.2
 */
AString *       a_string_list_get_value    (AStringList *list,
                                            asize index);
/**
 * @brief Get a AString from an AStringList
 * @param list An AStringList
 * @param index the position you want to get
 * @return a new AString
 * @see a_string_list_find_name
 * @since 0.2
 */
AString *       a_string_list_get_index    (AStringList *list, 
                                            asize index);
/**
 * @brief Sort the StringList
 * @param list An AStringList 
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_sort         (AStringList *list);
/**
 * @brief Sort the StringList by a custom function
 * @param list An AStringList
 * @param cmp An Compare function
 * @return list
 * @since 0.2
 */
AStringList *   a_string_list_sort_custom  (AStringList *list,
											A_CMP cmp);
/**
 * @brief Check the StringList is sorted
 * @param list An AStringList 
 * @return TRUE or FALSE
 * @since 0.2
 */
aboolean        a_string_list_sorted       (AStringList *list);
/**
 * @brief Check the StringList is sorted
 * @param list An AStringList 
 * @param cmp An Compare function
 * @return TRUE or FALSE
 * @since 0.2
 */
aboolean        a_string_list_sorted_custom(AStringList *list,
											A_CMP cmp);
/**
 * @brief duplicate the AStringList
 * @warning You should free the return string when you don't want to use it.
 * @param list An AStringList 
 * @return a new AStringList
 * @since 0.2
 */
AStringList *   a_string_list_dup          (AStringList *list);
/**
 * @brief export an AStringList to an AString
 * @warning You should free the return string when you don't want to use it.
 * @param list An AStringList
 * @param returnchar the return char, 0:'\n', 1:'\r', 2:"\r\n"
 * @return A new AString
 * @since 0.2
 */
AString *       a_string_list_get_text     (AStringList *list, auchar returnchar);
/**
 * @brief export an AStringList to a file
 * @warning You should free the return string when you don't want to use it.
 * @param list An AStringList
 * @param filename the file you want to export
 * @param returnchar the return char, 0:'\n', 1:'\r', 2:"\r\n"
 * @return TRUE or FALSE
 * @since 0.2
 */
aboolean        a_string_list_save_to_file (AStringList *list,
                                            const char * filename,
                                            auchar returnchar);
/**
 * @brief compare the two AStringLists
 * @param list the first AStringList
 * @param list2 the second AStringList
 * @return TRUE or FALSE
 * @since 0.2
 */
aboolean        a_string_list_equal        (AStringList *list1,
                                            AStringList *list2);
/**
 * @brief Frees the memory allocated for the AStringList.
 * @param string the AStringList you want to free
 * @since 0.2
 */
void            a_string_list_free         (AStringList *list);

#endif


