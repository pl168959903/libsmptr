#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "libsmptr/include/smptr.h"

// Please refer to smptr.h for function description

void printfAfterFree(void * ptr){
    (void) ptr;
    printf("In dtor_function!!");
}

void secondFunc(void * ptr){
    smartPtrAttr int *c = smartShare(ptr);

    *c = 20;

    printf("In second function. c : %d\n", *c);

    // Call smartFree function at this. but process isn't last use the shared pointer. 
    // Pointer will not be released.
}

int main(void){


    // Create pointer space use smart alloc function.
    smartPtrAttr int *p = smartAlloc(sizeof(int) * 100);

    // Add destructor function.
    smartAddDestructor(p, printfAfterFree);

    *p = 10;
    printf("%d\n", *p);

    // Call second function that share 'p' pointer to test unit function.
    secondFunc(p);


    printf("%d\n", *p);

    printf("Hello World\n");

    // Pointer has been released at this. if destructor has added, it called before the released function.
    return 0;
}
