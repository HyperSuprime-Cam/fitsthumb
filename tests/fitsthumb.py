#!/usr/bin/env python

#
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <http://www.lsstcorp.org/LegalNotices/>.
#


import os, os.path, sys
import unittest
import numpy

import hsc.fitsthumb as fitsthumb


class FitsThumbTest(unittest.TestCase):
    """Test that fitsthumb works"""
    def setUp(self):
        self.image = numpy.random.normal(size=(256, 128))

    def tearDown(self):
        del self.image

    def write(self, format):
        assert format in ("png", "jpg")
        height, width = self.image.shape
        fitsthumb.createFitsThumb(self.image, "test-linear." + format,
                                  min(width, height, 500), 0, True)
        fitsthumb.createLogFitsThumb(self.image, "test-log." + format,
                                  min(width, height, 500), 0, True)

    def testPng(self):
        self.write("png")

    def testJpg(self):
        self.write("jpg")

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

def suite():
    """Returns a suite containing all the test cases in this module."""
    suites = []
    suites += unittest.makeSuite(FitsThumbTest)

    return unittest.TestSuite(suites)

def run(shouldExit=False):
    """Run the tests"""
    if unittest.TextTestRunner().run(suite()).wasSuccessful():
        status = 0
    else:
        status = 1

    if shouldExit:
        sys.exit(status)
    else:
        return status

if __name__ == "__main__":
    run(True)
