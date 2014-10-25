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


import os, os.path, sys, math
import unittest
import numpy

import hsc.fitsthumb as fitsthumb

g_srcImage = None
def getSrcImage():
    global g_srcImage
    if g_srcImage != None:
        return g_srcImage

    height  = 500
    width   = 250
    density = 1e-2

    nObjects = int(height*width*density)

    g_srcImage = numpy.random.normal(size=(height, width))
    fluxes = 10 ** (0.4 * numpy.random.exponential(scale = 1.0 / math.log(2), size = (nObjects,)))
    positions_x = numpy.random.uniform(0, width , size = (nObjects,))
    positions_y = numpy.random.uniform(0, height, size = (nObjects,))

    for i in xrange(nObjects):
        xB = max(0, int(positions_x[i])-10)
        yB = max(0, int(positions_y[i])-10)
        xE = min(width , int(positions_x[i])+10)
        yE = min(height, int(positions_y[i])+10)

        x = numpy.arange(xB, xE) - positions_x[i]
        y = numpy.arange(yB, yE) - positions_y[i]
        y.shape = (yE-yB,1)
        g_srcImage[yB:yE, xB:xE] += fluxes[i]*numpy.exp(-(x*x + y*y))

    return g_srcImage


class FitsThumbTest(unittest.TestCase):
    """Test that fitsthumb works"""
    def setUp(self):
        self.image = getSrcImage()

    def tearDown(self):
        pass

    def test_outputJpg(self):
        fitsthumb.createThumbnail(
            self.image, "test-outputJpg.jpg",
            fitsthumb.AbsoluteSize(), fitsthumb.LinearScale()
        )
        os.remove("test-outputJpg.jpg");

    def test_linearScale(self):
        fitsthumb.createThumbnail(
            self.image, "test-linearScale.png",
            fitsthumb.AbsoluteSize(), fitsthumb.LinearScale()
        )
        os.remove("test-linearScale.png");

    def test_logScale(self):
        fitsthumb.createThumbnail(
            self.image, "test-logScale.png",
            fitsthumb.AbsoluteSize(), fitsthumb.LogScale()
        )
        os.remove("test-logScale.png");

    def test_relativeSize(self):
        fitsthumb.createThumbnail(
            self.image, "test-relativeSize.png",
            fitsthumb.RelativeSize(0.5), fitsthumb.LogScale()
        )
        os.remove("test-relativeSize.png");

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
