#ifndef FL_VECTOR_H
#define FL_VECTOR_H

#include <stdbool.h>
#include "../Types.h"
#include "../Array.h"
#include "Container.h"
#include "Iterator.h"

/*
 * Struct: FlVector
 * 
 *  A vector represents a contiguous storage location that can grow and shrink dynamically. One of the advantages
 *  over <FlArray>s is that the storage is handled automatically by the container. On the other hand, unlike <FlArray>s,
 *  you can't use array subscripts to access the vector elements, instead you need to use one of the provided functions.
 * 
 *  The following is an example that shows how to work with a vector that makes copies of the inserted values:
 * 
 * --- C
 *      // To work with primitive values, the vector needs to know the size of the element to be stored, in this case, it is
 *      // the size of an integer
 *      FlVector *vector_of_ints = flm_vector_new_with(.element_size = sizeof(int));
 *      
 *      int x = 10, y = 20;
 *        
 *      // fl_vector_add expects "pointer to" integer. Append an element to the vector (x will be copied to position 0)
 *      fl_vector_add(vector_of_ints, &x);
 *      
 *      // fl_vector_insert expects "pointer to" integer. Insert an element into the index 0 and move all the elements 
 *      // in the vector to the right, one position
 *      fl_vector_insert(vector_of_ints, &y, 0);
 *        
 *      int y_copy = flm_vector_get(vector_of_ints, int, 0);
 *      int x_copy = flm_vector_get(vector_of_ints, int, 1);
 *        
 *      assert(y_copy == y); // Element at index 0 must be equals to variable y
 *      assert(x_copy == x); // Element at index 1 must be equals to variable x
 *      
 *      fl_vector_free(vector_of_ints);
 * ---
 * 
 *  The next example shows how to work with a vector that stores pointers instead of values:
 * 
 * --- C
 *      // The default "constructor" of vector configures it to store pointers
 *      FlVector *vector_of_int_ptrs = fl_vector_new();
 *      
 *      int x = 10, y = 20;
 *      int *px = &x, *py = &y;
 *
 *      // fl_vector_add expects "pointer to" pointer to integer. Append an element to the vector (px will be copied to position 0)
 *      fl_vector_add(vector_of_int_ptrs, &px);
 * 
 *      // fl_vector_insert expects "pointer to" pointer to integer. Insert an element into the index 0 and move all the elements 
 *      // in the vector to the right, one position
 *      fl_vector_insert(vector_of_int_ptrs, &py, 0);
 *      
 *      int *py_copy = flm_vector_get(vector_of_int_ptrs, int*, 0);
 *      int *px_copy = flm_vector_get(vector_of_int_ptrs, int*, 1);
 *      
 *      assert(py_copy == py); // Element at index 0 must be equals to pointer py
 *      assert(px_copy == px); // Element at index 1 must be equals to pointer px
 *      
 *      fl_vector_free(vector_of_int_ptrs);
 * ---
 */

/*
 * Type: FlVector
 * 
 * --- Prototype
 *      typedef struct FlVector FlVector;
 * ---
 * 
 *  FlVector is an incomplete type. All the functions on this module work with opaque pointers.
 * 
 */
typedef struct FlVector FlVector;

/*
 * type: FlVectorArgs
 * 
 *  The members of this struct contain all the possible properties that can be configured to create a new instance of a vector.
 * 
 * --- Prototype
 * struct FlVectorArgs {
 *      FlContainerWriterFn writer;
 *      FlContainerCleanupFn cleaner;
 *      double growth_factor;
 *      size_t element_size;
 *      size_t capacity;
 *      size_t max_capacity;
 * };
 * ---
 * 
 * Members: 
 *  writer - When provided, a function to write the elements into the vector or out of it
 *  cleaner - When provided, the vector will use this function to free the memory used by each value
 *  growth_factor - Factor applied during a resize operation in order to expand the vector capacity
 *  element_size - The size of the type of elements to be stored into the vector, in bytes
 *  capacity - The minimum memory to be allocated on vector creation
 *  max_capacity - If provided, the vector will limit the number of elements that can be stored up to this number
 * 
 * Notes:
 *  Default value for the members when they are not provided are:
 * 
 *      - _writer_ uses the <fl_container_writer> function
 *      - _cleaner_ is NULL, which means the vector will not free the memory of its elements on removal
 *      - _growth_factor_ is 2.0
 *      - _element_size_ is sizeof(void*)
 *      - _capacity_ is 1
 *      - _max_capacity_ is SIZE_MAX / element_size
 */
