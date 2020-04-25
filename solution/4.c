#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define LENGTH (32*1024)
#define PROTECTION (PROT_READ | PROT_WRITE)

#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40
#endif

/* Only ia64 requires this */
#ifdef __ia64__
#define ADDR (void *)(0x8000000000000000UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_FIXED)
#else
#define ADDR (void *)(0x0UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB)
#endif

#if defined(__i386__)
static __inline__ unsigned long long rdtsc(void)
{
unsigned long long int x;
__asm__ volatile("xorl %%eax,%%eax\n cpuid \n" ::: "%eax", "%ebx", 
	"%ecx", "%edx"); // flush pipeline
__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
__asm__ volatile("xorl %%eax,%%eax\n cpuid \n" ::: "%eax", "%ebx", 
	"%ecx", "%edx"); // flush pipeline
return x;
}
#elif defined(__x86_64__)
static __inline__ unsigned long long rdtsc(void)
{
unsigned hi, lo;
__asm__ __volatile__ ("xorl %%eax,%%eax\n cpuid \n" ::: "%eax", "%ebx", "%ecx", "%edx"); // flush pipeline
__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
__asm__ __volatile__ ("xorl %%eax,%%eax\n cpuid \n" ::: "%eax", "%ebx", "%ecx", "%edx"); // flush pipeline
return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif


int main(void)
{
	int *addr;
	long long int t1, t2;
	int x, y, z;

	addr = mmap(ADDR, LENGTH, PROTECTION, FLAGS, 0, 0);
	
	t1 = rdtsc();
	x = addr[20];
	y = addr[50];
	z = addr[80];
	t2 = rdtsc();
	printf("%lld\n", t2-t1);

	t1 = rdtsc();
	x = addr[1500];
	y = addr[3000];
	z = addr[4500];
	t2 = rdtsc();
	printf("%lld\n", t2-t1);

	t1 = rdtsc();
	x = addr[1500];
	y = addr[3000];
	z = addr[4500];
	t2 = rdtsc();
	printf("%lld\n", t2-t1);

	munmap(addr, LENGTH);

	return 0;
}
