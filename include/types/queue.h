/**
 * @file queue.h
 * @brief A queue in C
 * @date 2026-01-07
 */

#ifndef CLIB2_TYPES_QUEUE_H
#define CLIB2_TYPES_QUEUE_H

#include "../shared.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The (opaque) queue type
 */
typedef struct clib2_types_queue_s clib2_types_queue_t;

/**
 * @brief Intialize a queue
 * @param elem_size The size of each element in the queue. Is referred to later.
 * @return A pointer to the queue
 * @note If @p elem_size == 0, the function returns NULL
 */
CLIB2_SHARED_PUBLIC clib2_types_queue_t *
clib2_types_queue_init(size_t elem_size);

/**
 * @brief Free a queue
 * @param queue The queue to free
 */
CLIB2_SHARED_PUBLIC void
clib2_types_queue_free(clib2_types_queue_t *restrict *restrict queue);

/**
 * @brief Get the size of each element in a queue
 * @param queue The queue to fetch from
 * @return The size of each element in the queue
 * @note Since 0 is not a valid element size, 0 is an error value
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_queue_elemsize(clib2_types_queue_t *restrict const queue);

/**
 * @brief Enqueue an item to a queue
 * @param queue The queue to push an item into
 * @param in The bytes to copy into the queue (copies up to
 * clib2_types_queue_elemsize( \p queue ) bytes)
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_queue_enqueue(clib2_types_queue_t *restrict queue,
                          const void *restrict const in);

/**
 * @brief Dequeue the first element of a queue
 * @param queue The queue to dequeue the first element of
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_queue_dequeue(clib2_types_queue_t *restrict queue);

/**
 * @brief Peek at the first item in a queue
 * @param queue The queue to peek from
 * @param out The memory buffer to copy into. (copies up to
 * clib2_types_queue_elemsize( \p queue ) bytes)
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_queue_peek(const clib2_types_queue_t *restrict const queue,
                       void *restrict out);

/**
 * @brief Get the length (amount of elements) in a queue
 * @param queue The queue to fetch from
 * @return The length of a queue
 * @note If @p queue is NULL, the function will return 0. 0 may also be a valid
 * length, so check if @p queue is valid before running.
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_queue_len(const clib2_types_queue_t *restrict const queue);

#endif // CLIB2_TYPES_QUEUE_H
