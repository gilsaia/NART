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
#include "art/op_tp.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    op_t o;
    uint32_t dtype;
} op_add_div_clip_cast_t;
op_add_div_clip_cast_t *op_default_add_div_clip_cast_tp_alloc(workspace_t *ws);
void op_default_add_div_clip_cast_tp_config(op_t *op);
void op_default_add_div_clip_cast_tp_prepare(op_t *op);
void op_default_add_div_clip_cast_tp_destroy(op_t *op);
void op_default_add_div_clip_cast_tp_dealloc(op_t *op);

#ifdef __cplusplus
}
#endif

op_add_div_clip_cast_t *op_default_add_div_clip_cast_tp_alloc(workspace_t *ws)
{
    (void)ws;
    op_add_div_clip_cast_t *res = (op_add_div_clip_cast_t *)malloc(sizeof(op_add_div_clip_cast_t));
    memset(res, 0, sizeof(op_add_div_clip_cast_t));
    return res;
}

void op_default_add_div_clip_cast_tp_config(op_t *op)
{
    CHECK(op_setting_single_get(
        op, SETTING_CAST_DTYPE, dtUINT32, &((op_add_div_clip_cast_t *)op)->dtype));
}

void op_default_add_div_clip_cast_tp_destroy(op_t *op) { (void)op; }

void op_default_add_div_clip_cast_tp_dealloc(op_t *op)
{
    if (NULL != op)
        free(op);
}

