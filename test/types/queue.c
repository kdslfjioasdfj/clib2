#include "../../include/types/queue.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void test_basics(void) {
  // Initialize test
  clib2_types_queue_t *q = clib2_types_queue_init(sizeof(int));
  assert(q != NULL);

  // Add first value
  int buf = 0;
  clib2_types_queue_enqueue(q, &buf);

  // Assert it
  int test1;
  clib2_types_queue_peek(q, &test1);
  assert(test1 == 0);

  // Push some more values
  buf = 14;
  clib2_types_queue_enqueue(q, &buf);
  buf = 67;
  clib2_types_queue_enqueue(q, &buf);
  buf = 0xFFE;
  clib2_types_queue_enqueue(q, &buf);
  // The queue is now {0, 14, 67, 0xFFE} in that order
  size_t qlen = clib2_types_queue_len(q);
  assert(qlen == 4);

  // Assert in that order
  int testbuf;
  clib2_types_queue_peek(q, &testbuf);
  clib2_types_queue_dequeue(q);
  assert(testbuf == 0);
  clib2_types_queue_peek(q, &testbuf);
  clib2_types_queue_dequeue(q);
  assert(testbuf == 14);
  clib2_types_queue_peek(q, &testbuf);
  clib2_types_queue_dequeue(q);
  assert(testbuf == 67);
  clib2_types_queue_peek(q, &testbuf);
  clib2_types_queue_dequeue(q);
  assert(testbuf == 0xFFE);

  // No more elements left in queue
  int trashbuf = 0;
  bool peek = clib2_types_queue_peek(q, &trashbuf);
  assert(!peek);
  size_t emptylen = clib2_types_queue_len(q);
  assert(emptylen == 0);

  size_t qelemsz = clib2_types_queue_elemsize(q);
  assert(qelemsz == sizeof(int));

  clib2_types_queue_free(&q);
}

int main(void) {
  test_basics();
  puts("Passed test[s]!");
  return 0;
}
