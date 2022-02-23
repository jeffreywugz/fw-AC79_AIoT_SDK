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
// Author: Su Hao(suhao@baidu.com)
//
// Description: A common events handler for internal call in the task.

#include "duerapp_config.h"
#include "lightduer_event_emitter.h"

#ifndef DUER_EMTTER_QUEUE_LENGTH
#define DUER_EMTTER_QUEUE_LENGTH    (64)
#endif

#ifndef DUER_EMTTER_STACK_SIZE
#define DUER_EMTTER_STACK_SIZE      (1024 * 6)
#endif

static duer_events_handler g_events_handler = NULL;
static duer_bool emitter_request_destroy_flag = 0;

void duer_emitter_create(void)
{
    if (g_events_handler == NULL) {
        g_events_handler = duer_events_create("lightduer_ca", DUER_EMTTER_STACK_SIZE, DUER_EMTTER_QUEUE_LENGTH);
    }
}

int duer_emitter_emit(duer_events_func func, int what, void *object)
{
    if (g_events_handler) {
        return duer_events_call(g_events_handler, func, what, object);
    }
    return DUER_ERR_FAILED;
}

void duer_emitter_request_destroy(void)
{
    emitter_request_destroy_flag = 1;
}

void duer_emitter_destroy(void)
{
    duer_events_handler handler = NULL;

    if (g_events_handler) {
        handler = g_events_handler;
        g_events_handler = NULL;
        duer_events_destroy(handler);
        emitter_request_destroy_flag = 0;
    }
}

duer_bool duer_check_emitter_if_destroy(void)
{
    return emitter_request_destroy_flag;
}
