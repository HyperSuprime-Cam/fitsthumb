#!/usr/bin/env python

import sys
import hsc.fitsthumb as qaFitsthumb
import pyfits

def main():

    # From file to file
    infile = 'SUPA01047170.fits'
    outfile1 = 'SUPA01047170_1.png'
    xSize = 300
    ySize = 0   # keep aspect ratio
    dynamicRangeFirst = True

    ret1 = qaFitsthumb.createFitsThumb(infile, outfile1, xSize, ySize, dynamicRangeFirst)

    # From numpy array to file
    outfile2 = 'SUPA01047170_2.png'
    image = pyfits.open(infile)[0].data
    if not image.dtype.isnative:
        image.byteswap(True)
        image = image.newbyteorder()

    ret2 = qaFitsthumb.createFitsThumb(image, outfile2, xSize, ySize, dynamicRangeFirst)


if __name__ == '__main__':
    main()


