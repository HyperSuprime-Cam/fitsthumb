#!/usr/bin/env python

import sys
import hsc.fitsthumb as fitsthumb
import pyfits

def main():

    # From file to file
    infile = 'SUPA01047170.fits'
    outfile1 = 'SUPA01047170_1.png'
    xSize = 300
    ySize = 0   # keep aspect ratio

    fitsthumb.createThumbnail(
        infile, outfile1,
        fitsthumb.AbsoluteSize(xSize, ySize),
        fitsthumb.LinearScale()
    )

    # From numpy array to file
    outfile2 = 'SUPA01047170_2.png'
    image = pyfits.open(infile)[0].data
    if not image.dtype.isnative:
        image.byteswap(True)
        image = image.newbyteorder()

    ratio = 0.4  # resize 40%

    fitsthumb.createThumbnail(
        image, outfile2,
        fitsthumb.RelativeSize(ratio),
        fitsthumb.LogScale()
    )


if __name__ == '__main__':
    main()
