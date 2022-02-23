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
 * File: lightduer_priority_conf.c
 * Auth: Su Hao (suhao@baidu.com)
 * Desc: Configuation the task priority.
 */

#include <string.h>
#include "lightduer_priority_conf.h"
#include "hsf.h"

static int g_task_priorities[DUER_TASK_TOTAL] = {
#if defined(DUER_PLATFORM_MARVELL)
    2, 3, 2
#else
    HFTHREAD_PRIORITIES_MID, HFTHREAD_PRIORITIES_MID, HFTHREAD_PRIORITIES_MID
#endif
};

static const char *const g_priority_tags[] = {
    "lightduer_ca",
    "lightduer_voice",
    "lightduer_socket",
};

int duer_priority_get_task_id(const char *name)
{
    int i = 0;
    for (i = 0; i < DUER_TASK_TOTAL; i ++) {
        if (strcmp(g_priority_tags[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

void duer_priority_set(int task, int priority)
{
    if (task >= 0 && task < DUER_TASK_TOTAL) {
        g_task_priorities[task] = priority;
    }
}

int duer_priority_get(int task)
{
    return (task >= 0 && task < DUER_TASK_TOTAL) ? g_task_priorities[task] : -1;
}
