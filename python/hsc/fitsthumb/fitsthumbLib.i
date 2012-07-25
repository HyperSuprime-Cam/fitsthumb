// -*- lsst-c++ -*-
%define fitsthumbLib_DOCSTRING
"
Python interface to hsc::fitsthumb
"
%enddef

%feature("autodoc", "1");
%module(package="hsc.fitsthumb", docstring=fitsthumbLib_DOCSTRING) fitsthumbLib

%{
#include "lsst/afw.h"
#include "lsst/pex/logging.h"
%}

%include "lsst/p_lsstSwig.i"

%import "lsst/afw/image/imageLib.i"
%import "lsst/afw/math/mathLib.i"
%import "lsst/afw/cameraGeom/cameraGeomLib.i"

%{
#include "hsc/fitsthumb/ZScale.h"
#include "hsc/fitsthumb/InFITS.h"
#include "hsc/fitsthumb/Output.h"
#include "hsc/fitsthumb/Resize.h"
#include "hsc/fitsthumb/CommandLine.h"
#include "hsc/fitsthumb/FitsThumbFuncs.h"
#include <cstdio>
%}

%include "hsc/fitsthumb/FitsThumbFuncs.h"

