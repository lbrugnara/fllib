#ifndef FL_VECTOR_H
#define FL_VECTOR_H

#include "../Types.h"
#include "Container.h"
#include "Iterator.h"

/*
 * Type: FlVector
 *  
 *  An opaque pointer to a vector instance.
 *
 * ===== C =====
 *  typedef struct FlVector *FlVector;
 * =============
 */
typedef struct FlVector FlVector;

/*
 * Type: struct FlVectorArgs
 *  Arguments to create a new vector using the provided configuration
 * 
 * Members: 
 *  writer - If provided, the vector will use the function to write vales, otherwise the vector will save pointers
 *  cleaner - If provided, the vector will use this function to free the memory used by values
 *  growth_factor - Factor applied to vector resize to expand its capacity
 *  element_size - The size of each element in bytes
 *  capacity - The initial capacity for the vector to be created
 *  max_capacity - If specified, the vector will limit the number of elements that can be stored to this number
 * 
 * Notes:
 *  - Default *growth_factor* is 2.0
 *  - Default *element_size* is sizeof(void*)
 *  - Default *capacity* is *1*
 *  - Default *max_capacity* is *SIZE_MAX* / *element_size*
 * 
 * ===== C ===== 
 * struct FlVectorArgs {
 *      FlContainerWriterFunction writer;
 *      FlContainerCleanupFunction cleaner;
 *      double growth_factor;
 *      size_t element_size;
 *      size_t capacity;
 *      size_t max_capacity;
 * };
 */
struct FlVectorArgs {
    FlContainerWriterFunction writer;
    FlContainerCleanupFunction cleaner;
    double growth_factor;
    size_t element_size;
    size_t capacity;
    size_t max_capacity;
};

/*
 * Function: fl_vector_new
 *  Creates a vector with the provided *capacity* to store pointers, and if *cleaner*
 *  is provided, it will be used to free the pointers on <fl_vector_free> or <fl_vector_remove>
 * 
 * Members:
 *  capacity - <FlVector>'s initial capacity
 *  cleaner - <FlContainerCleanupFunction> to free the memory of the pointers (if provided)
 *
 * Return:
 *  <FlVector> - Instance to the newly created vector
 * 
 * Notes:
 *  Check <struct FlVectorArgs> for the default values used to configure the vector
 */
FlVector* fl_vector_new(size_t capacity, FlContainerCleanupFunction cleaner);

/*
 * Function: fl_vector_new_args
 *  Creates a vector with a specific configuration through the usage of a <struct FlVectorArgs> object
 *
 * Parameters:
 *  args - Configuration for the vector to be created
 *
 * Returns:
 *  <FlVector> - Instance to the newly created vector
 *
 * Notes:
 *  Check <struct FlVectorArgs> for the default values used to configure the vector
 */
FlVector* fl_vector_new_args(struct FlVectorArgs args);

/*
 * Function: fl_vector_length
 *  Returns the number of elements stored in the <FlVector>
 *
 * Parameters:
 *  vector - Vector instance to get its length
 *
 * Return:
 *  size_t - Number of elements currently stored in the *vector*
 *
 */
size_t fl_vector_length(FlVector *vector);

/*
 * Function: fl_vector_growth_factor
 *  Returns the vector's growth factor
 *
 * Parameters:
 *  vector - Vector instance to get its growth factor
 *
 * Returns:
 *  double - growth factor
 *
 */
double fl_vector_growth_factor(FlVector *vector);

/*
 * Function: fl_vector_capacity
 *  Returns the current capacity of the <FlVector>
 *
 * Parameters:
 *  vector - Vector instance to get its capacity
 *
 * Return:
 *  size_t - Current capacity of the *vector*
 *
 */
size_t fl_vector_capacity(FlVector *vector);


/*
 * Function: fl_vector_max_capacity
 *  Returns the vector's maximum capacity. This value minus 1 is
 *  the greatest indexable position of the <FlVector>
 *  
 *
 * Parameters:
 *  vector - Vector to get its maximum capacity
 *
 * Returns:
 *  size_t - Maximum capacity
 *
 */
size_t fl_vector_max_capacity(FlVector *vector);

/*
 * Function: fl_vector_element_size
 *  Returns the size of a single element the vector expects
 *
 * Parameters:
 *  vector - target vector
 *
 * Returns:
 *  size_t - Size of the element type the vector accepts
 *
 */
size_t fl_vector_element_size(FlVector *vector);

/*
 * Function: fl_vector_add
 *  Adds an *element* to the end of the *vector*.
 *
 * Parameters:
 *  vector - Vector where to store the new element
 *  element - Element to be added to the *vector*
 * 
 * Return:
 *   void* - Pointer to the added element
 */
void* fl_vector_add(FlVector *vector, const void *element);

/*
 * Function: fl_vector_insert
 *  Inserts an *element* in the specified *index* of the <FlVector>.
 *
 * Parameters:
 *  vector - Vector where to store the *element*
 *  element - Element to be inserted into the *vector*
 *  index - Position where to insert the *element*
 *
 * Return:
 *  void* - Pointer to the inserted *element*
 * 
 * Notes:
 *  If *index* is greater than the current capacity, the vector will be 
 *  expanded to make root for the new element. The created positions will 
 *  be zero'd out
 */
