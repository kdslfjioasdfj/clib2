#include "../../include/types/queue.h"
#include <stdio.h>

#define MAIN_ERR 1

struct task {
  void (*task)(int *arg);
};

// Here are some EXAMPLE tasks
void do_nothing(int *ignore) {
  // Do nothing
  (void)ignore; // This does nothing at RT, it just makes the compiler happy
}

void do_stuff_1(int *param) {
  if (!param)
    printf("Got bad input!\n");
  else
    printf("Number passed: %d\n", *param);
}

void do_stuff_2(int *param) {
  if (!param)
    printf("Got bad input!\n");
  else
    printf("Pointer passed: %p\n", (void *)param);
}

int main(void) {
  clib2_types_queue_t *task_queue = clib2_types_queue_init(sizeof(struct task));
  if (!task_queue)
    return MAIN_ERR;

  struct task task_to_enqueue = {.task = do_nothing};

  bool success = clib2_types_queue_enqueue(task_queue, &task_to_enqueue);
  if (!success) {
    clib2_types_queue_free(&task_queue);
    return MAIN_ERR;
  }

  task_to_enqueue.task = do_stuff_1;

  // This puts in the NEW task_to_enqueue, not the old one!
  success = clib2_types_queue_enqueue(task_queue, &task_to_enqueue);
  if (!success) {
    clib2_types_queue_free(&task_queue);
    return MAIN_ERR;
  }

  task_to_enqueue.task = do_stuff_2;

  success = clib2_types_queue_enqueue(task_queue, &task_to_enqueue);
  if (!success) {
    clib2_types_queue_free(&task_queue);
    return MAIN_ERR;
  }

  // Now, we run our tasks:
  int param = 0;
  struct task task_next;
  for (int i = 0; i < 3; i++) {
    success = clib2_types_queue_peek(task_queue, &task_next);
    if (!success) {
      clib2_types_queue_free(&task_queue);
      return MAIN_ERR;
    } else {
      task_next.task(&param);
      clib2_types_queue_dequeue(task_queue);
    }
  }

  clib2_types_queue_free(&task_queue);
  return 0;
}
