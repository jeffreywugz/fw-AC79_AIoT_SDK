/**
 * Copyright (2017) Baidu Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * File: config.h
 * Auth: Su Hao (suhao@baidu.com)
 * Desc: Speex port configuration.
 */

#ifndef BAIDU_DUER_BAIDU_SPEEX_WRAPPER_CONFIG_H
#define BAIDU_DUER_BAIDU_SPEEX_WRAPPER_CONFIG_H

#if 0
#ifndef DUER_PLATFORM_ESPRESSIF

#define DISABLE_VBR /**/

#define DISABLE_DECODER

/* Enable valgrind extra checks */
/* #undef ENABLE_VALGRIND */

/* Symbol visibility prefix */
#define EXPORT __attribute__((visibility("default")))

/* Debug fixed-point implementation */
/* #undef FIXED_DEBUG */

/* Compile as fixed-point */
/* #undef FIXED_POINT */

/* Compile as floating-point */
#define FLOATING_POINT /**/

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/audioio.h> header file. */
/* #undef HAVE_SYS_AUDIOIO_H */

/* Define to 1 if you have the <sys/soundcard.h> header file. */
#define HAVE_SYS_SOUNDCARD_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "speex-dev@xiph.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "speex"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "speex 1.2.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "speex"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.0"

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `int16_t', as computed by sizeof. */
#define SIZEOF_INT16_T 2

/* The size of `int32_t', as computed by sizeof. */
#define SIZEOF_INT32_T 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `uint16_t', as computed by sizeof. */
#define SIZEOF_UINT16_T 2

/* The size of `uint32_t', as computed by sizeof. */
#define SIZEOF_UINT32_T 4

/* The size of `u_int16_t', as computed by sizeof. */
#define SIZEOF_U_INT16_T 2

/* The size of `u_int32_t', as computed by sizeof. */
#define SIZEOF_U_INT32_T 4

/* Version extra */
#define SPEEX_EXTRA_VERSION ""

/* Version major */
#define SPEEX_MAJOR_VERSION 1

/* Version micro */
#define SPEEX_MICRO_VERSION 16

/* Version minor */
#define SPEEX_MINOR_VERSION 1

/* Complete version string */
#define SPEEX_VERSION "1.2.0"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable support for TI C55X DSP */
/* #undef TI_C55X */

/* Make use of alloca */
/* #undef USE_ALLOCA */

/* Use FFTW3 for FFT */
/* #undef USE_GPL_FFTW3 */

/* Use Intel Math Kernel Library for FFT */
/* #undef USE_INTEL_MKL */

/* Use KISS Fast Fourier Transform */
/* #undef USE_KISS_FFT */

/* Use FFT from OggVorbis */
#define USE_SMALLFT /**/

/* Use SpeexDSP library */
/* #undef USE_SPEEXDSP */

/* Use C99 variable-size arrays */
#define VAR_ARRAYS /**/

/* Enable support for the Vorbis psy model */
/* #undef VORBIS_PSYCHO */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Enable SSE support */
//#define _USE_SSE /**/

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

#endif // DUER_PLATFORM_ESPRESSIF
#endif

#define OVERRIDE_SPEEX_ERROR

#define OVERRIDE_SPEEX_WARNING

#define OVERRIDE_SPEEX_WARNING_INT

#define OVERRIDE_SPEEX_NOTIFY

// #define OVERRIDE_SPEEX_ALLOC

// #define OVERRIDE_SPEEX_ALLOC_SCRATCH

// #define OVERRIDE_SPEEX_REALLOC

// #define OVERRIDE_SPEEX_FREE

// #define OVERRIDE_SPEEX_FREE_SCRATCH

#define OVERRIDE_SPEEX_PUTC

#define FIXED_POINT		//考虑性能要求使用定点数运算，否则压缩音频时花费时间很长，而且只能用8K采样率，用16K在库里会挂掉，待解决

#endif/*BAIDU_DUER_BAIDU_SPEEX_WRAPPER_CONFIG_H*/
