#include "List.h"
#include "Dictionary.h"
#include "KeyValuePair.h"

struct FlDictionary 
{
	FlList list;
	size_t kdtsize;
	size_t vdtsize;
};


bool 
kvp_key_cmp(const FlPointer celem, const FlPointer key, size_t dtsize)
{
	FlKeyValuePair *kvp = celem;
	return memcmp(fl_kvp_get_key(*kvp), key, fl_kvp_key_dtsize(*kvp)) == 0;
}

bool 
kvp_val_cmp(const FlPointer celem, const FlPointer val, size_t dtsize)
{
	FlKeyValuePair *kvp = celem;
	return memcmp(fl_kvp_get_val(*kvp), val, fl_kvp_val_dtsize(*kvp)) == 0;
}

/**
 * {@dtsize} DataType size for the new dictionary
 *
 * Creates an empty dictionary for elements of {dtsize} size
 */
FlDictionary 
fl_dictionary_new(size_t keydtsize, size_t valdtsize)
{
	FlDictionary dict = fl_calloc(1, sizeof(struct FlDictionary));
	dict->list = fl_list_new(sizeof(FlKeyValuePair));
	dict->kdtsize = keydtsize;
	dict->vdtsize = valdtsize;
	return dict;
}

size_t
fl_dictionary_length(const FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	return fl_list_length(dict->list);
}

size_t
fl_dictionary_key_dtsize(const FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	return dict->kdtsize;
}

size_t
fl_dictionary_val_dtsize(const FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	return dict->vdtsize;
}

/**
 * {@dictionary} Target dictionary to add a new value
 * {@elem} Element to add to the dictionary. (will be copied {dict->dtsize} bytes of elem)
 * Return a pointer to the inserted element
 */
FlKeyValuePair
fl_dictionary_add(FlDictionary dict, const FlPointer key, const FlPointer val)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");
	flm_assert(val != NULL, "Value cannot be NULL");
	
	FlKeyValuePair *kvp = fl_list_find_h(dict->list, kvp_key_cmp, key);
	flm_assert(kvp == NULL, "Key already exists in FlDictionary");

	FlKeyValuePair newkvp = fl_kvp_new_set(key, dict->kdtsize, val, dict->vdtsize);
	fl_list_add(dict->list, &newkvp);
	return newkvp;
}

FlKeyValuePair
fl_dictionary_set(FlDictionary dict, const FlPointer key, const FlPointer val)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");
	flm_assert(val != NULL, "Value cannot be NULL");

	FlKeyValuePair *kvp = NULL;
	if ((kvp = fl_list_find_h(dict->list, kvp_key_cmp, key)) != NULL)
	{
		fl_kvp_set(*kvp, key, val);		
	}
	else
	{
		FlKeyValuePair newkvp = fl_kvp_new_set(key, dict->kdtsize, val, dict->vdtsize);
		fl_list_add(dict->list, &newkvp);
		kvp = &newkvp;
	}
	return *kvp;
}

/**
 * Returns a pointer to the element in with key {key}
 */
FlPointer
fl_dictionary_get_val(const FlDictionary dict, const FlPointer key)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");

	FlKeyValuePair *kvp = fl_list_find_h(dict->list, kvp_key_cmp, key);
	if (kvp == NULL)
		return NULL;
	return fl_kvp_get_val(*kvp);
}

FlPointer
fl_dictionary_get_key(const FlDictionary dict, const FlPointer val)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(val != NULL, "Value cannot be NULL");

	FlKeyValuePair *kvp = fl_list_find_h(dict->list, kvp_val_cmp, val);
	if (kvp == NULL)
		return NULL;
	return fl_kvp_get_key(*kvp);
}

FlGenericArray fl_dictionary_keys(const FlDictionary dictionary)
{
	FlList kvplist = dictionary->list;
	size_t nelem = fl_list_length(kvplist);
	FlGenericArray *arr = fl_array_new(dictionary->kdtsize, nelem);
	for (size_t i=0, k=0; i < nelem; (i++, k += dictionary->kdtsize))
	{
		FlKeyValuePair *kvp = fl_list_get(kvplist, i);
		memcpy(((FlByte*)arr)+k, fl_kvp_get_key(*kvp), dictionary->kdtsize);
	}
	return arr;
}

