/**
 * @file tuya_os_adapt_output.c
 * @brief 日志操作接口
 *
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
#define _UNI_OUTPUT_GLOBAL
#include "system/includes.h"
#include "tuya_os_adapt_output.h"
#include "tuya_os_adapter_error_code.h"


/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/
static const TUYA_OS_OUTPUT_INTF m_tuya_os_output_intfs = {
    .output_log     = tuya_os_adapt_output_log,
    .log_close      = tuya_os_adapt_log_close,
    .log_open       = tuya_os_adapt_log_open,
};


/***********************************************************
*************************function define********************
***********************************************************/
/**
 * @brief tuya_os_adapt_output_log用于输出log信息
 *
 * @param[in] str log buffer指针
 */
void tuya_os_adapt_output_log(const signed char *str)
{
    if (str == ((void *)(0))) {
        return;
    }
    printf(str);
}


/**
 * @brief 用于关闭原厂sdk默认的log口
 *
 */
int tuya_os_adapt_log_close(void)
{
    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief 用于恢复原厂sdk默认的log口
 *
 */
int tuya_os_adapt_log_open(void)
{
    return OPRT_OS_ADAPTER_OK;
}

//注册函数
int tuya_os_adapt_reg_output_intf(void)
{
    return tuya_os_adapt_reg_intf(INTF_OUTPUT, (void *)&m_tuya_os_output_intfs);
}