struct FlVectorArgs {
    FlContainerWriterFn writer;
    FlContainerCleanupFn cleaner;
    double growth_factor;
    size_t element_size;
    size_t capacity;
    size_t max_capacity;
};

/*
 * Macro: flm_vector_new_with
 * 
 * --- Prototype
 *      #define flm_vector_new_with(...) fl_vector_new_args((struct FlVectorArgs) { __VA_ARGS__ })
 * ---
 * 
 *  Shortcut for the <fl_vector_new_args> function. It accepts the members of the <struct FlVectorArgs> struct
 *  as parameters either using the designated initializers or the positional initializers.
 * 
 *
 * Parameters:
 *  ... - A list of values representing either the designated or positional initializers of the <struct FlVectorArgs> struct
 *
 * Returns:
 *  <FlVector>* - Pointer to the created vector object or NULL if an error occurs on the initialization
 *
 * Example:
 * 
 * --- C
 *      // This initializes a vector with allocated capacity for 1 element, the rest of the arguments ar the default ones
 *      FlVector *v1 = flm_vector_new_with(.capacity = 1);
 * 
 *      // This initializes a vector using all the arguments:
 *      //  .writer = fl_container_writer
 *      //  .cleaner = fl_container_cleaner_pointer
 *      //  .growth_factor = 1.5
 *      //  .element_size = sizeof(int*)
 *      //  .capacity = 10
 *      //  .max_capacity = 30
 *      FlVector *v2 = flm_vector_new_with(fl_container_writer, fl_container_cleaner_pointer, 1.5, sizeof(int*), 10, 30);
 * ---
 * 
 * Notes:
 *  -   Even though both designated initializers and positional initializers are supported, using the *designated* initializers
 *      ensures that future updates that might impact the order of the elements in the <FlVectorArgs> struct will not break
 *      usages of this macro
 * 
 *  -   The object returned by this function must be freed using the <fl_vector_free> function
 */
#define flm_vector_new_with(...) fl_vector_new_args((struct FlVectorArgs) { __VA_ARGS__ })

/*
 * Macro: flm_vector_get
 * 
 * --- Prototype
 *      #define flm_vector_get(vector, type, index) (*(type*) fl_vector_ref_get((vector), (index)))
 * ---
 * 
 *  Returns the value placed at index-th within the vector. This is the preferred 
 *  way to retrieve elements from the vector to avoid pointer invalidations because of destructive operations.
 * 
 * Parameters:
 *  vector - Vector object
 *  type - Expected type of the value a the provided index
 *  index - Index
 *
 * Returns:
 *  type - The value placed in the index-th position of the vector
 * 
 * Example:
 * 
 * --- C
 *      int n[] = { 10, 15, 20 };
 * 
 *      FlVector *int_vector = flm_vector_new_with(.element_size = sizeof(int));
 * 
 *      fl_vector_add(int_vector, &n[0]);
 *      assert(flm_vector_get(int_vector, int, 0) == n[0]);
 * 
 *      fl_vector_add(int_vector, &n[1]);
 *      assert(flm_vector_get(int_vector, int, 1) == n[1]);
 * 
 *      fl_vector_add(int_vector, &n[2]);
 *      assert(flm_vector_get(int_vector, int, 2) == n[2]);
 * ---
 */
#define flm_vector_get(vector, type, index) (*(type*) fl_vector_ref_get((vector), (index)))

