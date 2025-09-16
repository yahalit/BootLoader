/*****************************************************************************/
/* stdlib.h   v6.0.1                                                         */
/*                                                                           */
/* Copyright (c) 1993-2015 Texas Instruments Incorporated                    */
/* http://www.ti.com/                                                        */
/*                                                                           */
/*  Redistribution and  use in source  and binary forms, with  or without    */
/*  modification,  are permitted provided  that the  following conditions    */
/*  are met:                                                                 */
/*                                                                           */
/*     Redistributions  of source  code must  retain the  above copyright    */
/*     notice, this list of conditions and the following disclaimer.         */
/*                                                                           */
/*     Redistributions in binary form  must reproduce the above copyright    */
/*     notice, this  list of conditions  and the following  disclaimer in    */
/*     the  documentation  and/or   other  materials  provided  with  the    */
/*     distribution.                                                         */
/*                                                                           */
/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
/*     of its  contributors may  be used to  endorse or  promote products    */
/*     derived  from   this  software  without   specific  prior  written    */
/*     permission.                                                           */
/*                                                                           */
/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
/*                                                                           */
/*****************************************************************************/

#ifndef _STDLIB
#define _STDLIB

/*---------------------------------------------------------------------------*/
/* Attributes are only available in relaxed ANSI mode.                       */
/*---------------------------------------------------------------------------*/
#ifndef __ATTRIBUTE
#if __TI_STRICT_ANSI_MODE__
#define __ATTRIBUTE(attr)
#else
#define __ATTRIBUTE(attr) __attribute__(attr)
#endif
#endif


#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cstdlib> IS RECOMMENDED OVER <stdlib.h>.  <stdlib.h> IS PROVIDED FOR 
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std {
#endif /* !__cplusplus */

typedef struct { int quot, rem; } div_t;

typedef struct { long quot, rem; } ldiv_t;

#define _LLONG_AVAILABLE 1
typedef struct { long long quot, rem; } lldiv_t;

#define MB_CUR_MAX    1

#ifndef NULL
#define NULL          0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_T_TYPE__ size_t;
#endif

#ifndef __cplusplus
#ifndef _WCHAR_T
#define _WCHAR_T
typedef __WCHAR_T_TYPE__ wchar_t;
#endif
#endif

#define EXIT_FAILURE  1
#define EXIT_SUCCESS  0

#define RAND_MAX      32767

#include "linkage.h"

/*---------------------------------------------------------------*/
/* NOTE - Normally, abs, labs, and fabs are expanded inline, so  */
/*        no formal definition is really required. However, ANSI */
/*        requires that they exist as separate functions, so     */
/*        they are supplied in the library.  The prototype is    */
/*        here mainly for documentation.                         */
/*---------------------------------------------------------------*/
      int       abs(int _val);
      long      labs(long _val);
#if defined(_LLONG_AVAILABLE)
      long long llabs(long long _val);
#endif
     int       atoi(const char *_st);
     long      atol(const char *_st);
#if defined(_LLONG_AVAILABLE)
     long long atoll(const char *_st);
#endif
     int       ltoa(long val, char *buffer);
          _IDECL double    atof(const char *_st);

     long      strtol(const char *_st, char **_endptr, int _base);
     unsigned long strtoul(const char *_st, char **_endptr,
    					  int _base);
#if defined(_LLONG_AVAILABLE)
     long long strtoll(const char *_st, char **_endptr, int _base);
     unsigned long long strtoull(const char *_st, char **_endptr,
					     int _base);
#endif
     double    strtod(const char *_st, char **_endptr);
     long double strtold(const char *_st, char **_endptr);
    
     int    rand(void);
     void   srand(unsigned _seed);
    
     void  *calloc(size_t _num, size_t _size)
               __ATTRIBUTE((malloc));
     void  *malloc(size_t _size)
               __ATTRIBUTE((malloc));
     void  *realloc(void *_ptr, size_t _size)
               __ATTRIBUTE((malloc));
     void   free(void *_ptr);
     void  *memalign(size_t _aln, size_t _size)
               __ATTRIBUTE((malloc));
    
     void   abort(void);
     int    atexit(void (*_func)(void));
     void  *bsearch(const void *_key, const void *_base,
    				   size_t _nmemb, size_t _size, 
    			           int (*compar)(const void *,const void *));
     void   qsort(void *_base, size_t _nmemb, size_t _size,
    			         int (*_compar)(const void *, const void *));
     void   exit(int _status);
    
     div_t  div(int _numer, int _denom);
     ldiv_t ldiv(long _numer, long _denom);
#if defined(_LLONG_AVAILABLE)
     lldiv_t lldiv(long long _numer, long long _denom);
#endif

     char  *getenv(const char *_string);
     int    system(const char *_name);

     int    mblen(const char *, size_t);
     size_t mbstowcs(wchar_t *, const char *, size_t);
     int    mbtowc(wchar_t *, const char *, size_t);

     size_t wcstombs(char *, const wchar_t *, size_t);
     int    wctomb(char *, wchar_t);

#ifdef __cplusplus
} /* extern "C" namespace std */
#endif /* __cplusplus */


#ifdef _INLINE

#ifdef __cplusplus
namespace std {
#endif

static __inline double atof(const char *_st) 
{
  return strtod(_st, (char **)0); 
}

#ifdef __cplusplus
} /* namespace std */
#endif

#endif  /* _INLINE */

#endif  /* ! _STDLIB */

#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
using std::div_t;
using std::ldiv_t;
#if defined(_LLONG_AVAILABLE)
using std::lldiv_t;
#endif
using std::size_t;
using std::abs;
using std::labs;
using std::atoi;
using std::atol;
#if defined(_LLONG_AVAILABLE)
using std::llabs;
using std::atoll;
#endif
using std::atof;
using std::strtol;
using std::strtoul;
#if defined(_LLONG_AVAILABLE)
using std::strtoll;
using std::strtoull;
#endif
using std::strtod;
using std::rand;
using std::srand;
using std::calloc;
using std::malloc;
using std::realloc;
using std::free;
using std::memalign;
using std::abort;
using std::atexit;
using std::bsearch;
using std::qsort;
using std::exit;
using std::div;
using std::ldiv;
#if defined(_LLONG_AVAILABLE)
using std::lldiv;
#endif
using std::getenv;
using std::system;
#endif /* ! _CPP_STYLE_HEADER */

#include "stdlibf.h"     /* far versions of stdlib fns */

//
// End of File
//

