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

#include "lite_proxy_windows_manager.h"

#include "gfx_utils/color.h"
#include "gfx_utils/graphic_log.h"
#include "lite_wm_requestor.h"
#include "lite_wms_client.h"

namespace OHOS {
int LiteProxyWindowsManager::Init()
{
    GRAPHIC_LOGI("Init");
    LiteWMSClient::GetInstance()->InitLiteWMSClient();
    LiteWMRequestor* requestor = LiteWMRequestor::GetInstance();
    requestor->ClientRegister();
    requestor->GetLayerInfo();
    return 0;
}

IWindow* LiteProxyWindowsManager::CreateWindow(const LiteWinConfig& config)
{
    LiteWinRequestor* requstor = LiteWMRequestor::GetInstance()->CreateWindow(config);
    if (requstor == nullptr) {
        return nullptr;
    } else {
        return new LiteProxyWindow(requstor);
    }
}

void LiteProxyWindowsManager::RemoveWindow(IWindow* win)
{
    if (win != nullptr) {
        int32_t id = win->GetWindowId();
        delete win;
        LiteWMRequestor::GetInstance()->RemoveWindow(id);
    }
}

void LiteProxyWindowsManager::GetEventData(DeviceData* data)
{
    LiteWMRequestor::GetInstance()->GetEventData(data);
}

void LiteProxyWindowsManager::Screenshot()
{
    LiteWMRequestor::GetInstance()->Screenshot();
}

void LiteProxyWindowsManager::SetScreenshotListener(ScreenshotListener* listener)
{
    LiteWMRequestor::GetInstance()->SetScreenshotListener(listener);
}
} // namespace OHOS