// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "common/stopwatch.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

using namespace doris::cloud;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(StopWatchTest, SimpleTest) {
    {
        constexpr int64_t kSleepUs = 1000;
        constexpr int64_t kSleepJitterUs = 5000;
        constexpr int64_t kPauseDriftUs = 1000;

        StopWatch s;
        s.start();
        std::this_thread::sleep_for(std::chrono::microseconds(kSleepUs));
        const auto elapsed_after_start = s.elapsed_us();
        ASSERT_TRUE(elapsed_after_start >= kSleepUs &&
                    elapsed_after_start < kSleepUs + kSleepJitterUs);

        s.pause();
        std::this_thread::sleep_for(std::chrono::microseconds(kSleepUs));
        const auto elapsed_while_paused = s.elapsed_us();
        ASSERT_TRUE(elapsed_while_paused >= elapsed_after_start &&
                    elapsed_while_paused < elapsed_after_start + kPauseDriftUs);

        s.resume();
        std::this_thread::sleep_for(std::chrono::microseconds(kSleepUs));
        const auto elapsed_after_resume = s.elapsed_us();
        ASSERT_TRUE(elapsed_after_resume >= elapsed_while_paused + kSleepUs &&
                    elapsed_after_resume <
                            elapsed_while_paused + kSleepUs + kSleepJitterUs);

        s.reset();
        std::this_thread::sleep_for(std::chrono::microseconds(kSleepUs));
        const auto elapsed_after_reset = s.elapsed_us();
        ASSERT_TRUE(elapsed_after_reset >= kSleepUs &&
                    elapsed_after_reset < kSleepUs + kSleepJitterUs);
    }
}