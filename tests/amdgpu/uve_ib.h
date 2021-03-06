/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef _uve_ib_h_
#define _uve_ib_h_

static const uint32_t uve_session_info[] = {
	0x00000018,
	0x00000001,
	0x00000001,
	0x00000000,
};

static const uint32_t uve_task_info[] = {
	0x00000014,
	0x00000002,
};

static const uint32_t uve_session_init[] = {
	0x00000068,
	0x00000003,
	0x00000000,
	0x000000c0,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000001,
	0x00000002,
	0x000000c0,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_layer_ctrl[] = {
	0x00000010,
	0x00000004,
	0x00000001,
	0x00000001,
};

static const uint32_t uve_layer_select[] = {
	0x0000000c,
	0x00000005,
	0x00000000,
};

static const uint32_t uve_slice_ctrl[] = {
	0x00000014,
	0x00000006,
	0x00000000,
	0x00000008,
	0x00000008,
};

static const uint32_t uve_spec_misc[] = {
	0x00000024,
	0x00000007,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000004,
	0x00000000,
	0x00000005,
};

static const uint32_t uve_rc_session_init[] = {
	0x00000010,
	0x00000008,
	0x00000000,
	0x00000040,
};

static const uint32_t uve_rc_layer_init[] = {
	0x00000028,
	0x00000009,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_hw_spec[] = {
	0x0000007c,
	0x0000000b,
	0x00000002,
	0x00000000,
	0x00000001,
	0x00000001,
	0x00000001,
	0x00000001,
	0x00000001,
	0x00000010,
	0x00000010,
	0x00000010,
	0x00000010,
	0x00000008,
	0x00000008,
	0x00000010,
	0x00000010,
	0x00000000,
	0x00000002,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000001,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000001,
	0x00000001,
};

static const uint32_t uve_deblocking_filter[] = {
	0x00000020,
	0x0000000f,
	0x00000001,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_feedback_buffer[] = {
	0x00000014,
	0x00000014,
};

/* TODO - Slice Header*/
static const uint32_t uve_slice_header[] = {
	0x000000c8,
	0x0000000c,
	0x26010000,
	0x40000000,
	0x60000000,
	0x80000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000002,
	0x00000010,
	0x00000003,
	0x00000000,
	0x00000002,
	0x00000002,
	0x00000004,
	0x00000000,
	0x00000001,
	0x00000000,
	0x00000002,
	0x00000003,
	0x00000005,
	0x00000000,
	0x00000002,
	0x00000001,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_encode_param[] = {
	0x00000000,
	0x00000000,
	0x000000a0,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000002,
	0xffffffff,
	0xffffffff,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_quality_param[] = {
	0x00000014,
	0x0000000e,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_intra_refresh[] = {
	0x00000014,
	0x00000010,
	0x00000000,
	0x00000000,
	0x00000004,
};

static const uint32_t uve_reconstructed_pic_output[] = {
	0x00000020,
	0x00000011,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_ctx_buffer[] = {
	0x00000010,
	0x00000012,
};

static const uint32_t uve_bitstream_buffer[] = {
	0x00000014,
	0x00000013,
};

static const uint32_t uve_rc_per_pic[] = {
	0x00000024,
	0x0000000a,
	0x0000001a,
	0x00000014,
	0x0000002D,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

static const uint32_t uve_op_init[] = {
	0x00000008,
	0x08000001,
};

static const uint32_t uve_op_close[] = {
	0x00000008,
	0x08000002,
};

static const uint32_t uve_op_encode[] = {
	0x00000008,
	0x08000003,
};

static const uint32_t uve_op_init_rc[] = {
	0x00000008,
	0x08000004,
};
#endif /*_uve_ib_h*/
