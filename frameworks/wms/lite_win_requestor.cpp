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

#include "lite_win_requestor.h"
#include "gfx_utils/graphic_log.h"
#include "lite_wm_type.h"
#include "lite_wms_client.h"
#include "surface_impl.h"

namespace OHOS {
LiteWinRequestor::LiteWinRequestor(int32_t id) : proxy_(nullptr), id_(id), surface_(nullptr)
{
    proxy_ = LiteWMSClient::GetInstance()->GetClientProxy();
}

LiteWinRequestor::~LiteWinRequestor()
{
    if (surface_ != nullptr) {
        delete surface_;
        surface_ = nullptr;
    }
}

int LiteWinRequestor::Callback(void* owner, int code, IpcIo* reply)
{
    if (code != 0) {
        GRAPHIC_LOGE("callback error, code = %d", code);
        return -1;
    }

    if (owner == nullptr) {
        return -1;
    }

    CallBackPara* para = static_cast<CallBackPara*>(owner);
    GRAPHIC_LOGD("Callback, funcId = %d", para->funcId);
    switch (para->funcId) {
        case LiteWMS_GetSurface: {
            LiteWinRequestor* requestor = static_cast<LiteWinRequestor*>(para->data);
            int32_t ret;
            ReadInt32(reply, &ret);
            if ((ret == LiteWMS_EOK) && (requestor != nullptr)) {
                requestor->GenericSurface(reply);
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

void LiteWinRequestor::GenericSurface(IpcIo* reply)
{
    surface_ = SurfaceImpl::GenericSurfaceByIpcIo(*reply);
}

Surface* LiteWinRequestor::GetSurface()
{
    if (surface_ == nullptr) {
        IpcIo io;
        uint8_t tmpData[DEFAULT_IPC_SIZE];
        IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
        WriteInt32(&io, id_);

        CallBackPara para = {};
        para.funcId = LiteWMS_GetSurface;
        para.data = this;
        int32_t ret = proxy_->Invoke(proxy_, LiteWMS_GetSurface, &io, &para, Callback);
        if (ret != 0) {
            GRAPHIC_LOGE("GetSurface failed, ret=%d", ret);
        }
    }
    return surface_;
}

void LiteWinRequestor::Show()
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_Show, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("Show failed, ret=%d", ret);
    }
}

void LiteWinRequestor::Hide()
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_Hide, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("Hide failed, ret=%d", ret);
    }
}

void LiteWinRequestor::RaiseToTop()
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_RaiseToTop, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("RaiseToTop failed, ret=%d", ret);
    }
}

void LiteWinRequestor::LowerToBottom()
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_LowerToBottom, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("LowerToBottom failed, ret=%d", ret);
    }
}

void LiteWinRequestor::MoveTo(int16_t x, int16_t y)
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);
    WriteUint32(&io, x);
    WriteUint32(&io, y);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_MoveTo, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("MoveTo failed, ret=%d", ret);
    }
}

void LiteWinRequestor::Resize(int16_t width, int16_t height)
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);
    WriteUint32(&io, width);
    WriteUint32(&io, height);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_Resize, &io, NULL, Callback);
    if (ret != 0) {
        GRAPHIC_LOGE("Resize failed, ret=%d", ret);
    }
}

void LiteWinRequestor::Update()
{
    IpcIo io;
    uint8_t tmpData[DEFAULT_IPC_SIZE];
    IpcIoInit(&io, tmpData, DEFAULT_IPC_SIZE, 0);
    WriteInt32(&io, id_);

    int32_t ret = proxy_->Invoke(proxy_, LiteWMS_Update, &io, NULL, NULL);
    if (ret != 0) {
        GRAPHIC_LOGE("Update failed, ret=%d", ret);
    }
}
} // namespace OHOS
