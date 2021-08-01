#ifndef SMPTR_H
#define SMPTR_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

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

void *smartAlloc( size_t allocSize );
void *smartShare( void *ptr );
void  smartAddDestructor( void *ptr, dtor_function dtor );

#endif /* SMPTR_H */
