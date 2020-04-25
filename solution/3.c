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
	int *a;
	a = (int*) malloc(1024*1024*sizeof(int)); //page contains 1024 int's
	
	int i;
	long long int t1, t2;
	printf("column 1, column 2\n");
	for(i=0; i<1024; i++)
	{
		t1 = rdtsc();
		a[i*1024 + 100]++;
		t2 = rdtsc();
		printf("%d, %lld\n", i, t2-t1);	
	}
return 0;

}
