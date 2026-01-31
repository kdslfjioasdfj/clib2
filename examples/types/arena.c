#include "../../include/types/arena.h"
#include <ctype.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char *text;
} token_t;

/* Advances *p to the next word.
   Returns 1 if a word was found, 0 otherwise. */
static int next_word(const char **p, const char **start, size_t *len) {
  const char *s = *p;

  while (isspace((unsigned char)*s))
    s++;

  if (!*s)
    return 0;

  *start = s;

  while (*s && !isspace((unsigned char)*s))
    s++;

  *len = (size_t)(s - *start);
  *p = s;
  return 1;
}

static token_t *make_token(clib2_types_arena_t *arena, const char *start,
                           size_t len) {
  token_t *tok = clib2_types_arena_alloc(arena, sizeof *tok);
  tok->text = clib2_types_arena_alloc(arena, len + 1);

  memcpy(tok->text, start, len);
  tok->text[len] = '\0';
  return tok;
}

static size_t tokenize(const char *input, clib2_types_arena_t *arena,
                       token_t **out, size_t max_tokens) {
  size_t count = 0;

  while (count < max_tokens) {
    const char *start;
    size_t len;

    if (!next_word(&input, &start, &len))
      break;

    out[count++] = make_token(arena, start, len);
  }

  return count;
}

int main(void) {
  const char *input = "load fast unsafe mode";

  clib2_types_arena_t *arena =
      clib2_types_arena_init(1024, alignof(max_align_t), false);

  token_t *tokens[16];
  size_t ntokens = tokenize(input, arena, tokens, 16);

  for (size_t i = 0; i < ntokens; i++)
    printf("token[%zu] = '%s'\n", i, tokens[i]->text);

  clib2_types_arena_destroy(&arena);
}
