/*
* Copyright (c) 2022, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file     encode_hevc_vdenc_pipeline_xe_xpm_base.h
//! \brief    Defines the interface for hevc vdenc encode pipeline
//!
#ifndef __ENCODE_HEVC_VDENC_PIPELINE_XE_XPM_BASE_H__
#define __ENCODE_HEVC_VDENC_PIPELINE_XE_XPM_BASE_H__

#include "encode_hevc_vdenc_pipeline.h"
#include "encode_hevc_vdenc_feature_manager_xe_xpm_base.h"

namespace encode {

class HevcVdencPipelineXe_Xpm_Base : public HevcVdencPipeline
{
public:
    //!
    //! \brief  HevcVdencPipelineXe_Xpm_Base constructor
    //! \param  [in] hwInterface
    //!         Pointer to CodechalHwInterface
    //! \param  [in] debugInterface
    //!         Pointer to CodechalDebugInterface
    //!
    HevcVdencPipelineXe_Xpm_Base(
        CodechalHwInterface    *hwInterface,
        CodechalDebugInterface *debugInterface);

    virtual ~HevcVdencPipelineXe_Xpm_Base() {}

    virtual MOS_STATUS Init(void *settings) override;

    virtual MOS_STATUS Prepare(void *params) override;

    virtual MOS_STATUS Execute() override;

    virtual MOS_STATUS GetStatusReport(void *status, uint16_t numStatus) override;

    virtual MOS_STATUS Destroy() override;

    virtual MOS_STATUS InitMmcState();

protected:
    virtual MOS_STATUS Initialize(void *settings) override;
    virtual MOS_STATUS Uninitialize() override;
    virtual MOS_STATUS ResetParams();

    virtual MOS_STATUS GetSystemVdboxNumber() override;

    virtual MOS_STATUS UserFeatureReport() override;

    virtual MOS_STATUS CreateFeatureManager() override;

    static const uint32_t       m_brcHistoryBufSize = 2304;            //!< BRC history buffer size
MEDIA_CLASS_DEFINE_END(encode__HevcVdencPipelineXe_Xpm_Base)
};

}
#endif // !__ENCODE_HEVC_VDENC_PIPELINE_XE_XPM_BASE_H__
