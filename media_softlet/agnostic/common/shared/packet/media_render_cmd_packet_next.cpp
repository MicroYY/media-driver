/*
* Copyright (c) 2021-2022, Intel Corporation
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
//! \file     media_render_cmd_packet_next.cpp
//! \brief    Defines the interface for media render workload cmd packet
//! \details  The media cmd packet is dedicated for command buffer sequenece submit
//!

#include "media_render_cmd_packet.h"
#include "mos_oca_interface.h"
#include "renderhal_platform_interface.h"
#include "hal_oca_interface.h"
#include "hal_oca_interface.h"
#include "mos_interface.h"
#include "mhw_mi_itf.h"
#include "mhw_mi_cmdpar.h"

#define COMPUTE_WALKER_THREAD_SPACE_WIDTH 1
#define COMPUTE_WALKER_THREAD_SPACE_HEIGHT 1
#define COMPUTE_WALKER_THREAD_SPACE_DEPTH 1

RenderCmdPacketNext::RenderCmdPacketNext(MediaTask* task, PMOS_INTERFACE pOsinterface, RENDERHAL_INTERFACE* renderHal) : CmdPacket(task),
m_renderHal(renderHal),
m_cpInterface(nullptr),
m_osInterface(pOsinterface)
{
    Init();
}

RenderCmdPacketNext::~RenderCmdPacketNext()
{
    Destroy();
}

MOS_STATUS RenderCmdPacketNext::Init()
{
    if (!m_renderHal)
    {
        m_renderHal = (PRENDERHAL_INTERFACE)MOS_AllocAndZeroMemory(sizeof(RENDERHAL_INTERFACE));
        RENDER_PACKET_CHK_NULL_RETURN(m_renderHal);
        RENDER_PACKET_CHK_STATUS_RETURN(RenderHal_InitInterface(
            m_renderHal,
            &m_cpInterface,
            m_osInterface));

        RENDERHAL_SETTINGS          RenderHalSettings;
        RenderHalSettings.iMediaStates = 32; // Init MEdia state values
        RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnInitialize(m_renderHal, &RenderHalSettings));
    }
    else
    {
        RENDER_PACKET_NORMALMESSAGE("RenderHal Already been created");
    }

    bool mediaWalkerUsed = false;
    bool computeWalkerUsed = false;
    mediaWalkerUsed = m_renderHal->pfnGetMediaWalkerStatus(m_renderHal) ? true : false;
    computeWalkerUsed = m_renderHal->pRenderHalPltInterface->IsComputeContextInUse(m_renderHal);

    if (mediaWalkerUsed && !computeWalkerUsed)
    {
        m_walkerType = WALKER_TYPE_MEDIA;
    }
    else if (computeWalkerUsed)
    {
        m_walkerType = WALKER_TYPE_COMPUTE;
    }
    else
    {
        m_walkerType = WALKER_TYPE_DISABLED;
    }

    if (m_renderHal->pRenderHalPltInterface)
    {
        m_miItf = m_renderHal->pRenderHalPltInterface->GetMhwMiItf();
    }

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::Destroy()
{
    MOS_STATUS eStatus = MOS_STATUS_SUCCESS;

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::RenderEngineSetup()
{
    // pls make sure the context already switched to render/compute engine before submit
    RENDER_PACKET_CHK_NULL_RETURN(m_renderHal);
    RENDER_PACKET_CHK_NULL_RETURN(m_renderHal->pStateHeap);

    // Register the resource of GSH
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnReset(m_renderHal));

    // Assign media state
    m_renderData.mediaState = m_renderHal->pfnAssignMediaState(m_renderHal, RENDERHAL_COMPONENT_PACKET);
    RENDER_PACKET_CHK_NULL_RETURN(m_renderData.mediaState);

    // Allocate and reset SSH instance
    if ((m_isMultiBindingTables == false) || (m_renderHal->pStateHeap->iCurrentBindingTable >= m_renderHal->StateHeapSettings.iBindingTables) ||
        (m_renderHal->pStateHeap->iCurrentSurfaceState >= m_renderHal->StateHeapSettings.iSurfaceStates))
    {
        RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnAssignSshInstance(m_renderHal));
    }

    // Assign and Reset binding table
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnAssignBindingTable(
        m_renderHal,
        &m_renderData.bindingTable));

    // Reset a new Binding index from start
    m_renderData.bindingTableEntry = 0;

    // load kernels before packet submit PipeLine to load kernel

    return MOS_STATUS_SUCCESS;
}


MOS_STATUS RenderCmdPacketNext::InitKernelEntry()
{
    if (m_kernelCount == 0)
    {
        RENDER_PACKET_NORMALMESSAGE("no kernel set up");
        return MOS_STATUS_LOAD_LIBRARY_FAILED;
    }

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::SetPowerMode(uint32_t KernelID)
{
    MOS_STATUS            eStatus                = MOS_STATUS_SUCCESS;
    uint16_t              wNumRequestedEUSlices  = 1;  // Default to 1 slice
    uint16_t              wNumRequestedSubSlices = 3;  // Default to 3 subslice
    uint16_t              wNumRequestedEUs       = 8;  // Default to 8 EUs
    RENDERHAL_POWEROPTION PowerOption;
    bool                  bSetRequestedSlices = false;
    const SseuSetting *   pcSSEUTable         = nullptr;

    RENDER_PACKET_CHK_NULL_RETURN(m_renderHal);

    if ((m_renderHal->bRequestSingleSlice) || (m_renderHal->bEUSaturationNoSSD))
    {
        bSetRequestedSlices = true;
        // bEUSaturationNoSSD: No slice shutdown, must request 2 slices [CM EU saturation on].
        // bRequestSingleSlice: Always single slice.
        wNumRequestedEUSlices = (m_renderHal->bEUSaturationNoSSD) ? 2 : 1;
    }
    else
    {
        bSetRequestedSlices = false;
    }

    if (m_renderHal->sseuTable)
    {
        pcSSEUTable = (const SseuSetting *)m_renderHal->sseuTable;
    }
    else
    {
        RENDER_PACKET_ASSERTMESSAGE("SSEU Table not valid.");
        return MOS_STATUS_UNKNOWN;
    }

    RENDER_PACKET_CHK_NULL_RETURN(pcSSEUTable);
    pcSSEUTable += KernelID;
    if (!bSetRequestedSlices)  // If num Slices is already programmed, then don't change it
    {
        if (wNumRequestedEUSlices < pcSSEUTable->numSlices)
        {
            wNumRequestedEUSlices = pcSSEUTable->numSlices;
        }
    }

    wNumRequestedSubSlices = pcSSEUTable->numSubSlices;
    wNumRequestedEUs       = pcSSEUTable->numEUs;

#if (_DEBUG || _RELEASE_INTERNAL)
    // User feature key reads
    MOS_USER_FEATURE_VALUE_DATA UserFeatureData;
    MOS_ZeroMemory(&UserFeatureData, sizeof(UserFeatureData));
    MOS_UserFeature_ReadValue_ID(
        nullptr,
        __MEDIA_USER_FEATURE_VALUE_SSEU_SETTING_OVERRIDE_ID,
        &UserFeatureData,
        m_osInterface->pOsContext);

    if (UserFeatureData.u32Data != 0xDEADC0DE)
    {
        wNumRequestedEUSlices  = UserFeatureData.u32Data & 0xFF;            // Bits 0-7
        wNumRequestedSubSlices = (UserFeatureData.u32Data >> 8) & 0xFF;     // Bits 8-15
        wNumRequestedEUs       = (UserFeatureData.u32Data >> 16) & 0xFFFF;  // Bits 16-31
    }
#endif

    PowerOption.nSlice    = wNumRequestedEUSlices;
    PowerOption.nSubSlice = wNumRequestedSubSlices;
    PowerOption.nEU       = wNumRequestedEUs;
    m_renderHal->pfnSetPowerOptionMode(m_renderHal, &PowerOption);

    return eStatus;
}

uint32_t RenderCmdPacketNext::SetSurfaceForHwAccess(PMOS_SURFACE surface, PRENDERHAL_SURFACE_NEXT pRenderSurface, PRENDERHAL_SURFACE_STATE_PARAMS pSurfaceParams, bool bWrite)
{
    PMOS_INTERFACE                 pOsInterface;
    PRENDERHAL_SURFACE_STATE_ENTRY pSurfaceEntries[MHW_MAX_SURFACE_PLANES];
    int32_t                        iSurfaceEntries;
    int32_t                        i;
    MOS_STATUS                     eStatus;
    RENDERHAL_SURFACE_STATE_PARAMS surfaceParams;

    // Initialize Variables
    eStatus      = MOS_STATUS_SUCCESS;
    pOsInterface = m_osInterface;

    RENDER_PACKET_CHK_NULL_RETURN(pRenderSurface);
    RENDER_PACKET_CHK_NULL_RETURN(pOsInterface);

    // Register surfaces for rendering (GfxAddress/Allocation index)
    // Register resource
    RENDER_PACKET_CHK_STATUS_RETURN(m_osInterface->pfnRegisterResource(
        m_osInterface,
        &surface->OsResource,
        bWrite,
        true));

    if (!pSurfaceParams)
    {
        MOS_ZeroMemory(&surfaceParams, sizeof(RENDERHAL_SURFACE_STATE_PARAMS));

        //set mem object control for cache
        surfaceParams.MemObjCtl = (m_renderHal->pOsInterface->pfnCachePolicyGetMemoryObject(
                                       MOS_MP_RESOURCE_USAGE_DEFAULT,
                                       m_renderHal->pOsInterface->pfnGetGmmClientContext(m_renderHal->pOsInterface))).DwordValue;

        pSurfaceParams = &surfaceParams;
    }

    if (pSurfaceParams->bAVS)
    {
        pSurfaceParams->Type = m_renderHal->SurfaceTypeAdvanced;
    }
    else
    {
        pSurfaceParams->Type = m_renderHal->SurfaceTypeDefault;
    }

    RENDER_PACKET_CHK_STATUS_RETURN(InitRenderHalSurface(
        *surface,
        pRenderSurface));

    if (bWrite)
    {
        pRenderSurface->SurfType = RENDERHAL_SURF_OUT_RENDERTARGET;
    }

    // Setup surface states-----------------------------------------------------
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetupSurfaceState(
        m_renderHal,
        pRenderSurface,
        pSurfaceParams,
        &iSurfaceEntries,  // for most cases, surface entry should only take 1 entry, need align with kerenl design
        pSurfaceEntries,
        nullptr));

    if (!m_isLargeSurfaceStateNeeded)
    {
        if (m_renderData.bindingTableEntry > 15)
        {
            RENDER_PACKET_ASSERTMESSAGE("input surface support up to 16 RSS");
            m_renderData.bindingTableEntry = 0;
        }
    }
    else
    {
        if (m_renderData.bindingTableEntry > 255)
        {
            RENDER_PACKET_ASSERTMESSAGE("input surface support up to 256 RSS");
            m_renderData.bindingTableEntry = 0;
        }
    }

    uint32_t iBTEntry = m_renderData.bindingTableEntry;
    // Bind surface states------------------------------------------------------
    for (i = 0; i < iSurfaceEntries; i++, m_renderData.bindingTableEntry++)
    {
        RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnBindSurfaceState(
            m_renderHal,
            m_renderData.bindingTable,
            m_renderData.bindingTableEntry,
            pSurfaceEntries[i]));

        pRenderSurface->Index = m_renderData.bindingTableEntry;
    }

    return iBTEntry;
}

uint32_t RenderCmdPacketNext::SetSurfaceForHwAccess(
    PMOS_SURFACE                    surface,
    PRENDERHAL_SURFACE_NEXT         pRenderSurface,
    PRENDERHAL_SURFACE_STATE_PARAMS pSurfaceParams,
    uint32_t                        bindingIndex,
    bool                            bWrite,
    PRENDERHAL_SURFACE_STATE_ENTRY *surfaceEntries,
    uint32_t *                      numOfSurfaceEntries)
{
    PMOS_INTERFACE                  pOsInterface                              = nullptr;
    PRENDERHAL_SURFACE_STATE_ENTRY  surfaceEntriesTmp[MHW_MAX_SURFACE_PLANES] = {};
    PRENDERHAL_SURFACE_STATE_ENTRY *pSurfaceEntries                           = nullptr;
    int32_t                         iSurfaceEntries                           = 0;
    int32_t                         i                                         = 0;
    MOS_STATUS                      eStatus                                   = MOS_STATUS_SUCCESS;
    RENDERHAL_SURFACE_STATE_PARAMS  surfaceParams                             = {};

    if (nullptr == surfaceEntries || nullptr == numOfSurfaceEntries)
    {
        pSurfaceEntries = surfaceEntriesTmp;
    }
    else
    {
        pSurfaceEntries = surfaceEntries;
    }

    // Initialize Variables
    eStatus      = MOS_STATUS_SUCCESS;
    pOsInterface = m_osInterface;

    RENDER_PACKET_CHK_NULL_RETURN(pRenderSurface);
    RENDER_PACKET_CHK_NULL_RETURN(pOsInterface);

    // Register surfaces for rendering (GfxAddress/Allocation index)
    // Register resource
    RENDER_PACKET_CHK_STATUS_RETURN(m_osInterface->pfnRegisterResource(
        m_osInterface,
        &surface->OsResource,
        bWrite,
        true));

    if (!pSurfaceParams)
    {
        MOS_ZeroMemory(&surfaceParams, sizeof(RENDERHAL_SURFACE_STATE_PARAMS));

        //set mem object control for cache
        surfaceParams.MemObjCtl = (m_renderHal->pOsInterface->pfnCachePolicyGetMemoryObject(
                                       MOS_MP_RESOURCE_USAGE_DEFAULT,
                                       m_renderHal->pOsInterface->pfnGetGmmClientContext(m_renderHal->pOsInterface))).DwordValue;

        pSurfaceParams = &surfaceParams;
    }

    if (pSurfaceParams->bAVS)
    {
        pSurfaceParams->Type = m_renderHal->SurfaceTypeAdvanced;
    }
    else
    {
        pSurfaceParams->Type = m_renderHal->SurfaceTypeDefault;
    }

    RENDER_PACKET_CHK_STATUS_RETURN(InitRenderHalSurface(
        *surface,
        pRenderSurface));

    if (bWrite)
    {
        pRenderSurface->SurfType = RENDERHAL_SURF_OUT_RENDERTARGET;
    }

    // Setup surface states-----------------------------------------------------
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetupSurfaceState(
        m_renderHal,
        pRenderSurface,
        pSurfaceParams,
        &iSurfaceEntries,  // for most cases, surface entry should only take 1 entry, need align with kerenl design
        pSurfaceEntries,
        nullptr));

    uint32_t iBTEntry = bindingIndex;
    // Bind surface states------------------------------------------------------
    for (i = 0; i < iSurfaceEntries; i++, iBTEntry++)
    {
        RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnBindSurfaceState(
            m_renderHal,
            m_renderData.bindingTable,
            iBTEntry,
            pSurfaceEntries[i]));

        pRenderSurface->Index = iBTEntry;
    }

    if (numOfSurfaceEntries)
    {
        *numOfSurfaceEntries = iSurfaceEntries;
    }

    return bindingIndex;
}


uint32_t RenderCmdPacketNext::SetBufferForHwAccess(PMOS_SURFACE buffer, PRENDERHAL_SURFACE_NEXT pRenderSurface, PRENDERHAL_SURFACE_STATE_PARAMS pSurfaceParams, bool bWrite)
{
    RENDERHAL_SURFACE              RenderHalSurface;
    RENDERHAL_SURFACE_STATE_PARAMS SurfaceParam;
    PRENDERHAL_SURFACE_STATE_ENTRY pSurfaceEntry;

    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface);
    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface->osCpInterface);
    RENDER_PACKET_CHK_NULL_RETURN(buffer);

    MOS_ZeroMemory(&RenderHalSurface, sizeof(RenderHalSurface));

    // Register surfaces for rendering (GfxAddress/Allocation index)
    // Register resource
    RENDER_PACKET_CHK_STATUS_RETURN(m_osInterface->pfnRegisterResource(
        m_osInterface,
        &buffer->OsResource,
        bWrite,
        true));

    // Setup Buffer surface-----------------------------------------------------
    if (pSurfaceParams == nullptr)
    {
        MOS_ZeroMemory(&SurfaceParam, sizeof(SurfaceParam));
        if (buffer->OsResource.mocsMosResUsageType == MOS_CODEC_RESOURCE_USAGE_BEGIN_CODEC ||
            buffer->OsResource.mocsMosResUsageType >= MOS_HW_RESOURCE_USAGE_MEDIA_BATCH_BUFFERS ||
            buffer->OsResource.memObjCtrlState.DwordValue == 0)
        {
            //set mem object control for cache
            SurfaceParam.MemObjCtl = (MosInterface::GetCachePolicyMemoryObject(
                                          m_renderHal->pOsInterface->pfnGetGmmClientContext(m_renderHal->pOsInterface),
                                          MOS_MP_RESOURCE_USAGE_DEFAULT)).DwordValue;
        }
        else
        {
            SurfaceParam.MemObjCtl = buffer->OsResource.memObjCtrlState.DwordValue;
        }

        pSurfaceParams = &SurfaceParam;
    }

    RENDER_PACKET_CHK_STATUS_RETURN(InitRenderHalSurface(
        *buffer,
        &RenderHalSurface));

    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetupBufferSurfaceState(
        m_renderHal,
        &RenderHalSurface,
        pSurfaceParams,
        &pSurfaceEntry));

    // Bind surface state-------------------------------------------------------
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnBindSurfaceState(
        m_renderHal,
        m_renderData.bindingTable,
        m_renderData.bindingTableEntry,
        pSurfaceEntry));

    pRenderSurface->Index = m_renderData.bindingTableEntry;

    m_renderData.bindingTableEntry++;
    return pRenderSurface->Index;
}

uint32_t RenderCmdPacketNext::SetBufferForHwAccess(PMOS_SURFACE buffer, PRENDERHAL_SURFACE_NEXT pRenderSurface, PRENDERHAL_SURFACE_STATE_PARAMS pSurfaceParams, uint32_t bindingIndex, bool bWrite)
{
    RENDERHAL_SURFACE              RenderHalSurface;
    RENDERHAL_SURFACE_STATE_PARAMS SurfaceParam;
    PRENDERHAL_SURFACE_STATE_ENTRY pSurfaceEntry;

    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface);
    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface->osCpInterface);
    RENDER_PACKET_CHK_NULL_RETURN(buffer);

    MOS_ZeroMemory(&RenderHalSurface, sizeof(RenderHalSurface));

    // Register surfaces for rendering (GfxAddress/Allocation index)
    // Register resource
    RENDER_PACKET_CHK_STATUS_RETURN(m_osInterface->pfnRegisterResource(
        m_osInterface,
        &buffer->OsResource,
        bWrite,
        true));

    // Setup Buffer surface-----------------------------------------------------
    if (pSurfaceParams == nullptr)
    {
        MOS_ZeroMemory(&SurfaceParam, sizeof(SurfaceParam));

        //set mem object control for cache
        SurfaceParam.MemObjCtl = (m_renderHal->pOsInterface->pfnCachePolicyGetMemoryObject(
                                      MOS_MP_RESOURCE_USAGE_DEFAULT,
                                      m_renderHal->pOsInterface->pfnGetGmmClientContext(m_renderHal->pOsInterface))).DwordValue;

        pSurfaceParams = &SurfaceParam;
    }

    RENDER_PACKET_CHK_STATUS_RETURN(InitRenderHalSurface(
        *buffer,
        &RenderHalSurface));

    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetupBufferSurfaceState(
        m_renderHal,
        &RenderHalSurface,
        pSurfaceParams,
        &pSurfaceEntry));

    uint32_t iBTEntry = bindingIndex;
    // Bind surface state-------------------------------------------------------
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnBindSurfaceState(
        m_renderHal,
        m_renderData.bindingTable,
        iBTEntry,
        pSurfaceEntry));

    pRenderSurface->Index = bindingIndex;

    return bindingIndex;
}

uint32_t RenderCmdPacketNext::SetBufferForHwAccess(MOS_BUFFER buffer, PRENDERHAL_SURFACE_NEXT pRenderSurface, PRENDERHAL_SURFACE_STATE_PARAMS pSurfaceParams, bool bWrite)
{
    RENDERHAL_SURFACE              RenderHalSurface;
    RENDERHAL_SURFACE_STATE_PARAMS SurfaceParam;
    PRENDERHAL_SURFACE_STATE_ENTRY pSurfaceEntry;

    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface);
    RENDER_PACKET_CHK_NULL_RETURN(m_osInterface->osCpInterface);

    MOS_ZeroMemory(&RenderHalSurface, sizeof(RenderHalSurface));

    // not support CP yet
    if (m_osInterface->osCpInterface->IsHMEnabled())
    {
        RENDER_PACKET_ASSERTMESSAGE("ERROR, need to use VpHal_CommonSetBufferSurfaceForHwAccess if under CP HM.");
    }

    // Register surfaces for rendering (GfxAddress/Allocation index)
    // Register resource
    RENDER_PACKET_CHK_STATUS_RETURN(m_osInterface->pfnRegisterResource(
        m_osInterface,
        &buffer.OsResource,
        bWrite,
        true));

    // Setup Buffer surface-----------------------------------------------------
    if (pSurfaceParams == nullptr)
    {
        MOS_ZeroMemory(&SurfaceParam, sizeof(SurfaceParam));

        //set mem object control for cache
        SurfaceParam.MemObjCtl = (m_renderHal->pOsInterface->pfnCachePolicyGetMemoryObject(
                                      MOS_MP_RESOURCE_USAGE_DEFAULT,
                                      m_renderHal->pOsInterface->pfnGetGmmClientContext(m_renderHal->pOsInterface))).DwordValue;

        pSurfaceParams = &SurfaceParam;
    }

    RENDER_PACKET_CHK_STATUS_RETURN(InitRenderHalBuffer(
        buffer,
        &RenderHalSurface));

    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetupBufferSurfaceState(
        m_renderHal,
        &RenderHalSurface,
        pSurfaceParams,
        &pSurfaceEntry));

    // Bind surface state-------------------------------------------------------
    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnBindSurfaceState(
        m_renderHal,
        m_renderData.bindingTable,
        m_renderData.bindingTableEntry,
        pSurfaceEntry));

    pRenderSurface->Index = m_renderData.bindingTableEntry;

    m_renderData.bindingTableEntry++;
    return pRenderSurface->Index;
}

MOS_STATUS RenderCmdPacketNext::SetupCurbe(void *pData, uint32_t curbeLength, uint32_t maximumNumberofThreads)
{
    m_renderData.iCurbeOffset = m_renderHal->pfnLoadCurbeData(
        m_renderHal,
        m_renderData.mediaState,
        pData,
        curbeLength);

    if (m_renderData.iCurbeOffset < 0)
    {
        RENDER_PACKET_ASSERTMESSAGE("Curbe Set Fail, return error");
        return MOS_STATUS_UNKNOWN;
    }

    m_renderData.iCurbeLength = curbeLength;

    RENDER_PACKET_CHK_STATUS_RETURN(m_renderHal->pfnSetVfeStateParams(
        m_renderHal,
        MEDIASTATE_DEBUG_COUNTER_FREE_RUNNING,
        maximumNumberofThreads,
        m_renderData.iCurbeLength,
        m_renderData.iInlineLength,
        nullptr));

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::PrepareMediaWalkerParams(KERNEL_WALKER_PARAMS params, MHW_WALKER_PARAMS &mediaWalker)
{
    uint32_t uiMediaWalkerBlockSize;
    RECT     alignedRect      = {};
    bool     bVerticalPattern = false;

    uiMediaWalkerBlockSize = m_renderHal->pHwSizes->dwSizeMediaWalkerBlock;
    alignedRect            = params.alignedRect;
    bVerticalPattern       = params.isVerticalPattern;

    // Calculate aligned output area in order to determine the total # blocks
    // to process in case of non-16x16 aligned target.
    alignedRect.right += uiMediaWalkerBlockSize - 1;
    alignedRect.bottom += uiMediaWalkerBlockSize - 1;
    alignedRect.left -= alignedRect.left % uiMediaWalkerBlockSize;
    alignedRect.top -= alignedRect.top % uiMediaWalkerBlockSize;
    alignedRect.right -= alignedRect.right % uiMediaWalkerBlockSize;
    alignedRect.bottom -= alignedRect.bottom % uiMediaWalkerBlockSize;

    if (params.calculateBlockXYByAlignedRect)
    {
        // Set number of blocks
        params.iBlocksX = (alignedRect.right - alignedRect.left) / uiMediaWalkerBlockSize;
        params.iBlocksY = (alignedRect.bottom - alignedRect.top) / uiMediaWalkerBlockSize;
    }

    // Set walker cmd params - Rasterscan
    mediaWalker.InterfaceDescriptorOffset = params.iMediaID;

    mediaWalker.dwGlobalLoopExecCount = 1;

    if (uiMediaWalkerBlockSize == 32)
    {
        mediaWalker.ColorCountMinusOne = 3;
    }
    else
    {
        mediaWalker.ColorCountMinusOne = 0;
    }

    if (alignedRect.left != 0 || alignedRect.top != 0)
    {
        // if the rect starts from any other macro  block other than the first
        // then the global resolution should be the whole frame and the global
        // start should be the rect start.
        mediaWalker.GlobalResolution.x =
            (alignedRect.right / uiMediaWalkerBlockSize);
        mediaWalker.GlobalResolution.y =
            (alignedRect.bottom / uiMediaWalkerBlockSize);
    }
    else
    {
        mediaWalker.GlobalResolution.x = params.iBlocksX;
        mediaWalker.GlobalResolution.y = params.iBlocksY;
    }

    mediaWalker.GlobalStart.x =
        (alignedRect.left / uiMediaWalkerBlockSize);
    mediaWalker.GlobalStart.y =
        (alignedRect.top / uiMediaWalkerBlockSize);

    mediaWalker.GlobalOutlerLoopStride.x = params.iBlocksX;
    mediaWalker.GlobalOutlerLoopStride.y = 0;

    mediaWalker.GlobalInnerLoopUnit.x = 0;
    mediaWalker.GlobalInnerLoopUnit.y = params.iBlocksY;

    mediaWalker.BlockResolution.x = params.iBlocksX;
    mediaWalker.BlockResolution.y = params.iBlocksY;

    mediaWalker.LocalStart.x = 0;
    mediaWalker.LocalStart.y = 0;

    if (bVerticalPattern)
    {
        mediaWalker.LocalOutLoopStride.x = 1;
        mediaWalker.LocalOutLoopStride.y = 0;

        mediaWalker.LocalInnerLoopUnit.x = 0;
        mediaWalker.LocalInnerLoopUnit.y = 1;

        mediaWalker.dwLocalLoopExecCount = params.iBlocksX - 1;
        mediaWalker.LocalEnd.x           = 0;
        mediaWalker.LocalEnd.y           = params.iBlocksY - 1;
    }
    else
    {
        mediaWalker.LocalOutLoopStride.x = 0;
        mediaWalker.LocalOutLoopStride.y = 1;

        mediaWalker.LocalInnerLoopUnit.x = 1;
        mediaWalker.LocalInnerLoopUnit.y = 0;

        mediaWalker.dwLocalLoopExecCount = params.iBlocksY - 1;
        mediaWalker.LocalEnd.x           = params.iBlocksX - 1;
        mediaWalker.LocalEnd.y           = 0;
    }

    mediaWalker.UseScoreboard  = m_renderHal->VfeScoreboard.ScoreboardEnable;
    mediaWalker.ScoreboardMask = m_renderHal->VfeScoreboard.ScoreboardMask;

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::PrepareComputeWalkerParams(KERNEL_WALKER_PARAMS params, MHW_GPGPU_WALKER_PARAMS &gpgpuWalker)
{
    uint32_t uiMediaWalkerBlockSize;
    RECT     alignedRect = {};
    // Get media walker kernel block size
    uiMediaWalkerBlockSize = m_renderHal->pHwSizes->dwSizeMediaWalkerBlock;
    alignedRect            = params.alignedRect;

    // Calculate aligned output area in order to determine the total # blocks
    // to process in case of non-16x16 aligned target.
    alignedRect.right += uiMediaWalkerBlockSize - 1;
    alignedRect.bottom += uiMediaWalkerBlockSize - 1;
    alignedRect.left -= alignedRect.left % uiMediaWalkerBlockSize;
    alignedRect.top -= alignedRect.top % uiMediaWalkerBlockSize;
    alignedRect.right -= alignedRect.right % uiMediaWalkerBlockSize;
    alignedRect.bottom -= alignedRect.bottom % uiMediaWalkerBlockSize;

    if (params.calculateBlockXYByAlignedRect)
    {
        // Set number of blocks
        params.iBlocksX = (alignedRect.right - alignedRect.left) / uiMediaWalkerBlockSize;
        params.iBlocksY = (alignedRect.bottom - alignedRect.top) / uiMediaWalkerBlockSize;
    }

    // Set walker cmd params - Rasterscan
    gpgpuWalker.InterfaceDescriptorOffset = params.iMediaID;

    // Specifies the initial value of the X component of the thread group when walker is started.
    // During the walker operation, when X is incremented to the X Dimension limit, on the next step
    // it is re-loaded with theStarting Xvalue. Same as GroupStartingY.
    gpgpuWalker.GroupStartingX = (alignedRect.left / uiMediaWalkerBlockSize);
    gpgpuWalker.GroupStartingY = (alignedRect.top / uiMediaWalkerBlockSize);
    // The X dimension of the thread group (maximum X is dimension -1), same as GroupHeight.
    gpgpuWalker.GroupWidth  = params.iBlocksX;
    gpgpuWalker.GroupHeight = params.iBlocksY;
    if (params.isGroupStartInvolvedInGroupSize)
    {
        gpgpuWalker.GroupWidth += gpgpuWalker.GroupStartingX;
        gpgpuWalker.GroupHeight += gpgpuWalker.GroupStartingY;
    }

    gpgpuWalker.ThreadWidth              = COMPUTE_WALKER_THREAD_SPACE_WIDTH;
    gpgpuWalker.ThreadHeight             = COMPUTE_WALKER_THREAD_SPACE_HEIGHT;
    gpgpuWalker.ThreadDepth              = COMPUTE_WALKER_THREAD_SPACE_DEPTH;
    gpgpuWalker.IndirectDataStartAddress = params.iCurbeOffset;
    // Indirect Data Length is a multiple of 64 bytes (size of L3 cacheline). Bits [5:0] are zero.
    gpgpuWalker.IndirectDataLength = MOS_ALIGN_CEIL(params.iCurbeLength, 1 << MHW_COMPUTE_INDIRECT_SHIFT);
    gpgpuWalker.BindingTableID     = params.iBindingTable;

    return MOS_STATUS_SUCCESS;
}

void RenderCmdPacketNext::UpdateKernelConfigParam(RENDERHAL_KERNEL_PARAM &kernelParam)
{
    // CURBE_Length is set as the size of curbe buffer. 32 alignment with 5 bits right shift need be done before it being used.
    kernelParam.CURBE_Length = (kernelParam.CURBE_Length + 31) >> 5;
}

MOS_STATUS RenderCmdPacketNext::LoadKernel()
{
    int32_t                iKrnAllocation = 0;
    MHW_KERNEL_PARAM       MhwKernelParam = {};
    RENDERHAL_KERNEL_PARAM KernelParam    = m_renderData.KernelParam;
    // Load kernel to GSH
    INIT_MHW_KERNEL_PARAM(MhwKernelParam, &m_renderData.KernelEntry);
    UpdateKernelConfigParam(KernelParam);
    iKrnAllocation = m_renderHal->pfnLoadKernel(
        m_renderHal,
        &KernelParam,
        &MhwKernelParam,
        nullptr);

    if (iKrnAllocation < 0)
    {
        RENDER_PACKET_ASSERTMESSAGE("kernel load failed");
        return MOS_STATUS_UNKNOWN;
    }

    if (m_renderData.iCurbeOffset < 0)
    {
        RENDER_PACKET_ASSERTMESSAGE("Curbe Set Fail, return error");
        return MOS_STATUS_UNKNOWN;
    }
    // Allocate Media ID, link to kernel
    m_renderData.mediaID = m_renderHal->pfnAllocateMediaID(
        m_renderHal,
        iKrnAllocation,
        m_renderData.bindingTable,
        m_renderData.iCurbeOffset,
        (m_renderData.iCurbeLength),
        0,
        nullptr);

    if (m_renderData.mediaID < 0)
    {
        RENDER_PACKET_ASSERTMESSAGE("Allocate Media ID failed, return error");
        return MOS_STATUS_UNKNOWN;
    }

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::InitRenderHalSurface(MOS_SURFACE surface, PRENDERHAL_SURFACE pRenderSurface)
{
    RENDER_PACKET_CHK_NULL_RETURN(pRenderSurface);

    RENDERHAL_GET_SURFACE_INFO info;
    MOS_ZeroMemory(&info, sizeof(info));
    RENDER_PACKET_CHK_STATUS_RETURN(RenderHal_GetSurfaceInfo(
        m_renderHal->pOsInterface,
        &info,
        &surface));

    if (Mos_ResourceIsNull(&pRenderSurface->OsSurface.OsResource))
    {
        pRenderSurface->OsSurface = surface;
    }

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::InitRenderHalBuffer(MOS_BUFFER surface, PRENDERHAL_SURFACE pRenderSurface)
{
    RENDER_PACKET_CHK_NULL_RETURN(pRenderSurface);
    pRenderSurface->OsSurface.OsResource = surface.OsResource;
    pRenderSurface->OsSurface.dwWidth    = surface.size;
    pRenderSurface->OsSurface.dwHeight   = 1;
    pRenderSurface->OsSurface.dwPitch    = surface.size;
    pRenderSurface->OsSurface.Format     = Format_RAW;

    return MOS_STATUS_SUCCESS;
}

#if 0
MOS_STATUS RenderCmdPacketNext::AddPipeControl(MOS_COMMAND_BUFFER* commandBuffer, PMHW_PIPE_CONTROL_PARAMS pipeControlParams)
{
    RENDER_PACKET_CHK_NULL_RETURN(pipeControlParams);
    RENDER_PACKET_CHK_NULL_RETURN(m_miItf);

    auto& params = m_miItf->MHW_GETPAR_F(PIPE_CONTROL)();
    params                               = {};
    params.dwFlushMode                   = pipeControlParams->dwFlushMode;
    params.bGenericMediaStateClear       = pipeControlParams->bGenericMediaStateClear;
    params.bIndirectStatePointersDisable = pipeControlParams->bIndirectStatePointersDisable;
    params.bDisableCSStall               = pipeControlParams->bDisableCSStall;
    m_miItf->MHW_ADDCMD_F(PIPE_CONTROL)(commandBuffer);

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::MediaStateFlush(MOS_COMMAND_BUFFER* commandBuffer, MHW_MEDIA_STATE_FLUSH_PARAM *flushParam)
{
    RENDER_PACKET_CHK_NULL_RETURN(m_miItf);
    RENDER_PACKET_CHK_NULL_RETURN(flushParam);

    auto& params = m_miItf->MHW_GETPAR_F(MEDIA_STATE_FLUSH)();
    params                              = {};
    params.ui8InterfaceDescriptorOffset = flushParam->ui8InterfaceDescriptorOffset;
    params.bFlushToGo                   = flushParam->bFlushToGo;
    m_miItf->MHW_ADDCMD_F(MEDIA_STATE_FLUSH)(commandBuffer);

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS RenderCmdPacketNext::AddMiBatchBufferEnd(MOS_COMMAND_BUFFER *commandBuffer, PMHW_BATCH_BUFFER batchBuffer)
{
    RENDER_PACKET_CHK_NULL_RETURN(m_miItf);

    m_miItf->AddMiBatchBufferEnd(commandBuffer, nullptr);

    return MOS_STATUS_SUCCESS;
}
#endif
