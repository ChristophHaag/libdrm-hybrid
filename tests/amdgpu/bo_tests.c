/*
 * Copyright 2014 Advanced Micro Devices, Inc.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "CUnit/Basic.h"

#include "amdgpu_test.h"
#include "amdgpu_drm.h"

#define BUFFER_SIZE (4*1024)
#define BUFFER_ALIGN (4*1024)

static amdgpu_device_handle device_handle;
static uint32_t major_version;
static uint32_t minor_version;

static amdgpu_bo_handle buffer_handle;
static uint64_t virtual_mc_base_address;
static amdgpu_va_handle va_handle;

static void amdgpu_bo_export_import(void);
static void amdgpu_bo_metadata(void);
static void amdgpu_bo_map_unmap(void);
static void amdgpu_get_fb_id_and_handle(void);
static void amdgpu_bo_direct_gma(void);
static void amdgpu_bo_ssg(void);

CU_TestInfo bo_tests[] = {
	{ "Export/Import",  amdgpu_bo_export_import },
#if 0
	{ "Metadata",  amdgpu_bo_metadata },
#endif
	{ "CPU map/unmap",  amdgpu_bo_map_unmap },
	{ "GET FB_ID AND FB_HANDLE",  amdgpu_get_fb_id_and_handle },
	{ "Direct GMA", amdgpu_bo_direct_gma },
	{ "SSG", amdgpu_bo_ssg },
	CU_TEST_INFO_NULL,
};

int suite_bo_tests_init(void)
{
	struct amdgpu_bo_alloc_request req = {0};
	amdgpu_bo_handle buf_handle;
	uint64_t va;
	int r;

	r = amdgpu_device_initialize(drm_amdgpu[0], &major_version,
				  &minor_version, &device_handle);
	if (r) {
		if ((r == -EACCES) && (errno == EACCES))
			printf("\n\nError:%s. "
				"Hint:Try to run this test program as root.",
				strerror(errno));

		return CUE_SINIT_FAILED;
	}

	req.alloc_size = BUFFER_SIZE;
	req.phys_alignment = BUFFER_ALIGN;
	req.preferred_heap = AMDGPU_GEM_DOMAIN_GTT;

	r = amdgpu_bo_alloc(device_handle, &req, &buf_handle);
	if (r)
		return CUE_SINIT_FAILED;

	r = amdgpu_va_range_alloc(device_handle,
				  amdgpu_gpu_va_range_general,
				  BUFFER_SIZE, BUFFER_ALIGN, 0,
				  &va, &va_handle, 0);
	if (r)
		goto error_va_alloc;

	r = amdgpu_bo_va_op(buf_handle, 0, BUFFER_SIZE, va, 0, AMDGPU_VA_OP_MAP);
	if (r)
		goto error_va_map;

	buffer_handle = buf_handle;
	virtual_mc_base_address = va;

	return CUE_SUCCESS;

error_va_map:
	amdgpu_va_range_free(va_handle);

error_va_alloc:
	amdgpu_bo_free(buf_handle);
	return CUE_SINIT_FAILED;
}

int suite_bo_tests_clean(void)
{
	int r;

	r = amdgpu_bo_va_op(buffer_handle, 0, BUFFER_SIZE,
			    virtual_mc_base_address, 0,
			    AMDGPU_VA_OP_UNMAP);
	if (r)
		return CUE_SCLEAN_FAILED;

	r = amdgpu_va_range_free(va_handle);
	if (r)
		return CUE_SCLEAN_FAILED;

	r = amdgpu_bo_free(buffer_handle);
	if (r)
		return CUE_SCLEAN_FAILED;

	r = amdgpu_device_deinitialize(device_handle);
	if (r)
		return CUE_SCLEAN_FAILED;

	return CUE_SUCCESS;
}

static void amdgpu_bo_export_import_do_type(enum amdgpu_bo_handle_type type)
{
	struct amdgpu_bo_import_result res = {0};
	uint32_t shared_handle;
	int r;

	r = amdgpu_bo_export(buffer_handle, type, &shared_handle);
	CU_ASSERT_EQUAL(r, 0);

	r = amdgpu_bo_import(device_handle, type, shared_handle, &res);
	CU_ASSERT_EQUAL(r, 0);

	CU_ASSERT_EQUAL(res.buf_handle, buffer_handle);
	CU_ASSERT_EQUAL(res.alloc_size, BUFFER_SIZE);

	r = amdgpu_bo_free(res.buf_handle);
	CU_ASSERT_EQUAL(r, 0);
}

static void amdgpu_bo_export_import(void)
{
	if (open_render_node) {
		printf("(DRM render node is used. Skip export/Import test) ");
		return;
	}

	amdgpu_bo_export_import_do_type(amdgpu_bo_handle_type_gem_flink_name);
	amdgpu_bo_export_import_do_type(amdgpu_bo_handle_type_dma_buf_fd);
}

static void amdgpu_bo_metadata(void)
{
	struct amdgpu_bo_metadata meta = {0};
	struct amdgpu_bo_info info = {0};
	int r;

	meta.size_metadata = 1;
	meta.umd_metadata[0] = 0xdeadbeef;

	r = amdgpu_bo_set_metadata(buffer_handle, &meta);
	CU_ASSERT_EQUAL(r, 0);

	r = amdgpu_bo_query_info(buffer_handle, &info);
	CU_ASSERT_EQUAL(r, 0);

	CU_ASSERT_EQUAL(info.metadata.size_metadata, 1);
	CU_ASSERT_EQUAL(info.metadata.umd_metadata[0], 0xdeadbeef);
}

static void amdgpu_bo_map_unmap(void)
{
	uint32_t *ptr;
	int i, r;

	r = amdgpu_bo_cpu_map(buffer_handle, (void **)&ptr);
	CU_ASSERT_EQUAL(r, 0);
	CU_ASSERT_NOT_EQUAL(ptr, NULL);

	for (i = 0; i < (BUFFER_SIZE / 4); ++i)
		ptr[i] = 0xdeadbeef;

	r = amdgpu_bo_cpu_unmap(buffer_handle);
	CU_ASSERT_EQUAL(r, 0);
}

static void amdgpu_get_fb_id_and_handle(void)
{
	uint32_t *ptr;
	int i, r;
	unsigned int fb_id;
	struct amdgpu_bo_import_result output;

	r = amdgpu_get_fb_id(device_handle, &fb_id);
	CU_ASSERT_EQUAL(r, 0);
	if (fb_id == 0) {
		fprintf(stderr, "\nSkipping amdgpu_get_fb_id_and_handle test: no monitor connected\n");
		return;
	}

	r = amdgpu_get_bo_from_fb_id(device_handle, fb_id, &output);
	CU_ASSERT_EQUAL(r, 0);
	CU_ASSERT_NOT_EQUAL(output.buf_handle, 0);
}

#define TEST_LOOP 20
static void amdgpu_bo_direct_gma(void)
{
	amdgpu_bo_handle buf_handle[TEST_LOOP] = {0};
	amdgpu_bo_handle buf_handle_import[TEST_LOOP] = {0};
	uint32_t *ptr[TEST_LOOP] = {0};
	struct amdgpu_bo_alloc_request req = {0};
	struct drm_amdgpu_capability cap;
	uint64_t size=4096, phys_addr, remain;
	int i, j, r;

	amdgpu_query_capability(device_handle, &cap);
	if(!(cap.flag & AMDGPU_CAPABILITY_DIRECT_GMA_FLAG))
		return;

	printf("direct_gma_size is %d MB\n", cap.direct_gma_size);
	remain = cap.direct_gma_size << 20;

	req.preferred_heap = AMDGPU_GEM_DOMAIN_DGMA;
	for (i = 0; i < TEST_LOOP; i++) {
		req.alloc_size = size;
		r = amdgpu_bo_alloc(device_handle, &req, &buf_handle[i]);
		CU_ASSERT_EQUAL(r, 0);

		r = amdgpu_bo_get_phys_address(buf_handle[i], &phys_addr);
		CU_ASSERT_EQUAL(r, 0);
		printf("bo_size %"PRIx64" phys_addr %"PRIx64"\n", size, phys_addr);
		r = amdgpu_create_bo_from_phys_mem(device_handle, phys_addr, size, &buf_handle_import[i]);
		CU_ASSERT_EQUAL(r, 0);
		r = amdgpu_bo_cpu_map(buf_handle_import[i], (void **)&ptr[i]);
		CU_ASSERT_EQUAL(r, 0);

		for (j = 0; j < (size / 4); ++j)
			ptr[i][j] = 0xdeadbeef;
		remain -= size;
		size <<= 1;
		printf("test loop %d finished, remain %"PRIx64", try to alloc %"PRIx64"\n", i, remain, size);
		if (remain < size)
			break;

	}

	for (i = 0; i < TEST_LOOP; i++) {
		if (ptr[i]) {
			r = amdgpu_bo_cpu_unmap(buf_handle_import[i]);
			CU_ASSERT_EQUAL(r, 0);
		}

		if (buf_handle_import[i]) {
			r = amdgpu_bo_free(buf_handle_import[i]);
			CU_ASSERT_EQUAL(r, 0);
		}

		if (buf_handle[i]) {
			r = amdgpu_bo_free(buf_handle[i]);
			CU_ASSERT_EQUAL(r, 0);
		}
	}
}

static void amdgpu_bo_ssg(void)
{
	struct drm_amdgpu_capability cap;
	amdgpu_bo_handle buf_handle;
	size_t buf_size = 0x2000000;
	struct amdgpu_bo_alloc_request req = {0};
	char *in_file = "/tmp/ssg-in", *out_file = "/tmp/ssg-out";
	int i, j, fd;
	uint64_t pattern = 0xdeadbeef12345678, out;
	void *buf;
	bool write_is_ok;

	CU_ASSERT(!amdgpu_query_capability(device_handle, &cap));
	if(!(cap.flag & AMDGPU_CAPABILITY_SSG_FLAG)) {
		printf("ignore SSG test due to kernel SSG is disabled\n");
		return;
	}

	if (buf_size > cap.direct_gma_size << 20)
		buf_size = cap.direct_gma_size << 20;

	printf("SSG read/write block size 0x%x\n", buf_size);

	CU_ASSERT((fd = open(in_file, O_WRONLY | O_CREAT, S_IRWXU)) >= 0);
	for (i = 0; i < buf_size; i += sizeof(pattern)) {
		write_is_ok = write(fd, &pattern, sizeof(pattern)) == sizeof(pattern);
		if (!write_is_ok)
			break;
	}
	CU_ASSERT(write_is_ok);
	fsync(fd);
	close(fd);

	CU_ASSERT((fd = open(in_file, O_RDONLY | O_DIRECT)) >= 0);

	req.alloc_size = buf_size;
	req.preferred_heap = AMDGPU_GEM_DOMAIN_DGMA;
	CU_ASSERT(!amdgpu_bo_alloc(device_handle, &req, &buf_handle));

	CU_ASSERT(!amdgpu_bo_cpu_map(buf_handle, &buf));
	for (i = 0; i < 3; i++)  {
		struct timespec ts1, ts2;
		double a, b, c;
		bool read_is_same;

		memset(buf, 0, buf_size);

		CU_ASSERT(!clock_gettime(CLOCK_MONOTONIC, &ts1));
		CU_ASSERT(read(fd, buf, buf_size) == buf_size);
		CU_ASSERT(!clock_gettime(CLOCK_MONOTONIC, &ts2));

		a = ts2.tv_sec - ts1.tv_sec;
		b = ts2.tv_nsec - ts1.tv_nsec;
		c = (buf_size >> 20) / (a + b / 1000000000.0);
		printf("\tSSG read speed = %f MB/s\n", c);

		for (j = 0; j < buf_size; j += sizeof(pattern)) {
			read_is_same = *(uint64_t *)(buf + j) == pattern;
			if (!read_is_same)
				break;
		}
		CU_ASSERT(read_is_same);

		lseek(fd, 0, SEEK_SET);
	}
	close(fd);
	remove(in_file);

	for (i = 0; i < 3; i++) {
		struct timespec ts1, ts2;
		double a, b, c;
		bool write_is_same;

		CU_ASSERT((fd = open(out_file, O_WRONLY | O_CREAT | O_DIRECT, S_IRWXU)) >= 0);

		CU_ASSERT(!clock_gettime(CLOCK_MONOTONIC, &ts1));
		CU_ASSERT(write(fd, buf, buf_size) == buf_size);
		CU_ASSERT(!clock_gettime(CLOCK_MONOTONIC, &ts2));

		a = ts2.tv_sec - ts1.tv_sec;
		b = ts2.tv_nsec - ts1.tv_nsec;
		c = (buf_size >> 20) / (a + b / 1000000000.0);
		printf("\tSSG write speed = %f MB/s\n", c);

		fsync(fd);
		close(fd);
		CU_ASSERT((fd = open(out_file, O_RDONLY)) >= 0);
		for (j = 0; j < buf_size; j += sizeof(pattern)) {
			write_is_same =
				read(fd, &out, sizeof(out)) == sizeof(out) &&
				out == pattern;
			if (!write_is_same)
				break;
		}
		CU_ASSERT(write_is_same);
		close(fd);
		remove(out_file);
	}

	amdgpu_bo_free(buf_handle);
}
