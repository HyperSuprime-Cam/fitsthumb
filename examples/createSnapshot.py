#!/usr/bin/env python

import sys
import lsst.afw.image as afwImage
import hsc.fitsthumb as qaFitsthumb

def main():

    infile = 'SUPA01047170.fits'
    outtype = 'png'
#    outtype = 'jpg'
    outfile1 = 'SUPA01047170_1.png'
    outfile2 = 'SUPA01047170_2.png'
    xSize = 300
    ySize = 0   # keep aspect ratio
    dynamicRangeFirst = True

    metadata = afwImage.readMetadata(infile)
#    image = afwImage.ImageF(infile, 0, metadata)
    image = afwImage.ImageF(infile, 0, metadata)
    print type(image)
    ret1 = qaFitsthumb.createFitsThumb(infile, outfile1, outtype, xSize, ySize, dynamicRangeFirst)
    ret2 = qaFitsthumb.createFitsThumb(image, outfile2, outtype, xSize, ySize, dynamicRangeFirst)


if __name__ == '__main__':
    main()


