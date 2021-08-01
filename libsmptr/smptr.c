
#include ".\include\smptr.h"
#include ".\include\smptr_common.h"

s_allocator smalloc_allocator = {malloc, free};

static SMPTR_INLINE size_t atomic_add( volatile size_t *count, const size_t limit, const size_t val ) {
    size_t old_count, new_count;
    do {
        old_count = *count;
        if ( old_count == limit ) abort();
        new_count = old_count + val;
    } while ( !__sync_bool_compare_and_swap( count, old_count, new_count ) );
    return new_count;
}

static SMPTR_INLINE size_t atomic_increment( volatile size_t *count ) {
    return atomic_add( count, SIZE_MAX, 1 );
}

static size_t atomic_decrement( volatile size_t *count ) {
    return atomic_add( count, 0, -1 );
}

static SMPTR_INLINE size_t align( size_t s ) {
    return ( s + ( sizeof( char * ) - 1 ) ) & ~( sizeof( char * ) - 1 );
}

SMPTR_MALLOC_API
void *smartAlloc( size_t allocSize)  {
    size_t totalSize = align( sizeof( smPtr_header_t ) + allocSize );
    void * basePtr   = ( void * )smalloc_allocator.alloc( totalSize );
    smPtr_header_t *st = ( smPtr_header_t * )basePtr;
    *st                = ( smPtr_header_t ){ .dtor = NULL, .possess_count = 0, .alloSize = allocSize };
    atomic_increment(&(st->possess_count));
    return ( void * )( st->smPtr );
}

void smartFree( void *clPtr ) {
    void * ptr = *(void**)clPtr;
    void* basePtr = container_of(ptr, smPtr_header_t, smPtr);
    smPtr_header_t *st = ( smPtr_header_t*) basePtr;
    if(atomic_decrement(&st->possess_count)) return;
    if(st->dtor) st->dtor(ptr);
    smalloc_allocator.dealloc( basePtr );
}

SMPTR_MALLOC_API
void *smartShare(void *ptr){
    void* basePtr = container_of(ptr, smPtr_header_t, smPtr);
    smPtr_header_t *st = ( smPtr_header_t*) basePtr;
    atomic_increment(&(st->possess_count));
    return ptr;
}

void smartAddDestructor( void * ptr, dtor_function dtor){
    void* basePtr = container_of(ptr, smPtr_header_t, smPtr);
    smPtr_header_t *st = ( smPtr_header_t*) basePtr;
    st->dtor = dtor;
}
