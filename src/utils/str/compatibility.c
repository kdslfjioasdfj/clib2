#include "../../../include/utils/str/compatibility.h"
#include <stdlib.h>
#include <string.h>

char *clib2_utils_str_compatibility_strdup(const char *restrict const str) {
  if (!str)
    return NULL;
  size_t len = ((strlen(str) + 1) * sizeof(char));
  char *chptr = malloc(len);
  if (!chptr)
    return NULL;
  /*
  I chose to use memcpy because strcpy copies every byte until it sees a
  NULL termination character ('\0', 0). This introduces a new branch every
  byte copied; this can stall pipelines when the CPU mispredicts branches
  and this costs tens, or in some cases, up to hundreds of cycles. memcpy,
  however, does not do this and copies a specific amount of bytes with no
  such branching and is hence faster.
  */
  memcpy(chptr, str, len);
  return chptr;
}

size_t clib2_utils_str_compatibility_strlcpy(char *restrict dest,
                                             const char *restrict const src,
                                             size_t dstsz) {
  if (!src) {
    if (dest && dstsz > 0)
      dest[0] = '\0'; // null-terminate destination if possible
    return 0;         // treat NULL source as empty string
  }

  size_t src_len = strlen(src);

  if (dstsz > 0) {
    size_t copy_len = (src_len >= dstsz) ? dstsz - 1 : src_len;
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0'; // always null-terminate
  }

  return src_len;
}
