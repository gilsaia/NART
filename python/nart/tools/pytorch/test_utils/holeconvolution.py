# Copyright 2022 SenseTime Group Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import math
import torch.nn as nn
from .layer import LayerTestUnit


class HoleConvolutionTestUnit(LayerTestUnit):
    def test_holeconvolution(self):
        model = nn.Conv2d(3, 3, 3, 1, 1, dilation=2).eval()
        n = model.kernel_size[0] * model.kernel_size[1] * model.out_channels
        model.weight.data.normal_(0, math.sqrt(2.0 / n))
        model.bias.data.normal_(0, 1)
        name = "holeconvolution"
        input_shapes = [(3, 224, 224)]
        input_names = ["data"]
        output_names = ["output"]
        self.compare(model, input_shapes, name, input_names, output_names)
