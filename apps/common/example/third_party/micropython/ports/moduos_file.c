#include "py/mpconfig.h"

#if MICROPY_PY_MODUOS_FILE
#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/mperrno.h"
#include "moduos_file.h"

#ifdef bool
#undef bool
#endif

#include "fs/fs.h"

#if 1
#define log_info(x, ...)    printf("\n\n>>>>>>[uos]" x " ", ## __VA_ARGS__)
#else
#define log_info(...)
#endif

//#include "app_config.h"
/** 默认为读取sd卡*/
#define CONFIG_STORAGE_PATH 	"storage/sd0"

mp_obj_t mp_posix_mount(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    log_info("Not supported ");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_posix_mount_obj, 2, mp_posix_mount);

mp_obj_t mp_posix_umount(mp_obj_t mnt_in)
{
    log_info("Not supported ");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_umount_obj, mp_posix_umount);

mp_obj_t mp_posix_chdir(mp_obj_t path_in)
{
    log_info("Not supported ");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_chdir_obj, mp_posix_chdir);

mp_obj_t mp_posix_getcwd(void)
{
    char buf[MICROPY_ALLOC_PATH_MAX + 1] = CONFIG_STORAGE_PATH"/C";
    return mp_obj_new_str(buf, strlen(buf));
}
MP_DEFINE_CONST_FUN_OBJ_0(mp_posix_getcwd_obj, mp_posix_getcwd);

/** 不指定路径时，默认为sd卡根目录.指定路径时需要指定完整路径，如：storage/sd0/C/DIR_PATH/*/
mp_obj_t mp_posix_listdir(size_t n_args, const mp_obj_t *args)
{
    mp_obj_t dir_list = mp_obj_new_list(0, NULL);

    u8 name[128];
    struct vfscan *fs;
    int name_len = 0;
    FILE *file = NULL;
    const char *pathname;

    if (n_args == 0) {
        pathname = CONFIG_STORAGE_PATH"/C";
    } else {
        pathname = mp_obj_str_get_str(args[0]);
    }

    fs = fscan(pathname, "-tTXTMP3 -sn");
    if (fs == NULL) {
        return mp_const_none;
    }

    for (int i = 1; i <= fs->file_number; i++) {
        file = fselect(fs, FSEL_BY_NUMBER, i);
        if (file) {
            memset(name, 0, sizeof(name));
            name_len = fget_name(file, name, sizeof(name));
            mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(3, NULL));
            t->items[0] = mp_obj_new_str(name, strlen(name));
            t->items[1] = MP_OBJ_NEW_SMALL_INT(MP_S_IFDIR);
            t->items[2] = MP_OBJ_NEW_SMALL_INT(0);
            mp_obj_t next = MP_OBJ_FROM_PTR(t);
            mp_obj_t *items;
            mp_obj_get_array_fixed_n(next, 3, &items);
            mp_obj_list_append(dir_list, items[0]);
            fclose(file);
        }
    }

    return dir_list;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_posix_listdir_obj, 0, 1, mp_posix_listdir);

/** 指定路径时需要指定完整路径，如：storage/sd0/C/DIR_PATH/*/
mp_obj_t mp_posix_mkdir(mp_obj_t path_in)
{
    const char *createpath = mp_obj_str_get_str(path_in);

    FILE *f = fopen(createpath, "w+");
    if (f == NULL) {
        mp_raise_OSError(MP_EEXIST);
    }

    fclose(f);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_mkdir_obj, mp_posix_mkdir);

/** 指定路径时需要指定完整路径，如：storage/sd0/C/DIR_PATH/*/
mp_obj_t mp_posix_remove(uint n_args, const mp_obj_t *arg)
{
    int index;
    if (n_args == 0) {
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        fdelete_by_name(mp_obj_str_get_str(arg[index]));
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_posix_remove_obj, 0, mp_posix_remove);

/** 指定路径时需要指定完整路径，如：storage/sd0/C/name.txt*/
mp_obj_t mp_posix_rename(mp_obj_t old_path_in, mp_obj_t new_path_in)
{
    const char *old_path = mp_obj_str_get_str(old_path_in);
    const char *new_path = mp_obj_str_get_str(new_path_in);

    FILE *f = fopen(old_path, "w+");
    if (f == NULL) {
        mp_raise_OSError(MP_EEXIST);
    }

    frename(f, new_path);
    fclose(f);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(mp_posix_rename_obj, mp_posix_rename);

/** 指定路径时需要指定完整路径，如：storage/sd0/C/name.txt*/
mp_obj_t mp_posix_rmdir(uint n_args, const mp_obj_t *arg)
{
    int index;
    if (n_args == 0) {
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        if (!fdir_exist(mp_obj_str_get_str(arg[index]))) {
            fdelete_by_name(mp_obj_str_get_str(arg[index]));
        }
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_posix_rmdir_obj, 0, mp_posix_rmdir);

/** 指定路径时需要指定完整路径，如：storage/sd0/C/name.txt*/
mp_obj_t mp_posix_stat(mp_obj_t path_in)
{
    const char *createpath = mp_obj_str_get_str(path_in);

    FILE *f = fopen(createpath, "r");
    if (f == NULL) {
        mp_raise_OSError(MP_EEXIST);
    }

    struct vfs_attr attr = {0};
    fget_attrs(f, &attr);

    mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(2, NULL));
    t->items[0] = MP_OBJ_NEW_SMALL_INT(attr.attr); // st_mode
    t->items[1] = mp_obj_new_int_from_uint(attr.fsize); // st_size
    //t->items[2] = MP_OBJ_NEW_SMALL_INT(); // st_mtime
    //t->items[3] = MP_OBJ_NEW_SMALL_INT(); // st_ctime
    fclose(f);
    return MP_OBJ_FROM_PTR(t);
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_stat_obj, mp_posix_stat);

mp_import_stat_t mp_posix_import_stat(const char *path)
{
    log_info("Not supported ");
    return MP_IMPORT_STAT_NO_EXIST;
}

#endif //MICROPY_MODUOS_FILE

