#
# Copyright (2017) Baidu Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

CUSTOMER ?= linux

COMPILER := linux

CFLAGS += -D_GNU_SOURCE -lrt
CFLAGS += -std=c99 -Wall
#(begin)below flags use to measure the stack usage
#CFLAGS += -m32
#CFLAGS += -fstack-usage
#LDFLAGS += -m32
#(begin)below flags use to measure the stack usage

TARGET := linux-demo

COM_DEFS := MBEDTLS_CONFIG_FILE=\"baidu_ca_mbedtls_config.h\"

LDFLAGS += -Wl,-Map,${OUT_DIR}/${TARGET}.map

# open this if want to use the AES-CBC encrypted communication
#COM_DEFS += NET_TRANS_ENCRYPTED_BY_AES_CBC

#=====start modules select=======#
modules_module_Device_Info=n
modules_module_System_Info=y
modules_module_HTTP=n
modules_module_OTA=n
modules_module_coap=y
modules_module_connagent=y
modules_module_dcs=y
modules_module_ntp=y
modules_module_voice_engine=y

platform_module_platform=n
platform_module_port-freertos=n
platform_module_port-linux=y
platform_module_port-lpb100=n

module_framework=y

external_module_mbedtls=y
external_module_nsdl=y
external_module_cjson=y
external_module_speex=y
#=====end modules select=======#