void* fl_vector_insert(FlVector *vector, const void *element, size_t index);

/*
 * Function: fl_vector_get
 *  Returns a pointer to the element placed in the specified *index* of the <FlVector>.
 *  If the index does not exist, this function returns <NULL>
 *
 * Parameters:
 *  vector - Vector where to get its _index-nth_ position
 *  index - Index of the element to retrieve
 *
 * Return:
 *  void* - Pointer to the element stored at vector's _index-nth_ position
 *
 */
void* fl_vector_get(FlVector *vector, size_t index);

/*
 * Function: fl_vector_last
 *  Returns a pointer to the last element of the <FlVector>
 *
 * Parameters
 *  vector - Vector where to get its last element
 *
 * Return:
 *  void* - Pointer to the last element stored in the vector.
 * 
 * Notes:
 *  If vector is empty, this function returns NULL
 */
void* fl_vector_last(FlVector *vector);

/*
 * Function: fl_vector_shift
 *  Removes the first element of the <FlVector>, and
 *
 * FlVector *vector - Vector to remove its first element
 * void *dest - Destination to copy the element to be removed
 *
 * {return: bool} If vector length is 0, this function returns 0,
 * if not true
 *
 */
void* fl_vector_shift(FlVector *vector, void *dest);

/*
 * Function: fl_vector_pop
 *  Removes the last element in {vector}. If {dest} is not null,
 *  the element to be removed will be copied into it. If *dest* is NULL
 *  and the cleaner function is configured for the *vector*, the value will
 *  be cleaned up.
 *
 * Parameters:
 *  vector - <FlVector> to remove its last element
 *  dest - If present, the removed element will be copied into it
 *
 * Return:
 *  bool - *false* if vector length is 0, true otherwise
 *
 */
void* fl_vector_pop(FlVector *vector, void *dest);

/**
 * Return true if {vector} contains {elem}
 */
/*
 * Function: fl_vector_contains
 *
 * Returns true of {vector} contains {elem}.
 *
 * FlVector *vector} Vector where to search {elem -
 * const void *elem - Element to search in vector
 *
 * {return: bool} true if {elem} is present in {vector}
 *
 */
bool fl_vector_contains(FlVector *vector, const void *elem);

/*
 * Function: fl_vector_concat
 *
 * Appends {vector2} elements at the end of {vector}
 *
 * FlVector *vector} Vector where to insert the elements of {vector2 -
 * FlVector *vector2} Vector where to retrieve its elements to be inserted into {vector -
 *
 * {return: void}
 *
 */
void fl_vector_concat(FlVector *vector, FlVector *vector2);

/*
 * Function: fl_vector_concat
 *
 * Creates a new vector with the elements of {vector} and {vector2}
 *
 * FlVector *vector - First vector to be copied in the new vector
 * FlVector *vector2} Second vector to be copied (after {vector -) in the new vector
 *
 * {return: FlVector} New vector that contains {vector}'s elements and {vector2}'s elements
 *
 */
FlVector* fl_vector_merge(FlVector *vector, FlVector *vector2);

/**
 * Removes the {pos}-th element in {vector}.
 * The removed element will be copied in {dest} if it's different from NULL
 */
/*
 * Function: fl_vector_remove
 *
 * Removes the element at position {pos}, and if {dest} is not null,
 * the target element will be copied into it.
 *
 * FlVector *vector} Vector where to remove element at position {pos -
 * size_t pos - Index of the element to be removed
 * void *dest - Element to be removed will be copied to it if it is not null
 *
 * {return: bool} If {pos} is greater than the current {vector}'s length,
 * this function returns false. Otherwise it returns true.
 *
 */
bool fl_vector_remove(FlVector *vector, size_t pos, void *dest);

/**
 * Free the memory reserved for vector 
 */
/*
 * Function: fl_vector_free
 *
 * Deletes {vector} releaseing its memory
 *
 * FlVector *vector - Vector to be deleted
 *
 * {return: void}
 *
 */
void fl_vector_free(FlVector *vector);

void* fl_vector_to_array(FlVector *vector);

/* Datatype specifics getters/setters. (Can be extended by each datatype created in the same way here) */

/*
 * Function: fl_vector_begin
 *
 * Returns an {FlIterator} that points to the first element in vector -
 *
 * const FlVector *vector - Vector handle to be pointed by the iterator
 *
 * {return: FlIterator*} Iterator pointing to the first element in vector -
 *
 */
FlIterator* fl_vector_begin(const FlVector *vector);

/*
 * Function: fl_vector_end
 *
 * Returns an {FlIterator} that points to the past-last-nth element in vector -
 * The FlIterator returned by this function MUST NOT be dereferenced by fl_iterator_value.
 *
 * const FlVector *vector - Vector handle to be pointed by the iterator
 *
 * {return: FlIterator*} Iterator pointing beyond the last element in vector -
 *
 */
FlIterator* fl_vector_end(const FlVector *vector);

#endif /* FL_VECTOR_H */
