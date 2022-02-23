#include "app_config.h"

#if TCFG_VIR_UPDATE_ENABLE

#include "system/includes.h"
#include "system/task.h"
#include "net_update.h"
#include "update/update_loader_download.h"
#include "dual_bank_updata_api.h"
#include "generic/errno-base.h"
#include "update.h"
#include "timer.h"
#include "fs/fs.h"
#include "os/os_api.h"

#include "device/device.h"
#include "system/task.h"
#include "asm/crc16.h"
#include "asm/efuse.h"
#include "dual_bank_updata_api.h"
#include "asm/wdt.h"
#include "generic/log.h"

typedef struct stJL_FILE_HEAD {
    u16 u16Crc;
    u16 u16DataCrc;
    u32 u32Address;
    u32 u32Length;

    u8 u8Attribute;
    u8 u8Res;
    u16 u16Index;
    char szFileName[16];
} JL_FILE_HEAD;

struct usb_update_hdl {
    u8 updateFlag;
    u32 fileSize;
    u32 firstSectorNum;
    void *fd;
};

#define BLOCK_SIZE 512u
static u32 total_sector_num = 0;

static struct usb_update_hdl *usb_update = NULL;

static void clean_usb_update(struct usb_update_hdl *hdl)
{
    if (NULL == hdl) {
        return;
    }

    memset(hdl, 0, sizeof(struct usb_update_hdl));
}

static int usb_update_check(JL_FILE_HEAD *head, const u8 *buf, const u32 len)
{
    int ret;

    if ((32 > len) || (NULL == head)) {
        return -1;
    }

    memcpy((u8 *)head, buf, sizeof(JL_FILE_HEAD));

    ret = jl_file_head_valid_check(head);
    if (ret) {
        return -1;
    }

    if (0 == strcmp((head->szFileName), "update_data")) {
        return 0;
    }

    return -1;
}

int ram_disk_update(const u8 *buf, const u32 len, const u32 offset)
{
    JL_FILE_HEAD head;
    int err = 0;
    int wLen = len * BLOCK_SIZE;
    u8 *wBuf = (u8 *)buf;

    if (0 == usb_update_check(&head, wBuf, wLen)) {
        u32 remain_len = 0;
        puts("Get update file");
        put_buf(wBuf, BLOCK_SIZE);

        usb_update = (struct usb_update_hdl *)malloc(sizeof(struct usb_update_hdl));
        if (NULL == usb_update) {
            return -1;
        }

        clean_usb_update(usb_update);

        usb_update->updateFlag = 1;

        usb_update->fd = net_fopen(CONFIG_UPGRADE_OTA_FILE_NAME, "w");
        if (NULL == usb_update->fd) {
            clean_usb_update(usb_update);
            printf("net_fopen error\n");
            free(usb_update)
            usb_update = NULL;
            return -1;
        }

        usb_update->fileSize = head.u32Length;
        total_sector_num = usb_update->fileSize / BLOCK_SIZE;

        remain_len = usb_update->fileSize % total_sector_num;
        if (remain_len) {
            total_sector_num++;
        }

        usb_update->firstSectorNum = offset;

    }

    if (usb_update && usb_update->updateFlag) {
        if (offset < usb_update->firstSectorNum || offset > (usb_update->firstSectorNum + total_sector_num - 1)) {
            return -1;
        }

        if (offset == (usb_update->firstSectorNum + total_sector_num - 1)) {
            wLen = usb_update->fileSize - ((total_sector_num - 1) * BLOCK_SIZE);
        }

        err = net_fwrite(usb_update->fd, wBuf, wLen, 0);
        if (err != wLen) {
            goto __err_;
        }

        if (offset == (usb_update->firstSectorNum + total_sector_num - 1)) {
            /* put_buf(wBuf, BLOCK_SIZE);	 */
            net_fclose(usb_update->fd, 0);
            clean_usb_update(usb_update);
            free(usb_update);
            usb_update = NULL;
        }

        return 0;
    }

__err_:
    if (usb_update) {
        net_fclose(usb_update->fd, -1);
        clean_usb_update(usb_update);
        free(usb_update);
        usb_update = NULL;
    }

    return -1;
}

#endif