/*
 * Macro: flm_vector_first
 * 
 * --- Prototype
 *      #define flm_vector_first(vector, type) (*(type*) fl_vector_ref_first((vector)))
 * ---
 * 
 *  Returns the value placed at the first index of the vector. This is the preferred way to retrieve the
 *  first element from the vector to avoid pointer invalidations because of destructive operations.
 * 
 * Parameters:
 *  vector - Vector object
 *  type - Expected type of the value a the provided index
 *
 * Returns:
 *  type - The value placed in the first position of the vector
 * 
 * Example:
 * 
 * --- C
 *      int n[] = { 10, 15, 20 };
 * 
 *      FlVector *int_vector = flm_vector_new_with(.element_size = sizeof(int));
 * 
 *      fl_vector_insert(int_vector, &n[0], 0);
 *      assert(flm_vector_first(int_vector, int) == n[0]);
 * 
 *      fl_vector_insert(int_vector, &n[1], 0);
 *      assert(flm_vector_first(int_vector, int) == n[1]);
 * 
 *      fl_vector_insert(int_vector, &n[2], 0);
 *      assert(flm_vector_first(int_vector, int) == n[2]);
 * ---
 */
#define flm_vector_first(vector, type) (*(type*) fl_vector_ref_first((vector)))

/*
 * Macro: flm_vector_last
 * 
 * --- Prototype
 *      #define flm_vector_last(vector, type) (*(type*) fl_vector_ref_last((vector)))
 * ---
 * 
 *  Returns the value placed at the last index of the vector. This is the preferred way to retrieve the
 *  last element from the vector to avoid pointer invalidations because of destructive operations.
 * 
 * Parameters:
 *  vector - Vector object
 *  type - Expected type of the value a the provided index
 *
 * Returns:
 *  type - The value placed in the last position of the vector
 * 
 * Example:
 * 
 * --- C
 *      int n[] = { 10, 15, 20 };
 * 
 *      FlVector *int_vector = flm_vector_new_with(.element_size = sizeof(int));
 * 
 *      fl_vector_add(int_vector, &n[0]);
 *      assert(flm_vector_last(int_vector, int) == n[0]);
 * 
 *      fl_vector_add(int_vector, &n[1]);
 *      assert(flm_vector_last(int_vector, int) == n[1]);
 * 
 *      fl_vector_add(int_vector, &n[2]);
 *      assert(flm_vector_last(int_vector, int) == n[2]);
 * ---
 */
#define flm_vector_last(vector, type) (*(type*) fl_vector_ref_last((vector)))

/*
 * Function: fl_vector_new
 *  Creates an instance of a vector that can store pointers.
 * 
 * Members:
 *  This function does not take parameters
 *
 * Return:
 *  <FlVector>* - Instance to the newly created vector
 * 
 * Notes:
 *  -   The object returned by this function must be freed using the <fl_vector_free> function
 *  -   The default configuration of the created vector is:
 *      - _writer_ uses the <fl_container_writer> function
 *      - _cleaner_ is NULL, which means the vector will not free the memory of its elements on removal
 *      - _growth_factor_ is 2.0
 *      - _element_size_ is sizeof(void*)
 *      - _capacity_ is 1
 *      - _max_capacity_ is SIZE_MAX / element_size
 */
FlVector* fl_vector_new(void);

/*
 * Function: fl_vector_new_args
 *  Creates a vector with a specific configuration through the usage of an <FlVectorArgs> struct
 *
 * Parameters:
 *  args - Configuration for the vector to be created
 *
 * Returns:
 *  <FlVector>* - Instance to the newly created vector
 *
 * Notes:
 *  -   Check <FlVectorArgs> struct for the default values used to configure the vector
 *  -   The object returned by this function must be freed using the <fl_vector_free> function
 */
FlVector* fl_vector_new_args(struct FlVectorArgs args);

/*
 * Function: fl_vector_length
 *  Returns the number of elements stored in the vector
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return:
 *  size_t - Number of elements currently stored in the vector
 */
size_t fl_vector_length(FlVector *vector);

/*
 * Function: fl_vector_cleanup_fn_get
 *  Returns the current cleanup function the vector uses, or NULL if there is no cleanup function
 *
 * Parameters:
 *  vector - Vector object
 *
 * Returns:
 *  FlContainerCleanupFn - The cleanup function or NULL
 */
