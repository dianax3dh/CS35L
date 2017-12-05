#include "randcpuid.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdbool.h>

//contain main program 
//include randcpuid.h, but not randlib.h

//Depending on whether randcpuid reports that the hardware supports the RDRAND instruction, this main program 
//should dynamically link the hardware-oriented or software-oriented implementation of randlib, doing the 
//dynamic linking via dlopen and dlsym.

//call dlclose to clean up before exiting

//if any function called by the main program fails, the main program should report an error and exit 
//with nonzero status

/* Main program, which outputs N bytes of random data.  */
static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
  return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  //void (*initialize) (void);
  void *dLink = NULL; 
  char *error; 
  unsigned long long (*rand64) (void);
 // void (*finalize) (void);
  if (rdrand_supported ())
    { //open hardware
      dLink = dlopen("randlibhw.so", RTLD_LAZY); //or RTLD_ NOW
      if (dLink == NULL)
      {
        fprintf(stderr, "Error in opening: %s\n", dlerror());
        exit(1); 
      }
      rand64 = dlsym(dLink, "hardware_rand64"); 
      error = dlerror(); 
      if (error != NULL)
      {
        printf("Error in dlysm: %s\n", error);
        exit(1); 
      }
      // initialize = hardware_rand64_init;
      // rand64 = hardware_rand64;
      // finalize = hardware_rand64_fini;
    }
  else
    {
      //open software
      dLink = dlopen("randlibsw.so", RTLD_LAZY); 
      if (dLink == NULL)
      {
        fprintf(stderr, "Error in opening: %s\n", dlerror());
        exit(1); 
      }
      rand64 = dlsym(dLink, "software_rand64"); 
      error = dlerror(); 
      if (error != NULL)
      {
        printf("Error in dlysm: %s\n", error);
      }
        
      // initialize = software_rand64_init;
      // rand64 = software_rand64;
      // finalize = software_rand64_fini;
    }

  //initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;
//fwrite(&x, 1, outbytes, stdout) != outbytes
  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes)) /*fwrite writes data from array
      pointed to by "x" to "stdout"*/ 
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      //finalize ();
      return 1;
    }

  //finalize ();
    dlclose(dLink); 
  return 0;
}
