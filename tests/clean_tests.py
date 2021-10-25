# MACVETH - clean_tests.py
#
# Copyright (c) Colorado State University. 2019-2021
# Copyright (c) Universidade da Coruña. 2020-2021
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
#
# Authors:
#     Marcos Horro <marcos.horro@udc.es>
#     Louis-Noël Pouchet <pouchet@colostate.edu>
#     Gabriel Rodríguez <grodriguez@udc.es>
#
# Contact:
#     Louis-Noël Pouchet <pouchet@colostate.edu>

import os


print("-- Cleaning files in tests...")
if os.path.isdir("integrationtests/macveth_dir"):
    os.system("rm -Rf integrationtests/macveth_dir")
if os.path.isdir("integrationtests/passed_tests"):
    os.system("rm -Rf integrationtests/passed_tests")
if os.path.isdir("integrationtests/failed_tests"):
    os.system("rm -Rf integrationtests/failed_tests")
if os.path.isdir("integrationtests/tmpfiles"):
    os.system("rm -Rf integrationtests/tmpfiles")
if os.path.isfile("integrationtests/macveth_compiler.log"):
    os.system("rm integrationtests/macveth_compiler.log")
