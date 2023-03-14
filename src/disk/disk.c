#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

typedef struct _results
{
   uint32_t block_size;
   uint32_t blocks;
   double writeMBps;
   double readMBps;
   double multiwriteMBps;
   double multireadMBps;
} results_t;

results_t results[] =
{
   {    2*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {    4*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {    8*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {   16*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {   32*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {   64*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {  128*1024, 0, 0.0, 0.0, 0.0, 0.0 },
   {  256*1024, 0, 0.0, 0.0, 0.0, 0.0 },
};
size_t size = sizeof(results)/sizeof(results_t);

char filename[64] = "testfile";
char pathname[128];

double writerun(uint32_t bs, uint32_t blocks)
{
   int fd;
   uint32_t count;
   double writeMBps;
   int size;
   char* block;
   struct timeval start;
   struct timeval end;
   double startsec;
   double endsec;
   double secs;
   int written;
   int total_size = 0;
   char* ptr;

   block = malloc(bs);

   fd = creat(pathname,0666);
   if (fd < 0)
   {
      perror("could not open file:");
      exit(1);
   }

   count = blocks;
   gettimeofday(&start, 0);
   while (count--)
   {
      size = bs;
      ptr = block;
      while (size)
      {
         written = write(fd, ptr, size);
	 if (written <= 0)
	 {
            perror("Could not write to file:");
	    exit(1);
	 }
	 size -= written;
	 ptr += written;
	 total_size += written;
      }
   }
   fsync(fd);
   close(fd);
   gettimeofday(&end, 0);
   startsec = start.tv_sec + start.tv_usec/1000000.0;
   endsec   = end.tv_sec + end.tv_usec/1000000.0;
   secs     = endsec - startsec;
   writeMBps     = blocks/1024.0*bs/1024.0;
   writeMBps    /= secs;
   free(block);
   return writeMBps;
}

double readrun(uint32_t bs, uint32_t blocks)
{
   int fd;
   uint32_t count;
   int size;
   char* block;
   struct timeval start;
   struct timeval end;
   double startsec;
   double endsec;
   double secs;
   int bytes;
   char* ptr;
   double readMBps;
   int total_size = 0;

   block = malloc(bs);

   fd = open(pathname,O_RDONLY);
   if (fd < 0)
   {
      perror("could not open file:");
      exit(1);
   }

   count = blocks;
   gettimeofday(&start, 0);
   while (count--)
   {
      size = bs;
      ptr = block;
      while (size)
      {
         bytes = read(fd, ptr, size);
	 if (bytes < 0)
	 {
            perror("Could not read from file:");
	    exit(1);
	 }
	 if (bytes == 0)
	    break;
	 size -= bytes;
	 ptr += bytes;
	 total_size += bytes;
      }
  }
   fsync(fd);
   close(fd);
   gettimeofday(&end, 0);
   startsec = start.tv_sec + start.tv_usec/1000000.0;
   endsec   = end.tv_sec + end.tv_usec/1000000.0;
   secs = endsec - startsec;
   if (blocks*bs != total_size)
      fprintf(stderr,"read %d. should have read %d\n",total_size, blocks*bs);
   readMBps = blocks/1024.0*bs/1024.0;
   readMBps /= secs;
   free(block);
   return readMBps;
}

double multiwriterun(uint32_t bs, uint32_t blocks)
{
   int fd;
   uint32_t count;
   double writeMBps;
   int size;
   char* block;
   struct timeval start;
   struct timeval end;
   double startsec;
   double endsec;
   double secs;
   int written;
   int total_size = 0;
   char* ptr;

   block = malloc(bs);


   count = blocks;
   gettimeofday(&start, 0);
   while (count--)
   {
      sprintf(pathname,"testfile%d", count);
      fd = creat(pathname,0666);
      if (fd < 0)
      {
         perror("could not open file:");
         exit(1);
      }
      size = bs;
      ptr = block;
      while (size)
      {
         written = write(fd, ptr, size);
	 if (written <= 0)
	 {
            perror("Could not write to file:");
	    exit(1);
	 }
	 size -= written;
	 ptr += written;
	 total_size += written;
      }
      fsync(fd);
      close(fd);
   }
   gettimeofday(&end, 0);
   startsec = start.tv_sec + start.tv_usec/1000000.0;
   endsec   = end.tv_sec + end.tv_usec/1000000.0;
   secs     = endsec - startsec;
   writeMBps     = blocks/1024.0*bs/1024.0;
   writeMBps    /= secs;
   free(block);
   return writeMBps;
}

double multireadrun(uint32_t bs, uint32_t blocks)
{
   int fd;
   uint32_t count;
   int size;
   char* block;
   struct timeval start;
   struct timeval end;
   double startsec;
   double endsec;
   double secs;
   int bytes;
   char* ptr;
   double readMBps;
   int total_size = 0;

   block = malloc(bs);

   count = blocks;
   gettimeofday(&start, 0);
   while (count--)
   {
      sprintf(pathname,"testfile%d", count);
      fd = open(pathname,O_RDONLY);
      if (fd < 0)
      {
         perror("could not open file:");
         exit(1);
      }

      size = bs;
      ptr = block;
      while (size)
      {
         bytes = read(fd, ptr, size);
	 if (bytes < 0)
	 {
            perror("Could not read from file:");
	    exit(1);
	 }
	 if (bytes == 0)
	    break;
	 size -= bytes;
	 ptr += bytes;
	 total_size += bytes;
      }
     fsync(fd);
     close(fd);
   }
   gettimeofday(&end, 0);
   startsec = start.tv_sec + start.tv_usec/1000000.0;
   endsec   = end.tv_sec + end.tv_usec/1000000.0;
   secs = endsec - startsec;
   if (blocks*bs != total_size)
      fprintf(stderr,"read %d. should have read %d\n",total_size, blocks*bs);
   readMBps = blocks/1024.0*bs/1024.0;
   readMBps /= secs;
   free(block);
   return readMBps;
}

int main(int argc, char*argv[])
{
   uint64_t ram;
   char *destdir;
   char *filesize;
   int i;
   int tw = 0;
   int tr = 0;

   if (argc < 3)
   {
      printf("usage: %s <target dir> <disk size>\n", argv[0]);
      exit(0);
   }
   destdir = argv[1];
   filesize = argv[2];
   sprintf(pathname,"%s/%s",destdir,filename);
   printf("writing to %s\n", pathname);
   i = strlen(filesize);
   ram = atoi(filesize);

   switch (filesize[i-1])
   {
   case 'k':
   case 'K':
	   ram *= 1024;
	   break;
   case 'm':
   case 'M':
	   ram *= 1024*1024;
	   break;
   case 'g':
   case 'G':
	   ram *= 1024*1024*1024;
	   break;
   default:
	   break;
   }

   for (i = 0; i < size; i++)
   {
      results[i].blocks = ram / results[i].block_size;
   }
   for (i = 0; i < size; i++)
   {
      results[i].writeMBps = writerun(results[i].block_size,results[i].blocks);
      results[i].readMBps = readrun(results[i].block_size,results[i].blocks);
   }
   printf("Block Size\tBlocks\tWrite MB/s\tRead MB/s\n");
   for (i = 0; i < size; i++)
   {
      printf("%6u\t\t%6u\t%6.0f\t\t%6.0f\n",
		      results[i].block_size,
		      results[i].blocks,
		      results[i].writeMBps,
		      results[i].readMBps
		      );
      tw += results[i].writeMBps;
      tr += results[i].readMBps;
   }
   tw /= size;
   tr /= size;
   printf("\t Average Read %u MB/s\tWrite %u MB/s\n",tr,tw);
   return 0;
}
