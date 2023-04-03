#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

#define MAXMEM (512*1024*1024)

#define MAXRESULTS 32

#define TYPE    int
#define KB	(1024.)
#define MB	(KB*KB)

#define CACHE_ALIGN	128

typedef struct _buffer {
	size_t		nbytes;
	TYPE           *src;
	TYPE           *dst;
	TYPE           *origsrc;
	TYPE           *origdst;
}		buffer_t;

typedef struct _results {
	uint32_t	secs;
	uint32_t    kb;
	uint32_t	mbs;
}		results_t;


struct timeval	start, stop;

results_t	results[MAXRESULTS];


void init(buffer_t * buffer)
{
	buffer->origsrc = (TYPE *) valloc(2 * buffer->nbytes - 1);
	if (!buffer->origsrc) {
		perror("malloc");
		exit(1);
	}
	buffer->origdst = (TYPE *) valloc(buffer->nbytes + 2 * CACHE_ALIGN);
	if (!buffer->origdst) {
		perror("malloc");
		exit(1);
	}
	buffer->src = buffer->origsrc;
	buffer->dst = buffer->origdst;
}

void cleanup(buffer_t * buffer)
{
	free(buffer->origsrc);
	free(buffer->origdst);
}

uint64_t getoverhead(int count)
{
	uint64_t	overhead;
	int		i;

	(void)gettimeofday(&start, (struct timezone *)0);
	while (count-- > 0) {
		i += 1;
	}
	(void)gettimeofday(&stop, (struct timezone *)0);
	overhead = (stop.tv_sec - start.tv_sec) * 1000000.0 + (stop.tv_usec - start.tv_usec);
	return overhead;
}


int main(int argc, char **argv)
{
	int		reps = 200000000;
	size_t		nbytes;
	int		c;
	buffer_t	buffer;
	uint64_t	utime;
	int		i;
	int		index;
	int		l1        , l2, l3, mem;
	uint64_t		secs;
	uint64_t		kb;
	uint64_t		mbs;
	uint64_t	overhead;

	overhead = getoverhead(reps);
	printf("overhead is %" PRIu64 " usecs\n", overhead);
	index = 0;
	for (i = 512; i <= MAXMEM; i *= 2) {
		nbytes = buffer.nbytes = i - 1;
		init(&buffer);
		(void)gettimeofday(&start, (struct timezone *)0);
		{
			register TYPE  *p = buffer.src;
			register TYPE  *dst = buffer.dst;
			register size_t	n = buffer.nbytes;
			int		count = reps;

			while (count-- > 0) {
				memcpy(p, dst, n);
			}
		}
		(void)gettimeofday(&stop, (struct timezone *)0);
		utime = (stop.tv_sec - start.tv_sec) * 1000000.0 + (stop.tv_usec - start.tv_usec);
		utime -= overhead;
		cleanup(&buffer);
		secs = (utime / (double)reps) / 1000000.0;
		kb = (nbytes + 1) / KB;
		kb *= 2;
		mbs = (nbytes / MB) / secs;
#if 1
		printf("%"PRIu64"\t", kb);
		printf("%"PRIu64"\n", mbs);
#endif
		if (index < MAXRESULTS) {
			results[index].secs = (utime / (double)reps) / 1000000.0;
			results[index].kb = (nbytes + 1) / KB;
			results[index].kb *= 2;
			results[index].mbs = (nbytes / MB) / results[index].secs;
			index++;
		}
		reps /= 2;
	}

	l1 = l2 = l3 = mem = -1;

	for (i = 1; i < index; i++) {
		if ((results[i - 1].mbs / results[i].mbs) > 1.3) {
			if (l1 == -1)
				l1 = i - 1;
			else if (l2 == -1)
				l2 = i - 1;
			else if (l3 == -1)
				l3 = i - 1;
		}
	}
	mem = index - 1;

	printf("Memory\t\tKBytes\t   MB/s\n");
	if (l1 > 0) {
		printf("L1 Cache:\t");
		printf("%u\t", results[l1].kb);
		printf("%u\n", results[l1].mbs);
	}
	if (l2 > 0) {
		printf("L2 Cache:\t");
		printf("%u\t", results[l2].kb);
		printf("%u\n", results[l2].mbs);
	}
	if (l3 > 0) {
		printf("L3 Cache:\t");
		printf("%u\t", results[l2].kb);
		printf("%u\n", results[l2].mbs);
	}
	if (mem > 0) {
		printf("RAM:\t\t");
		printf("%u\t", results[mem].kb);
		printf("%u\n", results[mem].mbs);
	}
	return (0);
}
