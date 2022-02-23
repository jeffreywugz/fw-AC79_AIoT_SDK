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

##
# Build for demo
#

include $(CLEAR_VAR)

MODULE_PATH := $(BASE_DIR)/examples/speex-encoder

LOCAL_MODULE := speex-encoder

LOCAL_STATIC_LIBRARIES := voice_engine device_status connagent coap framework port-linux speex cjson mbedtls nsdl

LOCAL_SRC_FILES := $(wildcard $(MODULE_PATH)/*.c)

LOCAL_LDFLAGS := -lm -lrt -lpthread -lvoice_engine -ldevice_status -lframework

include $(BUILD_EXECUTABLE)