FlContainerCleanupFn fl_vector_cleanup_fn_get(FlVector *vector);

/*
 * Function: fl_vector_cleanup_fn_set
 *  Updates the cleanup function for the vector and returns the old cleanup function, if present, or NULL
 *
 * Parameters:
 *  vector - Vector object
 *  cleanup_fn - The new cleanup function
 *
 * Returns:
 *  FlContainerCleanupFn - The old cleanup function or NULL
 */
FlContainerCleanupFn fl_vector_cleanup_fn_set(FlVector *vector, FlContainerCleanupFn cleanup_fn);

/*
 * Function: fl_vector_growth_factor
 *  Returns the vector's growth factor
 *
 * Parameters:
 *  vector - Vector object
 *
 * Returns:
 *  double - growth factor
 */
double fl_vector_growth_factor(FlVector *vector);

/*
 * Function: fl_vector_capacity
 *  Returns the current capacity of the vector. This value minus 1 is the greatest indexable position of the vector.
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return:
 *  size_t - Current capacity of the vector
 */
size_t fl_vector_capacity(FlVector *vector);

/*
 * Function: fl_vector_max_capacity
 *  Returns the vector's maximum capacity. If equals to the current capacity (<fl_vector_capacity>), this value 
 *  minus 1 is the greatest indexable position of the vector
 *
 * Parameters:
 *  vector - Vector object
 *
 * Returns:
 *  size_t - Maximum capacity
 */
size_t fl_vector_max_capacity(FlVector *vector);

/*
 * Function: fl_vector_element_size
 *  Returns the size in bytes of the element type this vector can store
 *
 * Parameters:
 *  vector - Vector object
 *
 * Returns:
 *  size_t - Size in bytes of the element type this vector can store
 */
size_t fl_vector_element_size(FlVector *vector);

/*
 * Function: fl_vector_add
 *  Adds an element to the end of the vector.
 *
 * Parameters:
 *  vector - Vector object
 *  element - Element to be appended to the vector
 * 
 * Return:
 *  bool: *true* if the element is added to the array, otherwise this function returns *false*.
 * 
 * Notes:
 *  -   If the vector object is configured to use an <FlContainerCleanupFn> function, it takes ownership of 
 *      the added element, which means it will release the element memory once it is removed from the vector
 *      unless the functions that remove elements from the vector say the opposite.
 * 
 * See:
 *  -   <fl_vector_new_args> to check how to configure an <FlContainerCleanupFn> function
 *  -   <fl_vector_shift> and <fl_vector_pop> to check how to remove elements from the vector
 */
bool fl_vector_add(FlVector *vector, const void *element);

/*
 * Function: fl_vector_insert
 *  Inserts an element into the vector in the specified index:
 * 
 *      -   If the index is equals to the vector's length, the element is appended
 *      -   If the index is lesser than the vector's length, all the elements stored in the 
 *          index-th position and above will be moved one position to the right
 *      -   If the index is greather than the vector's length, all the elements between the
 *          current last element and the new element to be inserted are zeroed-out
 *
 * Parameters:
 *  vector - vector object
 *  element - Element to insert into the vector
 *  index - Target position to store the element in the vector
 *
 * Return:
 *  bool: *true* if the element is inserted into the array, otherwise this function returns *false*.
 * 
 * Notes:
 *  -   If the vector object is configured to use an <FlContainerCleanupFn> function, it takes ownership of 
 *      the inserted element, which means it will release the element memory once it is removed from the vector
 *      unless the functions that remove elements from the vector say the opposite.
 * 
 * See:
 *  -   <fl_vector_new_args> to check how to configure an <FlContainerCleanupFn> function
 *  -   <fl_vector_shift> and <fl_vector_pop> to check how to remove elements from the vector
 */
bool fl_vector_insert(FlVector *vector, const void *element, size_t index);

/*
 * Function: fl_vector_ref_get
 *  Returns a pointer to the element placed in the specified *index* within the vector. If the index does not exist
 *  this function returns <NULL>. The returned pointer is a reference to the internal vector, which means it can be
 *  invalidated by a destructive operation like an internal resize or a call to a function that moves the elements
 *  within the vector. To prevent that and get the actual value, use the macro <flm_vector_get>.
 *
 * Parameters:
 *  vector - Vector where to get its _index-nth_ position
 *  index - Index of the element to retrieve
 *
 * Return:
 *  void* - Pointer to the element stored at vector's _index-nth_ position
 *
 */
