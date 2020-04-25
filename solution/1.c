#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h> 
#include <sys/wait.h>
#include <string.h>

static int a[8192];

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

int main()
{
	long long int t1, t2;
	int x, y, z;

	t1 = rdtsc();
	x = a[20];
	y = a[50];
	z = a[80];
	t2 = rdtsc();
	
	printf("%lld\n", t2-t1);

	t1 = rdtsc();
	x = a[1500];
	y = a[3000];
	z = a[4500];
	t2 = rdtsc();
	
	printf("%lld\n", t2-t1);

	t1 = rdtsc();
	x = a[1500];
	y = a[3000];
	z = a[4500];
	t2 = rdtsc();

	printf("%lld\n", t2-t1);
	return 0;
}
// out
// 1114, 11118, 502

