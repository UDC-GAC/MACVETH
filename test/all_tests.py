#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : all_tests.py
# Author            : Marcos Horro <marcos.horro@udc.gal>
# Date              : Ven 08 Nov 2019 08:44:52 MST
# Last Modified Date: Ven 08 Nov 2019 08:44:52 MST
# Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
import os
import sys
import unittest

if not os.path.isdir('inputs') or not os.path.isdir('build'):
    print('ERROR: Please execute from the project root directory.')
    print('.....  And build the samples first.')
    sys.exit(1)
else:
    tests = unittest.TestLoader().discover('test', 'test*.py', 'test')
    result = unittest.TextTestRunner().run(tests)
    sys.exit(0 if result.wasSuccessful() else 1)
