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

import torch.nn as nn
from .layer import LayerTestUnit


class ReLUTestUnit(LayerTestUnit):
    def test_relu(self):
        model = nn.ReLU()
        name = "relu"
        input_shapes = [(1, 14, 14)]
        input_names = ["data"]
        output_names = ["output"]
        self.compare(model, input_shapes, name, input_names, output_names)

    def test_leaky_relu(self):
        model = nn.LeakyReLU(negative_slope=1e-1)
        name = "leaky_relu"
        input_shapes = [(1, 14, 14)]
        input_names = ["data"]
        output_names = ["output"]
        self.compare(model, input_shapes, name, input_names, output_names)
