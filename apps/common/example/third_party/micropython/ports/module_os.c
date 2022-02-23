#include "py/mpconfig.h"

#if MICROPY_PY_MODUOS

#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/objtuple.h"
#include "py/objstr.h"
#include "py/mperrno.h"

#include "moduos_file.h"


STATIC const qstr os_uname_info_fields[] = {
    MP_QSTR_sysname, MP_QSTR_nodename,
    MP_QSTR_release, MP_QSTR_version, MP_QSTR_machine
};

STATIC const MP_DEFINE_STR_OBJ(os_uname_info_sysname_obj, "pyboard");
STATIC const MP_DEFINE_STR_OBJ(os_uname_info_nodename_obj, "pyboard");
STATIC const MP_DEFINE_STR_OBJ(os_uname_info_release_obj, MICROPY_VERSION_STRING);
//STATIC const MP_DEFINE_STR_OBJ(os_uname_info_version_obj, MICROPY_GIT_TAG " on " MICROPY_BUILD_DATE);
STATIC const MP_DEFINE_STR_OBJ(os_uname_info_version_obj, "test" " on " "test");
STATIC const MP_DEFINE_STR_OBJ(os_uname_info_machine_obj, MICROPY_HW_BOARD_NAME " with " MICROPY_HW_MCU_NAME);
STATIC MP_DEFINE_ATTRTUPLE(
    os_uname_info_obj,
    os_uname_info_fields,
    5,
    (mp_obj_t)&os_uname_info_sysname_obj,
    (mp_obj_t)&os_uname_info_nodename_obj,
    (mp_obj_t)&os_uname_info_release_obj,
    (mp_obj_t)&os_uname_info_version_obj,
    (mp_obj_t)&os_uname_info_machine_obj
);

STATIC mp_obj_t os_uname(void)
{
    return (mp_obj_t)&os_uname_info_obj;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(os_uname_obj, os_uname);

STATIC mp_obj_t os_sync(void)
{
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(mod_os_sync_obj, os_sync);

STATIC const mp_rom_map_elem_t os_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uos) },
    { MP_ROM_QSTR(MP_QSTR_uname), MP_ROM_PTR(&os_uname_obj) },
    { MP_ROM_QSTR(MP_QSTR_chdir), MP_ROM_PTR(&mp_posix_chdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_getcwd), MP_ROM_PTR(&mp_posix_getcwd_obj) },
    { MP_ROM_QSTR(MP_QSTR_listdir), MP_ROM_PTR(&mp_posix_listdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_mkdir), MP_ROM_PTR(&mp_posix_mkdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_remove), MP_ROM_PTR(&mp_posix_remove_obj) },
    { MP_ROM_QSTR(MP_QSTR_rename), MP_ROM_PTR(&mp_posix_rename_obj)},
    { MP_ROM_QSTR(MP_QSTR_rmdir), MP_ROM_PTR(&mp_posix_rmdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_stat), MP_ROM_PTR(&mp_posix_stat_obj) },
    { MP_ROM_QSTR(MP_QSTR_unlink), MP_ROM_PTR(&mp_posix_remove_obj) },
    { MP_ROM_QSTR(MP_QSTR_sync), MP_ROM_PTR(&mod_os_sync_obj) },
    { MP_ROM_QSTR(MP_QSTR_mount), MP_ROM_PTR(&mp_posix_mount_obj) },
};

STATIC MP_DEFINE_CONST_DICT(os_module_globals, os_module_globals_table);

const mp_obj_module_t mp_module_uos = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *) &os_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_uos, mp_module_uos, MODULE_CEXAMPLE_ENABLED);

#endif /* MICROPY_PY_MODUOS */

