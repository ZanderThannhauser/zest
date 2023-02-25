
#ifndef RELEASE
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/param.h>
	#include <sys/types.h>
	#include <sys/mman.h>
	
	#include <semaphore.h>
	#include <assert.h>
	#include <stdio.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <ctype.h>
	#include <getopt.h>
	#include <stddef.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <ftw.h>
	#include <string.h>
	#include <strings.h>
	#include <inttypes.h>
	#include <stdarg.h>
	#include <limits.h>
	#include <pwd.h>
	#include <time.h>
	#include <pwd.h>
	
	struct cmdln_flags;
	struct avl_tree_t;
	struct avl_node_t;
	struct zebu_test;
	struct record;
	struct test;
	struct string;
	struct value;
	struct zebu_expression;
	struct zebu_conditional_expression;
	struct zebu_inclusive_or_expression;
	struct zebu_logical_and_expression;
	struct zebu_exclusive_or_expression;
	struct zebu_and_expression;
	struct zebu_equality_expression;
	struct zebu_relational_expression;
	struct zebu_shift_expression;
	struct zebu_additive_expression;
	struct zebu_multiplicative_expression;
	struct zebu_prefix_expression;
	struct zebu_primary_expression;
	struct value_inheritance;
	struct flattened_records;
	
	#include <memory/smalloc.h>
	#include <memory/srealloc.h>
	
	#include <defines/argv0.h>
	
	#include <macros/strequals.h>
	
	#include <enums/error.h>
	
	#include <avl/alloc_tree.h>
	#include <avl/free_tree.h>
	#include <avl/insert.h>
	#include <avl/search.h>
	#include <avl/delete_node.h>
	
#endif

