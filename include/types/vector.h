/**
 * @file vector.h
 * @brief A vector array in C
 * @note In the documentation, both "array" and "vector" mean "dynamic array"
 * @date 2025-12-26
 */

#ifndef CLIB2_TYPES_VECTOR_H
#define CLIB2_TYPES_VECTOR_H

#include "../shared.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The vector type: Is opaque
 */
typedef struct clib2_types_vector_s clib2_types_vector_t;

/**
 * @brief The result of a comparison between 2 elements in a vector. Used for
 * sorting the vector.
 */
typedef enum clib2_types_vector_cmpres_e {
  CLIB2_TYPES_VECTOR_CMPRES_EQ = 0, ///< Equality between 2 elements
  CLIB2_TYPES_VECTOR_CMPRES_GT = 1, ///< The first element is greater
  CLIB2_TYPES_VECTOR_CMPRES_LT = 2, ///< The first element is lesser
} clib2_types_vector_cmpres_t;

/**
 * @brief A comparison between 2 items in an array
 * @param a The first operand
 * @param b The second operand
 * @details This function is used to compare to elements of an array
 *          It returns CLIB2_TYPES_VECTOR_CMPRES_EQ if both are equal
 *          It returns CLIB2_TYPES_VECTOR_CMPRES_GT if @p a is greater
 *          It returns CLIB2_TYPES_VECTOR_CMPRES_LT if @p a is lesser
 */
typedef clib2_types_vector_cmpres_t (*clib2_types_vector_cmp_t)(
    const void *const a, const void *const b);

/**
 * @brief Initialize a vector
 * @param elem_size The size of each element
 * @param initial_len The initial amount of elements to guarantee existence of.
 * Length may be shown as 0.
 * @return The pointer to the array
 * @note initial_len can not be 0
 */
CLIB2_SHARED_PUBLIC clib2_types_vector_t *
clib2_types_vector_init(size_t elem_size, size_t initial_len);

/**
 * @brief Free a vector
 * @param vec The vector to free
 */
CLIB2_SHARED_PUBLIC void
clib2_types_vector_free(clib2_types_vector_t *restrict *restrict vec);

/**
 * @brief Get the size of each element in an array
 * @param vec The vector the check on
 * @return The size of each element in the specific vector
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_vector_elemsize(const clib2_types_vector_t *restrict const vec);

/**
 * @brief Set an element in a vector
 * @param vec The vector to set into
 * @param data The data to copy bytes from. Copies in
 * clib2_types_vector_elemsize( @p vec ) bytes from @p data into the array
 * @param idx The index to set to
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_vector_set(clib2_types_vector_t *restrict vec,
                       const void *restrict const data, size_t idx);

/**
 * @brief Get an element in a vector
 * @param vec The vector to fetch from
 * @param out The output buffer to copy the data into. Copies up to
 * clib2_types_vector_elemsize( @p vec ) bytes into @p out
 * @param idx The index to read
 * @return The success of the operation
 * @warning If the memory at @p idx was not set before, the function reads into
 * uninitialized memory. To set the memory, set it with clib2_types_vector_set.
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_vector_get(const clib2_types_vector_t *restrict const vec,
                       void *restrict out, size_t idx);

/**
 * @brief Get the length (total element count) of a vector
 * @param vec The vector to fetch from
 * @return The length of the vector
 * @note 0 is ambiguous. If @p vec == NULL, the function returns 0.
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_vector_len(const clib2_types_vector_t *restrict const vec);

/**
 * @brief Push an element onto an array
 * @param vec The vector to push onto
 * @param in The data to copy into the new element. Leaving it as NULL leaves
 * the data uninitialized. Copies up to clib2_types_vector_elemsize( @p vec )
 * bytes from @p in
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_vector_push(clib2_types_vector_t *restrict vec,
                        const void *const restrict in);

/**
 * @brief Resize the vector so it has exactly @p size elements
 * @param vec The vector to resize
 * @param size The amount of accessible elements to have.
 * If it is higher, it allocates new elements and leaves them uninitialized.
 * If it is lower, it decreases the amount of elements (may remove existing
 * elements without notice).
 * If it is the same, it does nothing.
 * If it is 0, it returns an error and does nothing.
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_vector_resize(clib2_types_vector_t *restrict vec, size_t size);

/**
 * @brief Sort a vector into ascending order
 * @param vec The vector to sort
 * @param cmp The comparison callback
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_vector_sort(clib2_types_vector_t *restrict vec,
                        clib2_types_vector_cmp_t cmp);

#endif // CLIB2_TYPES_VECTOR_H
