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


import os, os.path
import unittest
import random

import lsst.afw.image as afwImage
import hsc.fitsthumb as fitsthumb

import lsst.utils.tests as utilsTests



class FitsThumbTest(unittest.TestCase):
    """Test that fitsthumb works"""
    def setUp(self):
        image = afwImage.ImageF(128, 256)
        for y in range(image.getHeight()):
            for x in range(image.getWidth()):
                image.set(random.gauss(0.0, 1.0))
        self.image = image

    def tearDown(self):
        del self.image

    def write(self, format):
        assert format in ("png", "jpg")
        fitsthumb.createFitsThumb(self.image, "test." + format, format,
                                  min(self.image.getWidth(), self.image.getHeight(), 500), 0, True)

    def testPng(self):
        self.write("png")

    def testJpg(self):
        self.write("jpg")

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

def suite():
    """Returns a suite containing all the test cases in this module."""
    utilsTests.init()

    suites = []
    suites += unittest.makeSuite(FitsThumbTest)
    suites += unittest.makeSuite(utilsTests.MemoryTestCase)

    return unittest.TestSuite(suites)

def run(shouldExit=False):
    """Run the tests"""
    utilsTests.run(suite(), shouldExit)

if __name__ == "__main__":
    run(True)