FlGenericArray fl_dictionary_values(const FlDictionary dictionary)
{
	FlList kvplist = dictionary->list;
	size_t nelem = fl_list_length(kvplist);
	FlGenericArray *arr = fl_array_new(dictionary->vdtsize, nelem);
	for (size_t i=0, k=0; i < nelem; (i++, k += dictionary->vdtsize))
	{
		FlKeyValuePair *kvp = fl_list_get(kvplist, i);
		memcpy(((FlByte*)arr)+k, fl_kvp_get_val(*kvp), dictionary->vdtsize);
	}
	return arr;
}

FlList
fl_dictionary_to_list(const FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	return dict->list;
}

/**
 * Returns a pointer to a FlKeyValuePair element where the key is {key}
 */
FlKeyValuePair
fl_dictionary_get_kvp(const FlDictionary dict, const FlPointer key)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");

	FlKeyValuePair *kvp = fl_list_find_h(dict->list, kvp_key_cmp, key);
	if (kvp == NULL)
		return NULL;
	return *kvp;
}

/**
 * Return true if {dictionary} contains a key with value {key}
 */
bool
fl_dictionary_contains_key(const FlDictionary dict, const FlPointer key)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");

	return fl_list_find_h(dict->list, kvp_key_cmp, key) != NULL;
}

/**
 * Return true if {dictionary} contains a value with value {value}
 */
bool
fl_dictionary_contains_val(const FlDictionary dict, const FlPointer value)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(value != NULL, "Value cannot be NULL");

	return fl_list_find_h(dict->list, kvp_val_cmp, value) != NULL;
}

/**
 * Append {dictionary2} elements at the end of {dictionary}. Repeated keys are overrided
 * in {dictionary}
 */
void
fl_dictionary_concat(FlDictionary dict, FlDictionary dict2)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(dict2 != NULL, "Dictionary2 cannot be NULL");

	for (int i=0, l2 = fl_dictionary_length(dict2); i < l2; i++)
	{
		FlKeyValuePair* kvp = fl_list_get(dict2->list, i);
		fl_dictionary_add(dict, fl_kvp_get_key(*kvp), fl_kvp_get_val(*kvp));
	}
}

/**
 * Creates a new FlDictionary that will contain all the elements of
 * {dictionary} and {dictionary2}. Repeated keys are overrided from {dictionary2}
 * in {dictionary}
 */
FlDictionary
fl_dictionary_merge(const FlDictionary dict1, const FlDictionary dict2)
{
	flm_assert(dict1 != NULL, "Dictionary1 cannot be NULL");
	flm_assert(dict2 != NULL, "Dictionary2 cannot be NULL");

	FlDictionary newdict = fl_calloc(1, sizeof(struct FlDictionary));
	newdict->list = fl_list_new(sizeof(FlKeyValuePair));
	newdict->kdtsize = dict1->kdtsize;
	newdict->vdtsize = dict1->vdtsize;
	for (int i=0, l1 = fl_dictionary_length(dict1); i < l1; i++)
	{
		FlKeyValuePair* kvp = fl_list_get(dict1->list, i);
		fl_dictionary_add(newdict, fl_kvp_get_key(*kvp), fl_kvp_get_val(*kvp));
	}
	for (int i=0, l2 = fl_dictionary_length(dict2); i < l2; i++)
	{
		FlKeyValuePair* kvp = fl_list_get(dict2->list, i);
		fl_dictionary_add(newdict, fl_kvp_get_key(*kvp), fl_kvp_get_val(*kvp));
	}
	return newdict;
}

/**
 * Remove from {dictionary}, the element with key {key}.
 * The value for the removed element will be copied in {dest}
 */
bool
fl_dictionary_remove(FlDictionary dict, const FlPointer key, FlPointer dest)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(key != NULL, "Key cannot be NULL");
	FlKeyValuePair kvp;
	bool res = fl_list_remove_h(dict->list, kvp_key_cmp, key, &kvp);
	if (dest != NULL)
	{
		FlPointer val = fl_kvp_get_val(kvp);
		memcpy(dest, val, dict->vdtsize);
		fl_kvp_delete(kvp);
	}
	return res;
}

/**
 * Remove all elements from the dictionary
 */
