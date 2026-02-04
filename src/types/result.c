#include "../../include/types/result.h"

clib2_types_result_t clib2_types_result_ok(void *success) {
  // NOTE: This implicitly sets .err to success as well, but that is documented
  return (clib2_types_result_t){.ok = true, .success = success};
}

clib2_types_result_t clib2_types_result_err(void *err) {
  // NOTE: This implicitly sets .success to err as well, but that is documented
  return (clib2_types_result_t){.ok = false, .err = err};
}
