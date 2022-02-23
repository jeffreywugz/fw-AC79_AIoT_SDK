#ifndef LBUF_H
#define LBUF_H


#include "typedef.h"
#include "list.h"
#include "system/spinlock.h"


/**
 * @brief lbuf头部信息
 */
struct lbuff_head {
    int magic_a; /*!<  测试验证变量*/
    struct list_head head;  /*!<  指向hentry链表*/
    struct list_head free;  /*!<  指向hfree链表*/
    spinlock_t lock;  /*!<  混合自旋锁,单核是为开关临界区,多核是自旋锁.*/
    u8 align;  /*!<  数据包字节对齐*/
    u16 priv_len;  /*!<  数据包结构体的最小长度*/
    u32 total_size;  /*!<  总大小*/
    u32 last_addr;  /*!<  指向free链表中找到的足够长度的hfree结构体地址*/
    void *priv;
    int magic_b;  /*!<  测试验证变量*/
};

/**
 * @brief lbuf状态
 */
struct lbuff_state {
    u32 avaliable;  /*!<  剩余空间的字节长度*/
    u32 fragment;  /*!<  lbuf内存碎片块数量*/
    u32 max_continue_len;  /*!<  最大的剩余内存块的字节长度*/
    int num;  /*!<  剩余内存块数量*/
};

/* --------------------------------------------------------------------------*/
/**
 * @brief 链表buf初始化
 *
 * @param [in] buf 需要lbuf进行管理的内存
 * @param [in] len 内存长度
 * @param [in] align 输入对管理的内存进行对齐的参数,避免后续使用因地址不对齐产生碎片
 * @param [in] priv_head_len 要管理的一个数据包结构体的最小的长度
 *
 * @return lbuf操作句柄
 */
/* --------------------------------------------------------------------------*/
struct lbuff_head *lbuf_init(void *buf, u32 len, int align, int priv_head_len);

/* --------------------------------------------------------------------------*/
/**
 * @brief 分配内存空间进行存储数据包
 *
 * @param [in] head lbuf操作句柄
 * @param [in] len 需要存入的数据包的长度
 *
 * @return 成功则返回进行存储数据包的地址,调用时候需要用户把该块内存的类型初始化为数据包结构体的类型。失败则返回NULL。
 */
/* --------------------------------------------------------------------------*/
void *lbuf_alloc(struct lbuff_head *head, u32 len);

/* --------------------------------------------------------------------------*/
/**
 * @brief 重新分配lbuf_alloc()返回用于存储数据包的lbuf空间
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 * @param [in] size 需重新分配的空间的字节长度.注:size的大小只能比lbuf_alloc()中的len小,即只能重新分配更小的lbuf空间,不能扩大空间.
 *
 * @return 重新分配后用于存储数据包的地址。失败则返回空指针。注:重新分配最好使用lbuf_real_size()获取lbuf空间的长度确认是否分配成功
 */
/* --------------------------------------------------------------------------*/
void *lbuf_realloc(void *lbuf, int size);

/* --------------------------------------------------------------------------*/
/**
 * @brief 判断lbuf空间内的内容是否为空
 *
 * @param [in] head lbuf操作句柄
 *
 * @return 返回1则为空,0则不为空
 */
