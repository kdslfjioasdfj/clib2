#include "../../include/types/result.h"
#include "../../include/utils/str/compatibility.h" // Not testing this, just here for example
#include <stdio.h>
#include <stdlib.h>

// Simulated function that returns a Result
clib2_types_result_t read_file(const char *path) {
  FILE *f = fopen(path, "r");
  if (!f) {
    // return an error string pointer
    return clib2_types_result_err(
        clib2_utils_str_compatibility_strdup("Failed to open file"));
  }

  // Allocate buffer and read first line
  char *line = malloc(256);
  if (!fgets(line, 256, f)) {
    free(line);
    fclose(f);
    return clib2_types_result_err(
        clib2_utils_str_compatibility_strdup("Failed to read file"));
  }

  fclose(f);
  return clib2_types_result_ok(line);
}

int main(void) {
  const char *filename = "example.txt";

  clib2_types_result_t result = read_file(filename);

  if (result.ok) {
    printf("First line: %s", (char *)result.success);
    free(result.success); // user owns it
  } else {
    printf("Error: %s\n", (char *)result.err);
    free(result.err); // user owns it
  }

  return 0;
}
