#use static linking to combine randmain.o and randcpuid.o into a 
#single program executable randmain
#use dynamic linking via dlsym as described above. 
#randmain.mk should link randmain with the options “-ldl -Wl,-rpath=$PWD”.
randmain: randcpuid.o randmain.o 
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randcpuid.o randmain.o -o randmain
#use dynamic linking as usual to link the C library and any other 
#necessary system-supplied files before its main function is calle.
#compile randlibhw.c and randlibsw.c with the -fPIC options as well 
#as the other GCC options already used.
#build shared object files randlibhw.so and randlibsw.so by linking the
#corresponding object modules with the -shared optionnd
randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -shared -fPIC -o randlibhw.so

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -shared -fPIC -o randlibsw.so




