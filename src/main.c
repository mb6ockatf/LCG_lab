#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdarg.h>
#define ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER 6

char str[10000];
FILE *input_fd;
bool is_successful = true;
uint8_t i;

void
argument_parser (uint8_t values_count, ...)
{
  va_list values;
  uint64_t *arg_pointer[ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER];
  memset (arg_pointer, 0,
	  ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER * sizeof (*arg_pointer));
  char *identifiers[ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER];
  memset (identifiers, 0,
	  ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER * sizeof (*identifiers));
  uint16_t identifiers_sizes[ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER];
  memset (identifiers_sizes, 0,
	  ARGUMENT_PARSER_MAX_ARGUMENTS_NUMBER * sizeof (*identifiers_sizes));
  va_start (values, values_count);
  uint8_t sections_border = values_count / 3;
  for (i = 0; i < sections_border; i++)
    arg_pointer[i] = va_arg (values, uint64_t *);
  for (i = 0; i < sections_border; i++)
    identifiers[i] = va_arg (values, char *);
  for (i = 0; i < sections_border; i++)
    identifiers_sizes[i] = va_arg (values, int);
  va_end (values);
  char *argument_p;
  i = 0;
  while (arg_pointer[i] != 0)
    {
      if ((argument_p = strstr (str, identifiers[i])) == NULL)
	{
	  perror ("Required argument not found");
	  is_successful = false;
	  return;
	}
      if (sscanf (argument_p + identifiers_sizes[i],
		  "%" SCNu64, arg_pointer[i]) != 1)
	{
	  perror ("Failed to parse argument");
	  is_successful = false;
	  return;
	}
      i++;
    }
}

void
get_c ()
{
  uint64_t cmin, cmax, m;
  argument_parser (9, &cmin, &cmax, &m, "cmin=", "cmax=", "m=", 5, 5, 2);
  if (!is_successful)
    return;
}

void
get_m ()
{
  uint64_t m;
  argument_parser (3, &m, "m=", 2);
  if (!is_successful)
    return;
}

void
lcg ()
{
  uint64_t a, x0, c, m, n;
  argument_parser (15, &a, &x0, &c, &m, &n, "a=", "x0=", "c=", "m=", "n=", 2,
		   3, 2, 2, 2);
  if (!is_successful)
    return;
}

void
test ()
{
}

void
error ()
{
  FILE *output_fd = fopen ("output.txt", "a");
  fputs ("incorrect command", output_fd);
  fclose (output_fd);
}

int
main ()
{
  input_fd = fopen ("input.txt", "r");
  if (input_fd == NULL)
    {
      perror ("Unable to open file");
      return ENOENT;
    }
  fgets (str, 10000, input_fd);
  if (strstr (str, "get_c") != NULL)
    get_c ();
  else if (strstr (str, "get_m") != NULL)
    get_m ();
  else if (strstr (str, "lcg") != NULL)
    lcg ();
  else if (strstr (str, "test") != NULL)
    test ();
  else
    error ();
  fclose (input_fd);
  if (is_successful)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
