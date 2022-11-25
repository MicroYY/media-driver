/*
* Copyright (c) 2009-2021, Intel Corporation
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
//! \file      mos_os_hw.h
//! \brief 
//!
//!
//! \file     mos_os_hw.h
//! \brief    Common interface and structure used in MOS OS HW
//! \details  Common interface and structure used in MOS OS HW
//!

#ifndef __MOS_OS_HW_H__
#define __MOS_OS_HW_H__

//!
//! \brief OS HW resource definitions
//!
typedef enum _MOS_HW_RESOURCE_DEF
{
    // CODEC USAGES
    MOS_CODEC_RESOURCE_USAGE_BEGIN_CODEC,
    MOS_CODEC_RESOURCE_USAGE_PRE_DEBLOCKING_CODEC,
    MOS_CODEC_RESOURCE_USAGE_PRE_DEBLOCKING_CODEC_PARTIALENCSURFACE,              //!< Pre-Deblocking codec surface for partial secure surfaces,
    MOS_CODEC_RESOURCE_USAGE_POST_DEBLOCKING_CODEC,
    MOS_CODEC_RESOURCE_USAGE_ORIGINAL_UNCOMPRESSED_PICTURE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_ORIGINAL_UNCOMPRESSED_PICTURE_DECODE,
    MOS_CODEC_RESOURCE_USAGE_STREAMOUT_DATA_CODEC,
    MOS_CODEC_RESOURCE_USAGE_INTRA_ROWSTORE_SCRATCH_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_SCRATCH_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_REFERENCE_PICTURE_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MACROBLOCK_STATUS_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MFX_INDIRECT_BITSTREAM_OBJECT_DECODE,
    MOS_CODEC_RESOURCE_USAGE_MFX_INDIRECT_MV_OBJECT_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MFD_INDIRECT_IT_COEF_OBJECT_DECODE,
    MOS_CODEC_RESOURCE_USAGE_MFC_INDIRECT_PAKBASE_OBJECT_CODEC,
    MOS_CODEC_RESOURCE_USAGE_BSDMPC_ROWSTORE_SCRATCH_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MPR_ROWSTORE_SCRATCH_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_BITPLANE_READ_CODEC,
    MOS_CODEC_RESOURCE_USAGE_DIRECTMV_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_CURR_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_REF_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MV_DATA_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_HME_DOWNSAMPLED_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_HME_DOWNSAMPLED_ENCODE_FF,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_HME_DOWNSAMPLED_ENCODE_DST,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ME_DISTORTION_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_BRC_ME_DISTORTION_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_PAK_OBJECT_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_FLATNESS_CHECK_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MBENC_CURBE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MAD_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_VP8_BLOCK_MODE_COST_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_VP8_MB_MODE_COST_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_VP8_MBENC_OUTPUT_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_VP8_HISTOGRAM_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_VP8_L3_LLC_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_MFX_STANDALONE_DEBLOCKING_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_MD_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_SAO_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_MV_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_STATUS_ERROR_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_LCU_ILDB_STREAMOUT_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VP9_PROBABILITY_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VP9_SEGMENT_ID_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VP9_HVD_ROWSTORE_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MBDISABLE_SKIPMAP_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VDENC_ROW_STORE_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VDENC_STREAMIN_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MB_QP_CODEC,
    MOS_CODEC_RESOURCE_USAGE_MACROBLOCK_ILDB_STREAM_OUT_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SSE_SRC_PIXEL_ROW_STORE_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SLICE_STATE_STREAM_OUT_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_CABAC_SYNTAX_STREAM_OUT_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_PRED_COL_STORE_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_PAK_IMAGESTATE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MBENC_BRC_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MB_BRC_CONST_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_BRC_MB_QP_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_BRC_ROI_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_SLICE_MAP_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_WP_DOWNSAMPLED_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_VDENC_IMAGESTATE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_UNCACHED,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ELLC_ONLY,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ELLC_LLC_ONLY,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ELLC_LLC_L3,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_BRC_HISTORY_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_SOFTWARE_SCOREBOARD_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ME_MV_DATA_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_MV_DISTORTION_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_4XME_DISTORTION_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_INTRA_DISTORTION_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_MB_STATS_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_PAK_STATS_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_PIC_STATE_READ_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_PIC_STATE_WRITE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_COMBINED_ENC_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_BRC_CONSTANT_DATA_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_INTERMEDIATE_CU_RECORD_SURFACE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_SCRATCH_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_LCU_LEVEL_DATA_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_HISTORY_INPUT_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_HISTORY_OUTPUT_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_DEBUG_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_CONSTANT_TABLE_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_CU_RECORD_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_MV_TEMPORAL_BUFFER_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_CU_PACKET_FOR_PAK_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_BCOMBINED1_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_SURFACE_ENC_BCOMBINED2_ENCODE,
    MOS_CODEC_RESOURCE_USAGE_FRAME_STATS_STREAMOUT_DATA_CODEC,
    MOS_CODEC_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_TILE_LINE_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_TILE_COLUMN_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_MD_TILE_LINE_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_MD_TILE_COLUMN_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_SAO_TILE_LINE_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HCP_SAO_TILE_COLUMN_CODEC,
    MOS_CODEC_RESOURCE_USAGE_VP9_PROBABILITY_COUNTER_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_HUC_VIRTUAL_ADDR_REGION_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_SIZE_STREAMOUT_CODEC,
    MOS_CODEC_RESOURCE_USAGE_COMPRESSED_HEADER_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_PROBABILITY_DELTA_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_TILE_RECORD_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_TILE_SIZE_STAS_BUFFER_CODEC,
    MOS_CODEC_RESOURCE_USAGE_END_CODEC,

    // CM USAGES
    MOS_CM_RESOURCE_USAGE_SurfaceState,
    MOS_CM_RESOURCE_USAGE_StateHeap,
    MOS_CM_RESOURCE_USAGE_NO_L3_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_LLC_ELLC_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_LLC_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_ELLC_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_LLC_L3_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_ELLC_L3_SurfaceState,
    MOS_CM_RESOURCE_USAGE_NO_CACHE_SurfaceState,
    MOS_CM_RESOURCE_USAGE_L1_Enabled_SurfaceState,

    // MP USAGES
    MOS_MP_RESOURCE_USAGE_BEGIN,
    MOS_MP_RESOURCE_USAGE_DEFAULT,
    MOS_MP_RESOURCE_USAGE_DEFAULT_FF,
    MOS_MP_RESOURCE_USAGE_DEFAULT_RCS,
    MOS_MP_RESOURCE_USAGE_SurfaceState,
    MOS_MP_RESOURCE_USAGE_SurfaceState_FF,
    MOS_MP_RESOURCE_USAGE_SurfaceState_RCS,
    MOS_MP_RESOURCE_USAGE_AGE3_SurfaceState,
    MOS_MP_RESOURCE_USAGE_EDRAM_SurfaceState,
    MOS_MP_RESOURCE_USAGE_EDRAM_AGE3_SurfaceState,
    MOS_MP_RESOURCE_USAGE_No_L3_SurfaceState,
    MOS_MP_RESOURCE_USAGE_No_LLC_L3_SurfaceState,
    MOS_MP_RESOURCE_USAGE_No_LLC_L3_AGE_SurfaceState,
    MOS_MP_RESOURCE_USAGE_No_LLC_eLLC_L3_AGE_SurfaceState,
    MOS_MP_RESOURCE_USAGE_PartialEnc_No_LLC_L3_AGE_SurfaceState,
    MOS_MP_RESOURCE_USAGE_END,

    // MHW - SFC
    MOS_MHW_RESOURCE_USAGE_Sfc_CurrentOutputSurface,                                //!< SFC Output Surface
    MOS_MHW_RESOURCE_USAGE_Sfc_CurrentOutputSurface_PartialEncSurface,              //!< SFC Output Surface for partial secure surfaces
    MOS_MHW_RESOURCE_USAGE_Sfc_AvsLineBufferSurface,                                //!< SFC AVS Line buffer Surface
    MOS_MHW_RESOURCE_USAGE_Sfc_IefLineBufferSurface,                                //!< SFC IEF Line buffer Surface

    // Camera  caputure.
    MOS_MHW_GMM_RESOURCE_USAGE_CAMERA_CAPTURE,

    // Media GMM Resource USAGES
    MOS_RESOURCE_USAGE_CCS_MEDIA_WRITABLE,

    // Media BLT copy USAGES
    MOS_GMM_RESOURCE_USAGE_BLT_SOURCE,
    MOS_GMM_RESOURCE_USAGE_BLT_DESTINATION,

    // PAT Media Usages
    MOS_HW_RESOURCE_USAGE_MEDIA_BATCH_BUFFERS,
    // DECODE
    MOS_HW_RESOURCE_USAGE_DECODE_INPUT_BITSTREAM,
    MOS_HW_RESOURCE_USAGE_DECODE_INPUT_REFERENCE,
    MOS_HW_RESOURCE_USAGE_DECODE_INTERNAL_READ,
    MOS_HW_RESOURCE_USAGE_DECODE_INTERNAL_WRITE,
    MOS_HW_RESOURCE_USAGE_DECODE_INTERNAL_READ_WRITE_CACHE,
    MOS_HW_RESOURCE_USAGE_DECODE_INTERNAL_READ_WRITE_NOCACHE,
    MOS_HW_RESOURCE_USAGE_DECODE_OUTPUT_PICTURE,
    MOS_HW_RESOURCE_USAGE_DECODE_OUTPUT_STATISTICS_WRITE,
    MOS_HW_RESOURCE_USAGE_DECODE_OUTPUT_STATISTICS_READ_WRITE,
    // ENCODE
    MOS_HW_RESOURCE_USAGE_ENCODE_INPUT_RAW,
    MOS_HW_RESOURCE_USAGE_ENCODE_INPUT_RECON,
    MOS_HW_RESOURCE_USAGE_ENCODE_INTERNAL_READ,
    MOS_HW_RESOURCE_USAGE_ENCODE_INTERNAL_WRITE,
    MOS_HW_RESOURCE_USAGE_ENCODE_INTERNAL_READ_WRITE_CACHE,
    MOS_HW_RESOURCE_USAGE_ENCODE_INTERNAL_READ_WRITE_NOCACHE,
    MOS_HW_RESOURCE_USAGE_ENCODE_EXTERNAL_READ,
    MOS_HW_RESOURCE_USAGE_ENCODE_OUTPUT_PICTURE,
    MOS_HW_RESOURCE_USAGE_ENCODE_OUTPUT_BITSTREAM,
    MOS_HW_RESOURCE_USAGE_ENCODE_OUTPUT_STATISTICS_WRITE,
    MOS_HW_RESOURCE_USAGE_ENCODE_OUTPUT_STATISTICS_READ_WRITE,
    // VP
    MOS_HW_RESOURCE_USAGE_VP_INPUT_PICTURE_FF,
    MOS_HW_RESOURCE_USAGE_VP_INPUT_REFERENCE_FF,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_READ_FF,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_WRITE_FF,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_READ_WRITE_FF,
    MOS_HW_RESOURCE_USAGE_VP_OUTPUT_PICTURE_FF,
    MOS_HW_RESOURCE_USAGE_VP_INPUT_PICTURE_RENDER,
    MOS_HW_RESOURCE_USAGE_VP_INPUT_REFERENCE_RENDER,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_READ_RENDER,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_WRITE_RENDER,
    MOS_HW_RESOURCE_USAGE_VP_INTERNAL_READ_WRITE_RENDER,
    MOS_HW_RESOURCE_USAGE_VP_OUTPUT_PICTURE_RENDER,
    // CP
    MOS_HW_RESOURCE_USAGE_CP_EXTERNAL_READ,
    MOS_HW_RESOURCE_USAGE_CP_INTERNAL_WRITE,

    // PAT Media Usages END

    // always last one
    MOS_HW_RESOURCE_DEF_MAX
} MOS_HW_RESOURCE_DEF;
C_ASSERT(MOS_HW_RESOURCE_DEF_MAX == 174); //!< update this and Mos_CachePolicyGetMemoryObject_XXX()

//!
//! \brief Enum allow callers to specify a command streamer.
//!
typedef enum MOS_HW_COMMAND_STREAMER_ENUM
{
    MOS_HW_COMMAND_STREAMER_NULL = 0,   //!< This stays zero.
    MOS_CS,                             //!< Render Command Streamer
    MOS_BCS,                            //!< Blitter (BLT) Command Streamer
    MOS_VCS,                            //!< Video Codec (MFX) Command Streamer
    MOS_VECS,                           //!< Video Enhancement (VEBOX) Command Streamer
    MOS_HW_COMMAND_STREAMERS            //!< This stays last.
} MOS_HW_COMMAND_STREAMER;
C_ASSERT(MOS_HW_COMMAND_STREAMERS == 5);//!< update this and Mos_GetUseGlobalGtt_XXX()

//!
//! \brief Enum to allow callers to specify a HW command.
//!        Currently only used with GmmGetUseGlobalGtt--with the 
//!        commands that have a PPGTT/GTT space select field.
//!
typedef enum MOS_HW_COMMAND_ENUM
{
    MOS_HW_COMMAND_NULL = 0, //!<-- This stays zero.
    MOS_MI_BATCH_BUFFER_START,
    MOS_MI_CLFLUSH,
    MOS_MI_CONDITIONAL_BATCH_BUFFER_END,
    MOS_MI_COPY_MEM_MEM,
    MOS_MI_FLUSH_DW,
    MOS_MI_LOAD_REGISTER_MEM,
    MOS_MI_REPORT_PERF_COUNT,
    MOS_MI_SEMAPHORE_MBOX,
    MOS_MI_SEMAPHORE_SIGNAL,
    MOS_MI_SEMAPHORE_WAIT,
    MOS_MI_STORE_DATA_IMM,
    MOS_MI_STORE_DATA_INDEX,
    MOS_MI_STORE_REGISTER_MEM,
    MOS_MI_UPDATE_GTT,
    MOS_PIPE_CONTROL,
    MOS_HW_COMMANDS, //!<-- This stays last.

    // Those commands below are not included in GMM
    MOS_MI_BATCH_BUFFER_START_RCS,
    MOS_MFX_PIPE_BUF_ADDR,
    MOS_MFX_INDIRECT_OBJ_BASE_ADDR,
    MOS_MFX_BSP_BUF_BASE_ADDR,
    MOS_MFX_AVC_DIRECT_MODE,
    MOS_MFX_VC1_DIRECT_MODE,
    MOS_MFX_VP8_PIC,
    MOS_MFX_DBK_OBJECT,
    MOS_HUC_DMEM,
    MOS_HUC_VIRTUAL_ADDR,
    MOS_HUC_IND_OBJ_BASE_ADDR,
    MOS_VDENC_PIPE_BUF_ADDR,
    MOS_SURFACE_STATE,
    MOS_SURFACE_STATE_ADV,
    MOS_STATE_BASE_ADDR,
    MOS_VEBOX_STATE,
    MOS_VEBOX_DI_IECP,
    MOS_VEBOX_TILING_CONVERT,
    MOS_SFC_STATE,
    MOS_MI_ATOMIC,
    MOS_MFX_CC_BASE_ADDR_STATE,
    MOS_VEBOX_SURFACE_STATE,
    MOS_OCA_RESERVED,
    MOS_HW_COMMAND_MAX
} MOS_HW_COMMAND;
C_ASSERT(MOS_HW_COMMANDS == 16); //!< update this and Mos_GetUseGlobalGtt_XXX()

#endif // __MOS_OS_HW_H__
