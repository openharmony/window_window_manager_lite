/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GRAPHIC_LITE_INPUT_MANAGER_SERVICE_H
#define GRAPHIC_LITE_INPUT_MANAGER_SERVICE_H

#include <pthread.h>
#include <queue>
#include "input_event_distributer.h"
#include "input_event_hub.h"

namespace OHOS {
/** @brief Manager service of input event. */
class InputManagerService {
public:
    /**
     * @brief Get instance of input manager service.
     *
     * @returns instance of input manager service
     */
    static InputManagerService* GetInstance();

    /**
     * Init input manager service.
     */
    static void Run();

    /**
     * Delete input manager service.
     */
    static void Stop();

    /**
     * @brief Distribute task function.
     *
     * @param args no use
     */
    static void* Distribute(void* args);

    /**
     * @brief Get input event distributer in service.
     *
     * @returns input event distributer
     */
    InputEventDistributer* GetDistributer()
    {
        return &distributer_;
    }

    /**
     * @brief Get input event hub in service.
     *
     * @returns input event hub
     */
    InputEventHub* GetHub()
    {
        return hub_;
    }

private:
    static void ReadCallback(const RawEvent* event);
    InputManagerService()
    {
        pthread_mutex_init(&lock_, nullptr);
        pthread_cond_init(&nonEmpty_, nullptr);
        pthread_cond_init(&nonFull_, nullptr);
    }
    ~InputManagerService();

    InputManagerService(const InputManagerService&) = delete;
    InputManagerService& operator=(const InputManagerService&) = delete;
    InputManagerService(InputManagerService&&) = delete;
    InputManagerService& operator=(InputManagerService&&) = delete;

    static InputEventHub* hub_;
    static InputEventDistributer distributer_;
    static pthread_t distributerThread_;
    static int32_t distributerThreadCreated_;
    static std::queue<RawEvent> eventQueue_;
    static pthread_mutex_t lock_;
    static pthread_cond_t nonEmpty_;
    static pthread_cond_t nonFull_;
};
} // namespace OHOS
#endif // GRAPHIC_LITE_INPUT_MANAGER_SERVICE_H
