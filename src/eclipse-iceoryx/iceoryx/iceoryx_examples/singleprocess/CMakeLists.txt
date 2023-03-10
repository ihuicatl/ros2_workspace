# Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
# Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

# Build singleprocess example
cmake_minimum_required(VERSION 3.16)
project(example_singleprocess)

include(GNUInstallDirs)

set(ICEORYX_SINGLE_PROCESS_STACK_SIZE 3500000)

find_package(iceoryx_posh CONFIG REQUIRED)
find_package(iceoryx_hoofs CONFIG REQUIRED)

get_target_property(ICEORYX_CXX_STANDARD iceoryx_posh::iceoryx_posh CXX_STANDARD)
include(IceoryxPlatform)

add_executable(single_process ./single_process.cpp)
target_link_libraries(single_process
    iceoryx_posh::iceoryx_posh
    iceoryx_posh::iceoryx_posh_roudi
)
set_target_properties(single_process PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD ${ICEORYX_CXX_STANDARD}
    POSITION_INDEPENDENT_CODE ON
)
target_compile_options(single_process PRIVATE ${ICEORYX_WARNINGS} ${ICEORYX_SANITIZER_FLAGS})

# Since we have one process where multiple threads are created we need to increase the stack size for some OS types
if(QNX)
    target_link_options(single_process BEFORE PRIVATE -Wl,-z,stack-size=${ICEORYX_SINGLE_PROCESS_STACK_SIZE})
elseif(WIN32)
    target_link_options(single_process BEFORE PRIVATE /STACK:${ICEORYX_SINGLE_PROCESS_STACK_SIZE})
endif()

install(
    TARGETS single_process
    RUNTIME DESTINATION bin
)
