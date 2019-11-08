#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : test_s2s_translator.py
# Author            : Marcos Horro <marcos.horro@udc.gal>
# Date              : Ven 08 Nov 2019 09:06:23 MST
# Last Modified Date: Ven 08 Nov 2019 09:06:38 MST
# Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
import os
import sys
import unittest
from tools import SamplesTestCase

OUTPUT = '''\
// Begin function foo returning void
void foo(int* a, int *b) {
  if (a[0] > 1) // the 'if' part
  {
    b[0] = 2;
  }
}
// End function foo

void bar(float x, float y); // just a declaration
'''

PROG = 's2s_translator'


class TestRewriterSample(SamplesTestCase):
    def test_live(self):
        self.assertSampleOutput([PROG], 'kernel.c', OUTPUT)


if __name__ == '__main__':
    unittest.main()
