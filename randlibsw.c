#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

//conatin software implementation of random number generator
//include randlib.h and implement the interface
//needs initialization and finalization, define an INITIALIZER and 
//FINALIZER function, using GCC's attribute constructor and destructor

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__((constructor))
static void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
__attribute__((destructor))
static void
software_rand64_fini (void)
{
  fclose (urandstream);
}

