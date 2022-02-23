#include <stdint.h>
//#include <stdio.h>
#include <string.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"

#include "lib/utils/pyexec.h"

#ifdef bool
#undef bool
#endif

#include "os/os_compat.h"

#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind)
{
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

static char *stack_top;
#if MICROPY_ENABLE_GC
/* static char heap[2048]; */
static char heap[3 * 1024];
#endif

extern int mp_hal_stdin_rx_chr(void);

int micropython_test(void)
{
    int stack_dummy;
    stack_top = (char *)&stack_dummy;

#if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
#endif
    mp_init();
#if MICROPY_ENABLE_COMPILER
#if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    for (;;) {
        int c = mp_hal_stdin_rx_chr();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
#else
    //pyexec_friendly_repl();
#endif
    // do_str("print('hello world!', list(x+1 for x in range(10)), end='eol\\n')", MP_PARSE_SINGLE_INPUT);
    // do_str("for i in range(10):\r\n  print(i)", MP_PARSE_FILE_INPUT);
#else
    //pyexec_frozen_module("frozentest.py");
#endif
    mp_deinit();
    return 0;
}

#if MICROPY_ENABLE_GC
void gc_collect(void)
{
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}
#endif

mp_lexer_t *mp_lexer_new_from_file(const char *filename)
{
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path)
{
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs)
{
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val)
{
    while (1) {
        ;
    }
}

void NORETURN __fatal_error(const char *msg)
{
    while (1) {
        ;
    }
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr)
{
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

static int mp_start(void)
{
    micropython_test();
    return 0;
}

/** micropython 入口*/
int micropython_start(void)
{
    if (thread_fork("mp_start", 17, 2 * 1024, 0, NULL, mp_start, NULL) != OS_NO_ERR) {
        return -1;
    }
    return 0;
}

