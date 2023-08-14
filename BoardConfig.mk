# Copyright (C) 2023 Paranoid Android
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

DEVICE_PATH := device/oneplus/oneplus9rt

# HIDL
DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE += $(DEVICE_PATH)/configs/vintf/device_framework_compatibility_matrix.xml

# Inherit from sm8350-common
include device/oneplus/sm8350-common/BoardConfigCommon.mk

# Include proprietary files
include vendor/oneplus/oneplus9rt/BoardConfigVendor.mk

# OTA
TARGET_OTA_ASSERT_DEVICE := oneplus9rt,OP5154L1,OP5155L1
