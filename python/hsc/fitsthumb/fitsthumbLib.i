// -*- lsst-c++ -*-
%define fitsthumbLib_DOCSTRING
"
Python interface to hsc::onsite::qa::fitsthumb
"
%enddef

%feature("autodoc", "1");
%module(package="hsc.onsite.qa.fitsthumb", docstring=fitsthumbLib_DOCSTRING) fitsthumbLib

%{
#include "lsst/afw.h"
#include "lsst/pex/logging.h"
%}

%include "lsst/p_lsstSwig.i"

%import "lsst/afw/image/imageLib.i"
%import "lsst/afw/math/mathLib.i"
%import "lsst/afw/cameraGeom/cameraGeomLib.i"

%{
#include "hsc/onsite/qa/fitsthumb/ZScale.h"
#include "hsc/onsite/qa/fitsthumb/InFITS.h"
#include "hsc/onsite/qa/fitsthumb/Output.h"
#include "hsc/onsite/qa/fitsthumb/Resize.h"
#include "hsc/onsite/qa/fitsthumb/CommandLine.h"
#include "hsc/onsite/qa/fitsthumb/FitsThumbFuncs.h"
#include <cstdio>
%}

%include "hsc/onsite/qa/fitsthumb/FitsThumbFuncs.h"

