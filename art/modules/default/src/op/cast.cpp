/*
 * Copyright 2022 SenseTime Group Limited
 *
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

#include <stdlib.h>

#include "art/data_type.h"
#include "art/log.h"
#include "art/module.h"
#include "art/op.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    op_t o;
    uint32_t dtype;
} op_cast_t;
op_cast_t *op_default_cast_tp_alloc(workspace_t *ws);
void op_default_cast_tp_config(op_t *op);
void op_default_cast_tp_prepare(op_t *op);
void op_default_cast_tp_destroy(op_t *op);
void op_default_cast_tp_dealloc(op_t *op);

#ifdef __cplusplus
}
#endif

op_cast_t *op_default_cast_tp_alloc(workspace_t *ws)
{
    (void)ws;
    op_cast_t *res = (op_cast_t *)malloc(sizeof(op_cast_t));
    memset(res, 0, sizeof(op_cast_t));
    return res;
}

void op_default_cast_tp_config(op_t *op)
{
    CHECK(op_setting_single_get(op, SETTING_CAST_DTYPE, dtUINT32, &((op_cast_t *)op)->dtype));
}

void op_default_cast_tp_destroy(op_t *op) { (void)op; }

void op_default_cast_tp_dealloc(op_t *op)
{
    if (NULL != op)
        free(op);
}

template <typename T, typename D> void op_default_cast(D *c, const T *a, size_t size)
{
    for (int i = 0; i < size; i++) {
        c[i] = (D)a[i];
    }
}
template <typename T, typename D> static void op_default_cast_run(op_t *op)
{
    size_t count = shape_count(&op->output_tensors[0].shape);
    const T *input_0 = (const T *)mem_data(op->input_tensors[0]->mem);
    D *output_0 = (D *)mem_data(op->output_tensors[0].mem);
    op_default_cast(output_0, input_0, count);
}

void op_default_cast_tp_prepare(op_t *op)
{
    op_cast_t *cast_op = (op_cast_t *)op;
    tensor_alloc(op->input_tensors[0]);
    // chaneg output dtype
    uint32_t o_dtype = op->output_tensors[0].dtype = cast_op->dtype;
    uint32_t i_dtype = op->input_tensors[0]->dtype;
    tensor_alloc(&op->output_tensors[0]);

    if (i_dtype == dtFLOAT32 && o_dtype == dtUINT8) {
        op->run_func = op_default_cast_run<float, uint8_t>;
    } else if (i_dtype == dtINT64 && o_dtype == dtFLOAT32) {
        op->run_func = op_default_cast_run<int64_t, float>;
    } else if (i_dtype == dtFLOAT32 && o_dtype == dtINT64) {
        op->run_func = op_default_cast_run<float, int64_t>;
    } else if (i_dtype == dtINT32 && o_dtype == dtUINT8) {
        op->run_func = op_default_cast_run<int32_t, uint8_t>;
    } else if (i_dtype == dtFLOAT32 && o_dtype == dtFLOAT32) {
        op->run_func = op_default_cast_run<float, float>;
    } else if (i_dtype == dtINT64 && o_dtype == dtINT64) {
        op->run_func = op_default_cast_run<int64_t, int64_t>;
    } else if (i_dtype == dtINT32 && o_dtype == dtINT8) {
        op->run_func = op_default_cast_run<int32_t, int8_t>;
    } else {
        CHECK(false);
    }
}
