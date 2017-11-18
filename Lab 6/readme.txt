After getting the srt folder onto my seasnet, I cd into 
the folder and ran 'make clean check' as mentioned in the 
assignment instructions. 

The output:
rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm
time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m41.568s
user 0m41.563s
sys  0m0.001s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp
./srt: Multithreading is not supported yet.

real   0m0.002s
user   0m0.001s
sys    0m0.000s
make: *** [2-test.ppm] Error 1

Looking through 'main.c', I notice in the main function it checks for 'nthreads' and
outputs an error; I commented out 'if (nthreads !=1)', because I am going to implement
multithreading, so multithreading will be supported. 
I also noticed that there are nested for loops within the main function that seems
to be calculating something for every pixel of the width and height of the scene.
To have a function to pass to pthread_create, I created a 'threading' function
called *thread_func(void *arg) and moved the entire check of calculations into 
this function. In order to use scene in the thread_func function, I made it a 
global variable. I also created a variable to hold the values that are the result
of the pixel colors calculations, called imageColors[width][height][3].

I first tried to create a struct that would contain data for the pixels, but I 
kept getting an error 139 and segmentation fault at 1-test.ppm and implementation
of the struct was not completely correct. 
  
In my second attempt, in my threadFunc function, I created an integer variable, 
threadN that is assigned the copy of the value 'arg' is pointing to, which is 
the number of threads passed in; (int*)arg casts arg to become and integar 
pointer, because of the 'void' in the paremeter, it is necessary to cast in 
order to access data, and * in front of it dereferences it. threadN will be 
used as the starting point for px in the for loop. The first for loop is 
modified to start at threadN and continue until width, but add itself with 
nthreads each time. 
At the end of the most inner loop, imageColors is assigned the scaled_color(s) to
be stored. 
I used malloc on the thread ids and refered to their address when using pthread_
create. 

The second problem I ran into is make: *** [check] Error 1 proceeded by various
coordinates, only when running 8 threads; it seems to work for 2 and 4 threads. 
I think as I was editing the code, I accidently deleted small important parts, 
so I saved what I had done on the side and opened up a new main.c to compare and 
redo my mistake. 

In the end, the multithreading worked and the result is:
rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm -lpthread
time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m43.310s
user 0m43.303s
sys  0m0.001s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real 0m22.520s
user 0m44.932s
sys  0m0.007s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real 0m11.261s
user 0m44.729s
sys  0m0.001s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real 0m5.779s
user 0m44.724s
sys  0m0.002s
mv 8-test.ppm.tmp 8-test.ppm
for file in 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm; do \
  diff -u baseline.ppm $file || exit; \
done

This shows that with more threads, the process is much faster. My implementation
of SRT makes use of nthreads and usings pthread multithreading to thread more than 
1 number of threads, making the program run faster and shortening the time. With 2 
threads the time is about half the time of 1 and as the number of threads increases
in multiple of two's the time continues to be cut in approximately half. 
