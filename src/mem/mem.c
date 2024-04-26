#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>




int main(int argc, char **argv)
{
    struct timeval	start, stop;

    size_t mem_size = 1024L*1024L*1024L;
    char* src = malloc(mem_size);
    char* dst = malloc(mem_size);

    for (size_t i = 0; i < mem_size; i++) {
        src[i] = i % 256;
    }

    (void)gettimeofday(&start, (struct timezone *)0);
    memcpy(src, dst, mem_size);
    (void)gettimeofday(&stop, (struct timezone *)0);

    size_t utime = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);

    free(src);
    free(dst);

    printf("%lu\n", mem_size*1000000/utime/1024/1024);  

}
