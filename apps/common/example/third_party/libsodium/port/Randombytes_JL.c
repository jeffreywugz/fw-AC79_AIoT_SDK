# include "randombytes.h"
# include "cpu.h"

//采用自定义随机数生成器
static const char *
randombytes_JL_implementation_name(void)
{
    return "JLsysrandom";
}

static uint32_t
JL_sysrandom(void)
{
    return (uint32_t)rand32();
}

static void
randombytes_JL_ysrandom_buf(void *const buf, const size_t size)
{
    uint8_t *r = (uint8_t *)buf;
    size_t i;

    for (i = 0; i < size; i++) {
        r[i] = JL_sysrandom();
    }
}

//在randombytes.c中已经选择为默认设置
//不需要再采用int randombytes_set_implementation(randombytes_implementation *impl)进行设置
const struct randombytes_implementation randombytes_JL_implementation = {
    .implementation_name = randombytes_JL_implementation_name,
    .random = JL_sysrandom,
    .stir = NULL,
    .uniform = NULL,
    .buf = randombytes_JL_ysrandom_buf,
    .close = NULL,
};
