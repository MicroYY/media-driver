/*
* Copyright (c) 2021, Intel Corporation
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
//! \file     decode_mpeg2_feature_manager.cpp
//! \brief    Defines the common interface for mpeg2 decode feature manager
//! \details  The mpeg2 decode feature manager is further sub-divided by codec type
//!           this file is for the base interface which is shared by all components.
//!

#include "decode_mpeg2_feature_manager.h"
#include "decode_mpeg2_basic_feature.h"
#include "decode_utils.h"

namespace decode
{

MOS_STATUS DecodeMpeg2FeatureManager::CreateFeatures(void *codecSettings)
{
    DECODE_FUNC_CALL();

    DECODE_CHK_STATUS(DecodeFeatureManager::CreateFeatures(codecSettings));

    auto decBasic = MOS_New(Mpeg2BasicFeature, m_allocator, m_hwInterface);
    DECODE_CHK_NULL(decBasic);
    DECODE_CHK_STATUS(RegisterFeatures(FeatureIDs::basicFeature, decBasic));

    return MOS_STATUS_SUCCESS;
}

}
