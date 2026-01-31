#include "../../include/types/vector.h"
#include <stdint.h>
#include <stdio.h>

/* Comparison function for sorting scores ascending */
static clib2_types_vector_cmpres_t cmp_scores(const void *a, const void *b) {
  int32_t x = *(const int32_t *)a;
  int32_t y = *(const int32_t *)b;

  if (x == y)
    return CLIB2_TYPES_VECTOR_CMPRES_EQ;
  return (x > y) ? CLIB2_TYPES_VECTOR_CMPRES_GT : CLIB2_TYPES_VECTOR_CMPRES_LT;
}

int main(void) {
  /* Create a vector of int32_t with initial capacity of 2 */
  clib2_types_vector_t *scores = clib2_types_vector_init(sizeof(int32_t), 2);
  if (!scores)
    return 1;

  /* Simulate adding scores dynamically */
  int32_t new_scores[] = {75, 88, 92, 60, 100};
  for (size_t i = 0; i < sizeof(new_scores) / sizeof(new_scores[0]); i++) {
    bool success = clib2_types_vector_push(scores, &new_scores[i]);
    if (!success) {
      clib2_types_vector_free(&scores);
      return 1;
    }
  }

  /* Sort the scores */
  bool success = clib2_types_vector_sort(scores, cmp_scores);
  if (!success) {
    clib2_types_vector_free(&scores);
    return 1;
  }

  /* Print sorted scores */
  printf("Sorted scores: ");
  for (size_t i = 0; i < clib2_types_vector_len(scores); i++) {
    int32_t score;
    success = clib2_types_vector_get(scores, &score, i);
    if (!success) {
      clib2_types_vector_free(&scores);
      return 1;
    }
    printf("%d ", score);
  }
  printf("\n");

  clib2_types_vector_free(&scores);
  return 0;
}
