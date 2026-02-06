#include "../../include/types/option.h"
#include <stdlib.h>

clib2_types_option_t clib2_types_option_some(void *val) {
  return (clib2_types_option_t){.is_some = true, .val = val};
}

clib2_types_option_t clib2_types_option_none(void) {
  return (clib2_types_option_t){.is_some = false, .val = NULL};
}

void *clib2_types_option_unwrap(clib2_types_option_t opt) {
  if (!(opt.is_some))
    abort();
  return opt.val;
}