void* fl_vector_ref_get(FlVector *vector, size_t index);

/*
 * Function: fl_vector_ref_first
 *  Returns a pointer to the first element within the vector. If the array contains no elements this function returns <NULL>. 
 *  The returned pointer is a reference to the internal vector, which means it can be invalidated by a destructive operation 
 *  like an internal resize or a call to a function that moves the elements within the vector. To prevent that and get the 
 *  actual value, use the macro <flm_vector_first>.
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return:
 *  void* - Pointer to the last element stored in the vector.
 */
void* fl_vector_ref_first(FlVector *vector);

/*
 * Function: fl_vector_ref_last
 *  Returns a pointer to the last element within the vector. If the array contains no elements this function returns <NULL>. 
 *  The returned pointer is a reference to the internal vector, which means it can be invalidated by a destructive operation 
 *  like an internal resize or a call to a function that moves the elements within the vector. To prevent that and get the 
 *  actual value, use the macro <flm_vector_last>.
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return:
 *  void* - Pointer to the last element stored in the vector.
 */
void* fl_vector_ref_last(FlVector *vector);

/*
 * Function: fl_vector_shift
 *  This function removes the first element of the vector, and if *dest* is not NULL, it copies
 *  the removed element to it. If *dest* is NULL and an <FlContainerCleanupFn> function was provided
 *  on the vector creation, the memory of the removed element is released with the cleanup function
 *
 * Parameters:
 *  vector - Vector object
 *  dest - If present, the destination to copy the removed element
 *
 * Return:
 *  bool - On success this function returns *true*, otherwise *false* is returned
 *
 * Notes:
 *  -   If the vector has a cleanup function and *dest* is NULL, the memory of the element is released with
 *      the provided function
 *  -   If the vector has a cleanup function and *dest* it not NULL, the caller takes ownership of the removed
 *      element 
 *  -   If the vector is empty, this function returns false and *dest* remains untouched
 */
bool fl_vector_shift(FlVector *vector, void *dest);

/*
 * Function: fl_vector_pop
 *  This function removes the last element of the vector, and if *dest* is not NULL, it copies
 *  the removed element to it. If *dest* is NULL and an <FlContainerCleanupFn> function was provided
 *  on the vector creation, the memory of the removed element is released with the cleanup function
 *
 * Parameters:
 *  vector - Vector object
 *  dest - If present, the destination to copy the removed element
 *
 * Return:
 *  bool - On success this function returns *true*, otherwise *false* is returned
 *
 * Notes:
 *  -   If the vector has a cleanup function and *dest* is NULL, the memory of the element is released with
 *      the provided function
 *  -   If the vector has a cleanup function and *dest* it not NULL, the caller takes ownership of the removed
 *      element 
 *  -   If the vector is empty, this function returns false and *dest* remains untouched
 */
bool fl_vector_pop(FlVector *vector, void *dest);

/*
 * Function: fl_vector_contains
 *  This function checks if the element is present in the vector
 *
 * Parameters:
 *  vector - Vector object
 *  elem - The element to be searched
 *
 * Return:
 *  bool - *true* if the element is present within the vector, otherwise it returns *false*
 */
bool fl_vector_contains(FlVector *vector, const void *elem);

/*
 * Function: fl_vector_concat
 *  This function takes the elements from *vector2* and appends them to the end of *vector1*
 *
 * Parameters:
 *  vector1 - Destination vector
 *  vector2 - Source vector
 *
 * Return:
 *  bool - *true* if elements of vector2 are copied at the end of vector1, otherwise this function returns *false*.
 */
bool fl_vector_concat(FlVector *vector1, FlVector *vector2);

