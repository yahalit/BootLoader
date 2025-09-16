/*****************************************************************************/
/* stdlibf.h   v6.0.1                                                        */
/*     far versions of stdlib.h functions                                    */
/*                                                                           */
/* Copyright (c) 1996-2015 Texas Instruments Incorporated                    */
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
#ifndef _STDLIBF
#define _STDLIBF

#ifndef __cplusplus
 long          far_strtol (const  char *st,  char **endptr,
				   int base);
 unsigned long far_strtoul(const  char *st,  char **endptr,
				   int base);
 double        far_strtod (const  char *st,  char **endptr);
 long double   far_strtold (const  char *st,  char **endptr);
 long long     far_strtoll(const  char *st,  char **endptr,
		 						    int base);
 unsigned long long far_strtoull(const  char *st,
                                                    char **endptr, int base);
 int           far_atoi(const  char *st);
 long          far_atol(const  char *st);
_IDECL       double        far_atof(const  char *st);
 long long     far_atoll(const  char *st);

 void     *far_calloc  (unsigned long num, unsigned long size);
 void     *far_malloc  (unsigned long size);
 void     *far_realloc ( void *ptr, unsigned long size);
 void          far_free    ( void *ptr);

 void     *far_bsearch(const  void *key,
                                       const  void *base,
			               size_t nmemb, size_t size, 
			               int (*compar)(const  void *,
                                                     const  void *));
 void          far_qsort  ( void *base,
                                       size_t nmemb, size_t size,
			               int (*compar)(const  void *,
                                                     const  void *));

 int	   free_memory(void);
 int           max_free(void);
 void         *chkheap();

#ifdef _INLINE
static __inline double far_atof(const  char *st)
{ 
   return far_strtod(st, NULL);
}
#endif /* _INLINE */

#else /* __cplusplus */

namespace std {

extern "C" int	       free_memory(void);
extern "C" int         max_free(void);
extern "C" void        *chkheap();

 long      far_calloc  (unsigned long num, unsigned long size);
 long      far_malloc  (unsigned long size);
 long      far_realloc (long ptr, unsigned long size);
 void      far_free    (long ptr);
 long      far_free_memory(void);
 long      far_max_free(void);
 long      far_chkheap(void);

}
#endif /* __cplusplus */
#endif /* _STDLIBF */

#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
using std::free_memory; 
using std::max_free;
using std::chkheap;
using std::far_calloc;
using std::far_malloc;
using std::far_realloc;
using std::far_free;
using std::far_free_memory; 
using std::far_max_free;
using std::far_chkheap;
#endif /* _CPP_STYLE_HEADER */

//
// End of File
//

