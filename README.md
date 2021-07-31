# libsmptr

## 實作目的

**實現指標在程序內結束生命週期後的自動回收能力，並能添加解構子、共用指標 等功能。**

## 主要函式

* void *smartAlloc( size_t allocSize )
* void *smartShare( void *ptr );
* void  smartAddDestructor( void *ptr, dtor_function dtor );

## 解構子型別
* typedef void ( *dtor_function )( void *ptr );

## 屬性宏
* #define smartPtrAttr __attribute__( ( cleanup( smartFree ) ) 
```
smartPtrAttr int *p = smartAlloc(sizeof(int));
```

## 功能說明
### void *smartAlloc( size_t allocSize );
* @brief  分配智能指標
* @note   指標需添加 "smartPtrAttr" 屬性
* @param  allocSize: 大小
* @retval 指標位址


### void *smartShare( void *ptr );
* @brief  共用指標
* @note   共用已配配的指標
* @param  *ptr: 已分配的指標
* @retval 位址


### void  smartAddDestructor( void *ptr, dtor_function dtor );
* @brief  添加解構子函式
* @note   解構子須符合 "dtor_function" 類型，ptr 解構子所在的指標。
* @param  *ptr: 添加的智能指標
* @param  dtor: 解構子
* @retval None

## 範例 main.c
```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libsmptr/include/smptr.h"

// Please refer to smptr.h for function description

void printfAfterFree( void *ptr ) {
    ( void )ptr;
    printf( "In dtor_function!!" );
}

void secondFunc( void *ptr ) {
    smartPtrAttr int *c = smartShare( ptr );

    *c = 20;

    printf( "In second function. c : %d\n", *c );

    // Call smartFree function at this. but process isn't last use the shared pointer.
    // Pointer will not be released.
}

int main( void ) {

    // Create pointer space use smart alloc function.
    smartPtrAttr int *p = smartAlloc( sizeof( int ) * 100 );

    // Add destructor function.
    smartAddDestructor( p, printfAfterFree );

    *p = 10;
    printf( "%d\n", *p );

    // Call second function that share 'p' pointer to test unit function.
    secondFunc( p );

    printf( "%d\n", *p );

    printf( "Hello World\n" );

    // Pointer has been released at this. if destructor has added, it called before the released function.
    return 0;
}

```
## Output
```
10
In second function. c : 20
20
Hello World
In dtor_function!!
```

