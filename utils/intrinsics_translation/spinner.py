'''
Filename: utils/intrinsics_translation/spinner.py

Created Date: Wednesday, October 21st 2020, 1:03:52 pm
Author: Marcos Horro

Copyright (c) 2021
'''

import time
import threading
import sys


class Spinner:
    busy = False
    delay = 0.075
    init_msg = ""
    end_msg = ""

    @staticmethod
    def spinning_cursor():
        while True:
            for cursor in "|/-\\":
                yield cursor

    def __init__(self, init_msg="Loading task...", end_msg="OK", delay=None):
        self.spinner_generator = self.spinning_cursor()
        self.init_msg = init_msg
        self.end_msg = end_msg
        if delay and float(delay):
            self.delay = delay

    def spinner_task(self):
        sys.stdout.write(self.init_msg)
        while self.busy:
            sys.stdout.write(next(self.spinner_generator))
            sys.stdout.flush()
            time.sleep(self.delay)
            sys.stdout.write("\b")
            sys.stdout.flush()
        sys.stdout.write(" \b")
        sys.stdout.write("\r" + " " * len(self.init_msg))
        sys.stdout.write("\r%s\n" % self.end_msg)

    def __enter__(self):
        self.busy = True
        threading.Thread(target=self.spinner_task).start()

    def __exit__(self, exception, value, tb):
        self.busy = False
        time.sleep(self.delay)
        if exception is not None:
            return False