void
fl_dictionary_clear(FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	for (int i=0, l = fl_dictionary_length(dict); i < l; i++)
	{
		FlKeyValuePair* kvp = fl_list_get(dict->list, i);
		fl_kvp_delete(*kvp);
	}
	fl_list_clear(dict->list);
}

/**
 * Releases the memory used by a {FlKeyValuePair}
 */
void
fl_dictionary_delete_kvp(FlKeyValuePair* nodedata)
{
	flm_assert(nodedata != NULL, "FlKeyValuePair cannot be NULL");

	fl_kvp_delete(*nodedata);
	fl_free(nodedata);
}

/**
 * Free the memory reserved for dictionary 
 */
void
fl_dictionary_delete(FlDictionary dict)
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");

	fl_list_delete_h(dict->list, (void (*)(FlByte *))&fl_dictionary_delete_kvp);
	fl_free(dict);
}

/**
 * Free the memory reserved for dictionary using {delete_handler} for each element.
 * The handler MUST free the memory used by each element
 */
void
fl_dictionary_delete_h(FlDictionary dict, void (*delete_handler)(FlByte*))
{
	flm_assert(dict != NULL, "Dictionary cannot be NULL");
	flm_assert(delete_handler != NULL, "Handler cannot be NULL");

	fl_list_delete_h(dict->list, delete_handler);
	fl_free(dict);
}

/* -------------------------------------------------------------
 * FlIterator support
 * -------------------------------------------------------------
 */
/* -------------------------------------------------------------
 * FlIterator support
 * -------------------------------------------------------------
 */
typedef struct FlDictionaryIterator {
    FlIteratorType type;
    FlListNode current;
    FlListNode next;
    FlListNode prev;
} FlDictionaryIterator;

static void it_next(FlPointer it)
{
    FlDictionaryIterator *lit = (FlDictionaryIterator*)it;
    lit->prev = lit->current;
    lit->current = lit->next;
    lit->next = fl_list_node_next(lit->current);
}

static void it_prev(FlPointer it)
{
    FlDictionaryIterator *lit = (FlDictionaryIterator*)it;
    lit->next = lit->current;
    lit->current = lit->prev;
    lit->next = fl_list_node_prev(lit->current);
}

static FlPointer it_value(FlPointer it)
{
    FlDictionaryIterator *lit = (FlDictionaryIterator*)it;
    return fl_list_node_data(lit->current);
}

static bool it_equals(FlPointer it1, FlPointer it2)
{
    FlDictionaryIterator *lit1 = (FlDictionaryIterator*)it1;
    FlDictionaryIterator *lit2 = (FlDictionaryIterator*)it2;
    return lit1->current == lit2->current;
}

static bool it_start(FlPointer it, FlPointer container)
{
    FlDictionaryIterator *lit = it;
    FlDictionary dict = container;
    FlListNode head = fl_list_get_node(dict->list, 0);
    return  head != NULL && lit->current == head;
}

static bool it_end(FlPointer it, FlPointer container)
{
    FlDictionaryIterator *lit = it;
    FlDictionary dict = container;
    FlListNode tail = fl_list_get_node(dict->list, fl_list_length(dict->list)-1);
    return  tail != NULL && lit->prev == tail;
}

static void it_delete(FlPointer it)
{
    fl_free(it);
}

FlIterator fl_dictionary_start(const FlDictionary dict)
{
    FlDictionaryIterator *dict_it = fl_calloc(1, sizeof(struct FlDictionaryIterator));
    dict_it->prev = NULL;
    dict_it->current = fl_list_get_node(dict->list, 0);
    dict_it->next = fl_list_node_next(dict_it->current);
    return fl_iterator_new(IT_DICTIONARY, dict_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}

FlIterator fl_dictionary_end(const FlDictionary dict)
{
    FlDictionaryIterator *dict_it = fl_calloc(1, sizeof(struct FlDictionaryIterator));
    dict_it->prev = fl_list_get_node(dict->list, fl_list_length(dict->list)-1);
    dict_it->current = NULL;
    dict_it->next = NULL;
    return fl_iterator_new(IT_DICTIONARY, dict_it, &it_next, &it_prev, &it_value, &it_equals, &it_start, &it_end, &it_delete);
}
