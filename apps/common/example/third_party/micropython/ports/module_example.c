#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

/** 添加一个加法模块*/

/** 1.添加将从Python中调用的函数cexample.add_ints（a，b）*/
STATIC mp_obj_t example_add_ints(mp_obj_t a_obj, mp_obj_t b_obj)
{
    int a = mp_obj_get_int(a_obj);
    int b = mp_obj_get_int(b_obj);
    return mp_obj_new_int(a + b);
}

/** 2.定义对上面函数的Python引用*/
STATIC MP_DEFINE_CONST_FUN_OBJ_2(example_add_ints_obj, example_add_ints);

/** 3.定义模块的所有属性*/
STATIC const mp_rom_map_elem_t example_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_cexample) },
    { MP_ROM_QSTR(MP_QSTR_add_ints), MP_ROM_PTR(&example_add_ints_obj) },
};

STATIC MP_DEFINE_CONST_DICT(example_module_globals, example_module_globals_table);

/** 4.定义模块对象*/
const mp_obj_module_t example_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *) &example_module_globals,
};

/** 5.注册该模块以使其在Python中可用*/
MP_REGISTER_MODULE(MP_QSTR_cexample, example_user_cmodule, MODULE_CEXAMPLE_ENABLED);

/** 6.在qstrdefs.generated.h中添加对应的键值对*/
//QDEF(MP_QSTR_add_ints, (const byte*)"\x3b\x08" "add_ints")
//QDEF(MP_QSTR_cexample, (const byte*)"\xce\x08" "cexample")

/** 7.在mpconfigport.h文件中添加对应模块*/
//#define MICROPY_PORT_BUILTIN_MODULES \
//    {MP_ROM_QSTR(MP_QSTR_cexample), MP_ROM_PTR(&example_user_cmodule)}, \

/** 8.在moduledefs.h中对应模块*/
//extern const mp_obj_module_t example_user_cmodule;

/** 9.用法*/
//(1)导入模块
//	import cexample
//(2)调用模块接口
//  cexample.add_ints(a,b)
