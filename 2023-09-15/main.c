#include <stdio.h>
#include <windows.h>

void step1(){

	printf("step 1\n");

	char *a = (char *)malloc(10);
	strcpy(a, "ABCD");

	printf("%x\n",(unsigned int)&a);	// ebp-4
	printf("%x\n",(unsigned int)a);		// [ebp-4]
	printf("%s\n",a);					// [[ebp-4]]

	free(a);

	return;
}

void step2(){

	printf("\nstep 2\n");
	
	DWORD heapNumber = GetProcessHeaps(0, NULL);
	printf("number of heaps : %d\n", heapNumber);
	// 1

	HANDLE defaultHeap = GetProcessHeap();
	printf("default heap : %x\n", (unsigned int)defaultHeap);
	// +  1330000  133a000     a000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     Heap       [ID: 0; Handle: 01330000; Type: Segment]
	//    133a000  142f000    f5000 MEM_PRIVATE MEM_RESERVE                                    Heap       [ID: 0; Handle: 01330000; Type: Segment]
	//    142f000  1430000     1000 MEM_PRIVATE MEM_RESERVE                                    <unknown>  
	// +  1430000 76160000 74d30000             MEM_FREE    PAGE_NOACCESS                      Free       

	ULONG_PTR *a = (ULONG_PTR *)HeapAlloc(defaultHeap, 0, 10);

	HANDLE newHeap = HeapCreate(0, 0, 0);
	printf("created heap : %x\n", (unsigned int)newHeap);
	// +  1330000  133a000     a000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     Heap       [ID: 0; Handle: 01330000; Type: Segment]
	//    133a000  142f000    f5000 MEM_PRIVATE MEM_RESERVE                                    Heap       [ID: 0; Handle: 01330000; Type: Segment]
	//    142f000  1430000     1000 MEM_PRIVATE MEM_RESERVE                                    <unknown>  
	// +  1430000  1610000   1e0000             MEM_FREE    PAGE_NOACCESS                      Free       
	// +  1610000  1611000     1000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     Heap       [ID: 1; Handle: 01610000; Type: Segment]
	//    1611000  161f000     e000 MEM_PRIVATE MEM_RESERVE                                    Heap       [ID: 1; Handle: 01610000; Type: Segment]
	//    161f000  1620000     1000 MEM_PRIVATE MEM_RESERVE                                    <unknown>  
	// +  1620000 76160000 74b40000             MEM_FREE    PAGE_NOACCESS                      Free       


	ULONG_PTR *b = (ULONG_PTR *)HeapAlloc(newHeap, 0, 10);
	ULONG_PTR *c = (ULONG_PTR *)HeapAlloc(newHeap, HEAP_ZERO_MEMORY, 10);

	HANDLE newnewHeap = HeapCreate(0, 0, 0);
	RtlFillMemory(newnewHeap, 0x10, 0x41);

	heapNumber = GetProcessHeaps(0, 0);
	printf("number of heaps : %d\n", heapNumber);
	// 3

	// +   f61000   f80000    1f000             MEM_FREE    PAGE_NOACCESS                      Free       
	// +   f80000   f87000     7000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     <unknown>  [................]

	void *mem = VirtualAlloc(NULL, 24 * 1024, MEM_COMMIT, PAGE_READWRITE);
	// +   f61000   f70000     f000             MEM_FREE    PAGE_NOACCESS                      Free       
	// +   f70000   f76000     6000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     <unknown>  [................]
	// +   f76000   f80000     a000             MEM_FREE    PAGE_NOACCESS                      Free       
	// +   f80000   f87000     7000 MEM_PRIVATE MEM_COMMIT  PAGE_READWRITE                     <unknown>  [................]

	HeapFree(defaultHeap, 0, a);
	HeapFree(newHeap, 0, b);
	HeapFree(newHeap, 0, c);
	VirtualFree(mem, 0, MEM_RELEASE);
}

void step3(){

	printf("step 3\n");

	char *str = (char *)malloc(5);
	int n = scanf("%s", str);

	if (n == 6) {
		(char *)realloc(str, 10);
	}

	printf("%s", str);

	free(str);
}

void main(){

	step1();

	step2();

	step3();

}