/* --------------------------------------------------------------------------*/
int lbuf_empty(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 清空lbuf空间内进行已经分配给数据包的空间
 *
 * @param [in] head lbuf操作句柄
 */
/* --------------------------------------------------------------------------*/
void lbuf_clear(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 把数据包写入分配好的lbuf区域
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 * @param [in] channel_map 选择映射到哪个通道,最多8个通道,使用位映射的方式进行通道对应.
 */
/* --------------------------------------------------------------------------*/
void lbuf_push(void *lbuf, u8 channel_map);

/* --------------------------------------------------------------------------*/
/**
 * @brief 读取对应的通道映射的lbuf区域存储的内容
 *
 * @param [in] head lbuf操作句柄
 * @param [in] channel 需要读取的通道值,一般使用BIT(n),n为需要读取的通道
 *
 * @return 成功则返回存储对应的通道映射的数据包的地址
 */
/* --------------------------------------------------------------------------*/
void *lbuf_pop(struct lbuff_head *head, u8 channel);

/* --------------------------------------------------------------------------*/
/**
 * @brief 释放存储数据包的lbuf空间
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 *
 * @return 0则释放失败，存在地址越界操作或者通道还没有被读完，ref-1,读完后才能完全释放。1则释放成功。
 */
/* --------------------------------------------------------------------------*/
int lbuf_free(void *lbuf);

/* --------------------------------------------------------------------------*/
/**
 * @brief 用于调试,检查是否可以释放存储数据包的lbuf空间
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 * @param [in] rets 调用lbuf_free_check()函数的返回地址rets,取值可参考lbuf_free()
 */
/* --------------------------------------------------------------------------*/
void lbuf_free_check(void *lbuf, u32 rets);

/* --------------------------------------------------------------------------*/
/**
 * @brief 返回可分配的用来存储数据包的最大lbuf内存空间
 *
 * @param [in] head lbuf操作句柄
 *
 * @return 可分配的最大lbuf内存空间的字节长度
 */
/* --------------------------------------------------------------------------*/
u32 lbuf_free_space(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 获取lbuf空间的状态
 *
 * @param [in] head lbuf操作句柄
 * @param [out] state lbuff_state结构体
 */
/* --------------------------------------------------------------------------*/
void lbuf_state(struct lbuff_head *head, struct lbuff_state *state);

/* --------------------------------------------------------------------------*/
/**
 * @brief lbuf信息打印
 *
 * @param [in] head lbuf操作句柄
 */
/* --------------------------------------------------------------------------*/
void lbuf_dump(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 获取已经存入lbuf空间的数据包的数量
 *
 * @param [in] head lbuf操作句柄
 *
 * @return lbuf存储的数据包的数量
 */
/* --------------------------------------------------------------------------*/
int lbuf_traversal(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 返回lbuf空间还可以被写入size大小数据包的数量
 *
 * @param [in] head lbuf操作句柄
 * @param [in] size 欲检测写入数据包的大小
 *
 * @return 可以写入的数量
 */
/* --------------------------------------------------------------------------*/
int lbuf_avaliable(struct lbuff_head *head, int size);

/* --------------------------------------------------------------------------*/
/**
 * @brief 返回给数据包分配的内存空间的大小
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 *
 * @return 实际占用空间的字节长度
 */
/* --------------------------------------------------------------------------*/
int lbuf_real_size(void *lbuf);

/* --------------------------------------------------------------------------*/
/**
 * @brief 计算lbuf空间剩下多少剩余空间
 *
 * @param [in] head lbuf操作句柄
 *
 * @return 剩余空间的字节长度
 */
/* --------------------------------------------------------------------------*/
int lbuf_remain_space(struct lbuff_head *head);

/* --------------------------------------------------------------------------*/
/**
 * @brief 需要被重复释放的次数+1
 *
 * @param [in] lbuf lbuf_alloc()返回用于存储数据包的地址
 */
/* --------------------------------------------------------------------------*/
void lbuf_inc_ref(void *lbuf);


/// \cond DO_NOT_DOCUMENT
/**********************************
 *  设备读写对应的lbuf管理
 *
 *
 * *******************************/
struct dev_lbuf_map {
    void *head_buf; /*管理的head buffer*/
    u32  head_len;/*头的大小*/
    u32  addr; /*设备地址*/
    u32  len; /*设备长度*/
    int  align; /*对齐长度*/
    int  dev_align;
    int  priv_len;
};

u32 dlbuf_mapping(void *lbuf);

struct lbuff_head *dlbuf_init(struct dev_lbuf_map *map);

void *dlbuf_alloc(struct lbuff_head *head, u32 len);

void *dlbuf_realloc(struct lbuff_head *head, void *lbuf, int size);

int dlbuf_empty(struct lbuff_head *head);

void dlbuf_clear(struct lbuff_head *head);

void dlbuf_push(struct lbuff_head *head, void *lbuf, u8 channel_map);

void dlbuf_repush(struct lbuff_head *head, void *lbuf, u8 channel_map);

void *dlbuf_pop(struct lbuff_head *head, u8 channel);

void dlbuf_free(struct lbuff_head *head, void *lbuf);

u32 dlbuf_free_space(struct lbuff_head *head);

void dlbuf_state(struct lbuff_head *head, struct lbuff_state *state);

void dlbuf_dump(struct lbuff_head *head);

int dlbuf_traversal(struct lbuff_head *head);
/// \endcond

#endif


