#include "grep_file.h"
#include "grep_parcer.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
  GrepProperties *our_properties = calloc(1, sizeof(GrepProperties));
  if (our_properties == NULL) {
    exit(EXIT_FAILURE);
  }
  get_grep_properties(our_properties, argc - 1, argv);
  if (grep_on_file(our_properties)) {
    free(our_properties);
    exit(EXIT_SUCCESS);
  }
  free(our_properties);
  exit(EXIT_FAILURE);
}