/*
 * Function: fl_vector_merge
 *  Creates a copy of vector1, including its elements and configuration, and appends all the elements of vector2
 *  at the end of the new vector.
 *
 * Parameters:
 *  vector1 - First source vector to take the elements and configuration from
 *  vector2 - Second source vector to take the elements from
 *
 * Return:
 *  <FlVector> - New vector that contains copies of the elements in vector1 and vector2
 * 
 * Notes:
 *  -   *IMPORTANT*: Because the destination vector is a copy of vector1, it will share the <FlContainerCleanupFn>
 *      function (if present in vector1) which means that if you are working with pointers, you may run into 
 *      double-free errors. You can remove the cleanup function of a vector using the <fl_vector_cleanup_fn_set>
 *      function or you can directly use the <fl_vector_merge_args> function to provide the configuration for the
 *      destination vector.
 */
FlVector* fl_vector_merge(FlVector *vector1, FlVector *vector2);

/*
 * Function: fl_vector_merge_args
 *  Creates a new vector using the provided arguments and copies all the elements of vector1 and vector2 to it.
 *
 * Parameters:
 *  args - Arguments to configure the new vector instance
 *  vector1 - First source vector to take the elements from
 *  vector2 - Second source vector to take the elements from
 *
 * Return:
 *  <FlVector> - New vector that contains copies of the elements in vector1 and vector2
 * 
 * See:
 *  -   <fl_vector_new_args> function
 *  -   <FlVectorArgs> struct
 */
FlVector* fl_vector_merge_args(struct FlVectorArgs args, FlVector *vector1, FlVector *vector2);

/*
 * Function: fl_vector_remove
 *  Removes the element at the index-th position, and if *dest* is not NULL, this function
 *  copies the element to it. If *dest* is NULL and an <FlContainerCleanupFn> function was provided
 *  on the vector creation, the memory of the removed element is released with the cleanup function.
 *
 * Parameters:
 *  vector - Vector object
 *  index - The index of the element to be removed
 *  dest - If present, the destination to copy the removed element
 *
 * Return:
 *  bool - *true* if the element is removed, otherwise this function returns *false*.
 *
 * Notes:
 *  -   If the vector has a cleanup function and *dest* is NULL, the memory of the element is released with
 *      the provided function
 *  -   If the vector has a cleanup function and *dest* it not NULL, the caller takes ownership of the removed
 *      element 
 *  -   If the vector is empty or the index to remove does not exist, this function returns false and *dest* remains untouched
 */
bool fl_vector_remove(FlVector *vector, size_t index, void *dest);

/*
 * Function: fl_vector_free
 *  This function frees the memory used by the vector object. If the vector contains a cleanup function, it is called
 *  for each element within the array before freeing the memory of the vector object.
 * 
 * Parameter:
 *  vector - Vector object
 *
 * Return:
 *  This function does not return a value
 *
 */
void fl_vector_free(FlVector *vector);

/*
 * Function: fl_vector_to_array
 *  Creates an <FlArray> object and copies all the elements from the vector to the array object
 *
 * Parameters:
 *  vector - Vector object
 *
 * Returns:
 *  <FlArray>* - A pointer to the created array
 * 
 * Notes:
 *  -   The object returned by this function must be freed using one of the <fl_array_free>, <fl_array_free_each>, or
 *      <fl_array_free_each_pointer> functions.
 */
FlArray* fl_vector_to_array(FlVector *vector);

/*
 * Function: fl_vector_begin
 *  Returns an iterator that points to the first element in the vector.
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return: 
 *  <FlIterator>* - An iterator object that can be used to traverse the vector
 *
 * Notes:
 *  -   The object returned by this function must be freed using the <fl_iterator_free> function.
 */
FlIterator* fl_vector_begin(const FlVector *vector);

/*
 * Function: fl_vector_end
 *  Returns an iterator that points to the past-last-th element in the vector.
 *
 * Parameters:
 *  vector - Vector object
 *
 * Return: 
 *  <FlIterator>* - An iterator object that can be used to traverse the vector
 *
 * Notes:
 *  -   The object returned by this function must be freed using the <fl_iterator_free> function.
 */
FlIterator* fl_vector_end(const FlVector *vector);

#endif /* FL_VECTOR_H */