template <typename T, typename D> static void op_default_add_div_clip_cast_run(op_t *op)
{
    size_t count = shape_count(&op->output_tensors[0].shape);
    int i, j;
    T *input_0 = (T *)mem_data(op->input_tensors[0]->mem);
    const T *input_1 = (const T *)mem_cpu_data(op->input_tensors[1]->mem);
    const T *input_2 = (const T *)mem_cpu_data(op->input_tensors[2]->mem);
    D *output_0 = (D *)mem_data(op->output_tensors[0].mem);
    if (op->input_size == 5) {
        const T *min = (const T *)mem_data(op->input_tensors[3]->mem);
        const T *max = (const T *)mem_data(op->input_tensors[4]->mem);
        if (tensor_shape_eq(&op->input_tensors[0]->shape, &op->input_tensors[1]->shape)) {
            for (i = 0; i < (int)count; ++i) {
                input_0[i] = (input_0[i] + input_1[i]) / input_2[i];
                input_0[i] = input_0[i] < (*min) ? (*min)
                    : (*max) < input_0[i]        ? (*max)
                                                 : input_0[i];
                output_0[i] = (D)input_0[i];
            }
        } else {
            shape_t shape = op->output_tensors[0].shape;
            shape_t shape1 = op->input_tensors[0]->shape;
            shape_t shape2 = op->input_tensors[1]->shape;
            uint32_t *shape_dim = shape.dim;
            uint32_t *shape_dim1 = shape1.dim;
            uint32_t *shape_dim2 = shape2.dim;
            for (i = 0; i < shape1.dim_size; ++i) {
                shape1.dim[shape.dim_size - 1 - i] = shape1.dim[shape1.dim_size - 1 - i];
            }
            for (; i < shape.dim_size; ++i) {
                shape1.dim[shape.dim_size - 1 - i] = 1;
            }
            for (i = 0; i < shape2.dim_size; ++i) {
                shape2.dim[shape.dim_size - 1 - i] = shape2.dim[shape2.dim_size - 1 - i];
            }
            for (; i < shape.dim_size; ++i) {
                shape2.dim[shape.dim_size - 1 - i] = 1;
            }
            if (shape_part_count(&op->input_tensors[1]->shape, 0) == 1) {
                T t = input_1[0];
                T u = input_2[0];
                for (i = 0; i < (int)count - 4; i = i + 4) {
                    input_0[0] = (input_0[0] + t) / u;
                    input_0[0 + 1] = (input_0[0 + 1] + t) / u;
                    input_0[0 + 2] = (input_0[0 + 2] + t) / u;
                    input_0[0 + 3] = (input_0[0 + 3] + t) / u;
                    input_0[0] = input_0[0] < (*min) ? (*min)
                        : (*max) < input_0[0]        ? (*max)
                                                     : input_0[0];
                    output_0[0] = (D)input_0[0];
                    input_0[1] = input_0[1] < (*min) ? (*min)
                        : (*max) < input_0[1]        ? (*max)
                                                     : input_0[1];
                    output_0[1] = (D)input_0[1];
                    input_0[2] = input_0[2] < (*min) ? (*min)
                        : (*max) < input_0[2]        ? (*max)
                                                     : input_0[2];
                    output_0[2] = (D)input_0[2];
                    input_0[3] = input_0[3] < (*min) ? (*min)
                        : (*max) < input_0[3]        ? (*max)
                                                     : input_0[3];
                    output_0[3] = (D)input_0[3];
                    input_0 += 4;
                    output_0 += 4;
                }
                for (; i < (int)count; ++i) {
                    *input_0 = (*input_0 + t) / u;
                    *input_0 = *input_0 < (*min) ? (*min) : (*max) < *input_0 ? (*max) : *input_0;
                    *output_0 = (D)(*input_0);
                    input_0++;
                    output_0++;
                }

            } else if (
                count == shape_count(&op->input_tensors[0]->shape) && shape2.dim_size == 4
                && shape_dim2[1] == 1 && shape_dim2[2] == 1) {
                size_t count_except_axis0 = shape_part_count(&shape, 1);
                size_t count_except_axis0_2 = shape_part_count(&shape2, 1);
                size_t last_count = shape.dim[shape.dim_size - 1];
                size_t count_middle0
                    = count_except_axis0 / shape.dim[shape.dim_size - 1] * last_count;
                size_t dim0_axis0 = 0;
                for (uint32_t dim0 = 0; dim0 < shape.dim[0]; ++dim0) {
                    const T *input_1_dim0 = input_1 + (dim0 % shape2.dim[0]) * count_except_axis0_2;
                    const T *input_2_dim0 = input_2 + (dim0 % shape2.dim[0]) * count_except_axis0_2;
                    for (uint32_t dim_middle = 0; dim_middle < count_middle0;
                         dim_middle += last_count) {
                        D *output_0_ptr = output_0 + dim_middle;
                        T *input_0_ptr = (T *)input_0 + dim_middle;
                        const T *input_1_ptr = input_1_dim0;
                        const T *input_2_ptr = input_2_dim0;
                        const T *input_1_ptr_end = input_1_ptr + last_count;
                        while (input_1_ptr < input_1_ptr_end) {
                            *input_0_ptr = (*input_0_ptr + *input_1_ptr) / *input_2_ptr;
                            *input_0_ptr = *input_0_ptr < (*min) ? (*min)
                                : (*max) < *input_0_ptr          ? (*max)
                                                                 : *input_0_ptr;
                            *output_0_ptr = (D)(*input_0_ptr);
                            input_0_ptr++;
                            input_1_ptr++;
                            input_2_ptr++;
                            output_0_ptr++;
                        }
                    }
                    output_0 += count_except_axis0;
                    input_0 += count_except_axis0;
                }
            } else {
                for (i = 0; i < (int)count; ++i) {
                    uint32_t p = 1, p1 = 1, p2 = 1;
                    uint32_t id1 = 0, id2 = 0;
                    for (j = shape.dim_size - 1; j >= 0; j--) {
                        uint32_t tmp = (i / p) % (shape_dim[j]);
                        id1 += (shape_dim1[j] == 1 ? 0 : tmp) * p1;
                        id2 += (shape_dim2[j] == 1 ? 0 : tmp) * p2;
                        p1 *= shape_dim1[j];
                        p2 *= shape_dim2[j];
                        p *= shape_dim[j];
                    }
                    T tmp = (input_0[id1] + input_1[id2]) / input_2[id2];
                    tmp = tmp < (*min) ? (*min) : (*max) < tmp ? (*max) : tmp;
                    output_0[i] = (D)tmp;
                }
            }
        }
    } else {
        CHECK(false);
    }
}

void op_default_add_div_clip_cast_tp_prepare(op_t *op)
{
    op_add_div_clip_cast_t *add_div_clip_cast_op = (op_add_div_clip_cast_t *)op;
    int i;
    for (i = 0; i < op->input_size; ++i) {
        tensor_alloc(op->input_tensors[i]);
    }
    // chaneg output dtype
    uint32_t o_dtype = op->output_tensors[0].dtype = add_div_clip_cast_op->dtype;
    uint32_t i_dtype = op->input_tensors[0]->dtype;
    tensor_alloc(&op->output_tensors[0]);

    if (i_dtype == dtFLOAT32 && o_dtype == dtUINT8) {
        op->run_func = op_default_add_div_clip_cast_run<float, uint8_t>;
    } else if (i_dtype == dtINT32 && o_dtype == dtUINT8) {
        op->run_func = op_default_add_div_clip_cast_run<int32_t, uint8_t>;
    } else if (i_dtype == dtINT32 && o_dtype == dtINT8) {
        op->run_func = op_default_add_div_clip_cast_run<int32_t, int8_t>;
    } else {
        CHECK(false);
    }
}
