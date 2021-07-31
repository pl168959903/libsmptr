#ifndef SMPTR_H
#define SMPTR_H

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#define container_of( ptr, type, member )                                 \
    __extension__( {                                                      \
        const __typeof__( ( ( type * )0 )->member ) *__pmember = ( ptr ); \
        ( type * )( ( char * )__pmember - offsetof( type, member ) );     \
    } )

#define smartPtrAttr __attribute__( ( cleanup( smartFree ) ) )

typedef void ( *dtor_function )( void *ptr );

typedef struct {
    dtor_function   dtor;
    volatile size_t possess_count;
    size_t          alloSize;
    volatile size_t smPtr[ 0 ];
} smPtr_header_t;

typedef struct {
    void *(*alloc)(size_t);
    void (*dealloc)(void *);
} s_allocator;

void  smartFree( void *ptr );

//----------------------------------------------------------------

/**
 * @brief  分配智能指標
 * @note   指標需添加 "smartPtrAttr" 屬性
 * @param  allocSize: 大小
 * @retval 指標位址
 */
void *smartAlloc( size_t allocSize );

/**
 * @brief  共用指標
 * @note   共用已配配的指標
 * @param  *ptr: 已分配的指標
 * @retval 位址
 */
void *smartShare( void *ptr );

/**
 * @brief  添加解構子函式
 * @note   解構子須符合 "dtor_function" 類型，ptr 解構子所在的指標。
 * @param  *ptr: 添加的智能指標
 * @param  dtor: 解構子
 * @retval None
 */
void  smartAddDestructor( void *ptr, dtor_function dtor );

#endif /* SMPTR_H */