#ifdef DEBUGGING
	extern int debug_depth;
	
	extern void real_dpvc(const char* e, char ch);
	
	extern void real_dpvsn(const char* e, const char* s, size_t n);
	
	extern void real_dpvwsn(const char* e, const wchar_t* s, size_t n);
	
	#define TODO \
	{\
		assert(debug_depth >= 0); \
		printf("%*s""TODO: File: %s: Line: %i\n", debug_depth, "", \
			__FILE__, __LINE__);\
		fflush(stdout); \
		char buffer[100];\
		sprintf(buffer, "+%i", __LINE__);\
		assert(!"TODO"); \
	}
	
	#define CHECK \
	{\
		assert(debug_depth >= 0); \
		printf("%*s""CHECK: File: %s: Line: %i\n", debug_depth, "", \
			__FILE__, __LINE__);\
		fflush(stdout); \
		char buffer[100];\
		sprintf(buffer, "+%i", __LINE__);\
		assert(!"CHECK"); \
	}
	
	#define NOPE \
	{\
		assert(debug_depth >= 0); \
		printf("%*s""NOPE: File: %s: Line: %i\n", debug_depth, "", \
			__FILE__, __LINE__);\
		fflush(stdout); \
		char buffer[100];\
		sprintf(buffer, "+%i", __LINE__);\
		assert(!"NOPE"); \
	}
	
	#define CHECK_NTH(n) \
	{ \
		static unsigned counter = 1; \
		if (counter++ == n) \
		{ \
			CHECK; \
		} \
	} \
	
	#define HERE \
		printf("%*sHERE: File: %s: Line: %i\n", debug_depth, "", __FILE__, __LINE__);

	#define ENTER \
		assert(debug_depth >= 0); \
		printf("%*s<%s>\n", debug_depth++, "", __PRETTY_FUNCTION__);
	
	#define EXIT \
		assert(--debug_depth >= 0); \
		printf("%*s</%s>\n", debug_depth, "", __PRETTY_FUNCTION__);
	
	#define dpv(val) dprint(val)
	
	#define dpvb(b) \
		assert(debug_depth >= 0);\
		printf("%*s" #b " == %s\n", debug_depth, "", (b) ? "true" : "false");
	
	#define dputs(str) \
		assert(debug_depth >= 0);\
		printf("%*s" "%s\n", debug_depth, "", str);
	
	#define dpvc(ch) \
		real_dpvc(#ch, ch);
	
	#define dpvsn(str, _len) { \
		_Generic(str, \
			char*:          real_dpvsn (#str, ( char  *) (str), _len), \
			const char*:    real_dpvsn (#str, ( char  *) (str), _len), \
			wchar_t*:       real_dpvwsn(#str, (wchar_t*) (str), _len), \
			const wchar_t*: real_dpvwsn(#str, (wchar_t*) (str), _len), \
			default:        real_dpvsn (#str, (   void*) (str), _len)); \
	}
	
	#define dpvs(str) dpvsn(str, -1)
	
	#define dperror(val) { \
		assert(debug_depth >= 0); \
		printf("%*s""%s"" == %s (%s)\n", \
			debug_depth, "", \
			(const char*[]){ \
				[0] = "0", \
				[EPERM] = "EPERM", \
				[ENOENT] = "ENOENT", \
				[ESRCH] = "ESRCH", \
				[EINTR] = "EINTR", \
				[EIO] = "EIO", \
				[ENXIO] = "ENXIO", \
				[E2BIG] = "E2BIG", \
				[ENOEXEC] = "ENOEXEC", \
				[EBADF] = "EBADF", \
				[ECHILD] = "ECHILD", \
				[EAGAIN] = "EAGAIN", \
				[ENOMEM] = "ENOMEM", \
				[EACCES] = "EACCES", \
				[EFAULT] = "EFAULT", \
				[ENOTBLK] = "ENOTBLK", \
				[EBUSY] = "EBUSY", \
				[EEXIST] = "EEXIST", \
				[EXDEV] = "EXDEV", \
				[ENODEV] = "ENODEV", \
				[ENOTDIR] = "ENOTDIR", \
				[EISDIR] = "EISDIR", \
				[EINVAL] = "EINVAL", \
				[ENFILE] = "ENFILE", \
				[EMFILE] = "EMFILE", \
				[ENOTTY] = "ENOTTY", \
				[ETXTBSY] = "ETXTBSY", \
				[EFBIG] = "EFBIG", \
				[ENOSPC] = "ENOSPC", \
				[ESPIPE] = "ESPIPE", \
				[EROFS] = "EROFS", \
				[EMLINK] = "EMLINK", \
				[EPIPE] = "EPIPE", \
				[EDOM] = "EDOM", \
				[ERANGE] = "ERANGE", \
				[EHWPOISON] = "EHWPOISON", \
			}[val] ?: "???", \
			#val, strerror(val)); \
		}
	
	#define dpvo(val) { \
		assert(debug_depth >= 0);\
		printf((_Generic(val, \
			signed char:    "%*s" "%s" " == " "(signed char) 0%hho\n", \
			unsigned char:  "%*s" "%s" " == " "(unsigned char) 0%hho\n", \
			signed short:   "%*s" "%s" " == " "(signed short) 0%ho\n", \
			unsigned short: "%*s" "%s" " == " "(unsigned char) 0%ho\n", \
			signed int:     "%*s" "%s" " == " "(signed int) 0%o\n", \
			unsigned int:   "%*s" "%s" " == " "(unsigned int) 0%o\n", \
			signed long:    "%*s" "%s" " == " "(signed long) 0%lo\n", \
			unsigned long:  "%*s" "%s" " == " "(unsigned long) 0%lo\n", \
			float:          "%*s" "%s" " == " "(float) %#a\n", \
			double:         "%*s" "%s" " == " "(double) %#la\n", \
			long double:    "%*s" "%s" " == " "(long double) %#La\n", \
			default:        "%*s" "%s" " == " "(void*) %#p\n")), \
			debug_depth, "", #val, val); \
		}
	
	#define dpvx(val) { \
		assert(debug_depth >= 0); \
		printf((_Generic(val, \
			signed char:    "%*s" "%s" " == " "(signed char) 0x%hhX\n", \
			unsigned char:  "%*s" "%s" " == " "(unsigned char) 0x%hhX\n", \
			signed short:   "%*s" "%s" " == " "(signed short) 0x%hX\n", \
			unsigned short: "%*s" "%s" " == " "(unsigned char) 0x%hX\n", \
			signed int:     "%*s" "%s" " == " "(signed int) 0x%X\n", \
			unsigned int:   "%*s" "%s" " == " "(unsigned int) 0x%X\n", \
			signed long:    "%*s" "%s" " == " "(signed long) 0x%lX\n", \
			unsigned long:  "%*s" "%s" " == " "(unsigned long) 0x%lX\n", \
			float:          "%*s" "%s" " == " "(float) %#a\n", \
			double:         "%*s" "%s" " == " "(double) %#la\n", \
			long double:    "%*s" "%s" " == " "(long double) %#La\n", \
			default:        "%*s" "%s" " == " "(void*) %#p\n")), \
			debug_depth, "", #val, val); \
		}
	
	#define dprint(val) { \
		assert(debug_depth >= 0); \
		printf((_Generic((val), \
			signed char:    "%*s" "%s" " == " "(signed char) %hhi\n", \
			unsigned char:  "%*s" "%s" " == " "(unsigned char) %hhu\n", \
			signed short:   "%*s" "%s" " == " "(signed short) %hi\n", \
			unsigned short: "%*s" "%s" " == " "(unsigned short) %hu\n", \
			signed int:     "%*s" "%s" " == " "(signed int) %i\n", \
			unsigned int:   "%*s" "%s" " == " "(unsigned int) %u\n", \
			signed long:    "%*s" "%s" " == " "(signed long) %li\n", \
			unsigned long:  "%*s" "%s" " == " "(unsigned long) %lu\n", \
			long long:      "%*s" "%s" " == " "(signed long long) %llu\n", \
			float:          "%*s" "%s" " == " "(float) %f\n", \
			double:         "%*s" "%s" " == " "(double) %lf\n", \
			long double:    "%*s" "%s" " == " "(long double) %Lf\n", \
			default:        "%*s" "%s" " == " "(void*) %p\n")), \
			debug_depth, "", #val, (val)); \
		}
	
	#define ddprintf(format, ...) { \
		assert(debug_depth >= 0); \
		printf("%*s" format, debug_depth, "", ## __VA_ARGS__); \
	}
	
#else
	#define dpv(x) ;
	#define dpvb(x) ;
	#define dpvc(x) ;
	#define dpvo(x) ;
	#define dpvs(x) ;
	#define dputs(x) ;
	#define dpvx(x) ;
	#define dpvsn(_, __) ;
	#define ddprintf(...) ;
	
	#define dperror(x) ;
	
	#define ENTER
	#define EXIT
	#define HERE
	
	#define TODO assert(!"TODO");
	#define CHECK assert(!"CHECK");
	#define NOPE assert(!"NOPE");
	
#endif




































