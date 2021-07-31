#ifndef SMPTR_COMMON_H
#define SMPTR_COMMON_H

#ifdef __GNUC__
#define SMPTR_INLINE     __attribute__( ( always_inline ) ) inline
#define SMPTR_MALLOC_API __attribute__( ( malloc ) )
#define SMPTR_PURE       __attribute__( ( pure ) )
#endif

#endif /* !SMPTR_COMMON_H */
