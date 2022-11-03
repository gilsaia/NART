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
#include <string.h>

#include "art/log.h"
#include "art/module.h"
#include "art/op.h"
#include "art/op_settings.h"
#include "art/op_tp.h"

typedef struct {
    op_t o;
} op_hswish_t;

op_hswish_t *op_default_hswish_tp_alloc(workspace_t *ws)
{
    (void)ws;
    op_hswish_t *res = (op_hswish_t *)malloc(sizeof(op_hswish_t));
    memset(res, 0, sizeof(op_hswish_t));
    return res;
}

void op_default_hswish_tp_config(op_t *op) { (void)op; }

void op_default_hswish_tp_destroy(op_t *op) { (void)op; }

void op_default_hswish_tp_dealloc(op_t *op)
{
    if (NULL != op)
        free(op);
}

static void op_default_hswish_run(op_t *op)
{
    const op_hswish_t *hswish_op = (op_hswish_t *)op;
    int i;
    shape_t const *shape = &(op->input_tensors[0]->shape);
    int n = 1;
    for (i = 0; i < shape->dim_size; ++i) {
        n *= shape->dim[i];
    }
    const float *input_0 = mem_cpu_data(op->input_tensors[0]->mem);
    float *output_0 = mem_cpu_data(op->output_tensors[0].mem);
    // scale shift mean variance
    for (i = 0; i < n; ++i) {
        float x = (input_0[i] + 3 > 0) ? input_0[i] + 3 : 0;
        x = x < 6 ? x : 6;
        output_0[i] = input_0[i] * x / 6;
    }
}

void op_default_hswish_tp_prepare(op_t *op)
{
    int i;
    for (i = 0; i < op->input_size; ++i) {
        tensor_alloc(op->input_tensors[i]);
    }
    for (i = 0; i < op->output_size; ++i) {
        tensor_alloc(&op->output_tensors[i]);
    }
    int channel = op->input_tensors[0]->shape.dim[1];
    switch (op->input_tensors[0]->dtype) {
    case dtFLOAT32:
        op->run_func = op_default_hswish_run;
        break;
    default:
        CHECK(false);
        break;
    }
}
