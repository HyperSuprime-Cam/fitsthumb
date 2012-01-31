// -*- lsst-c++ -*-
%define fitsthumbLib_DOCSTRING
"
Python interface to hsc::onsite::qa::fitsthumb
"
%enddef

%feature("autodoc", "1");
%module(package="hsc.onsite.qa.fitsthumb", docstring=fitsthumbLib_DOCSTRING) fitsthumbLib

%{
#include "lsst/afw/image.h"
%}

%include "lsst/p_lsstSwig.i"

%pythoncode %{
import lsst.utils

def version(HeadURL = r"$HeadURL: ssh://hsc-gw2.mtk.nao.ac.jp//ana/hgrepo/fitsthumb/python/hsc/onsite/qa/fitsthumbLib.i $"):
    version_svn = lsst.utils.guessSvnVersion(HeadURL)

    try:
	import eups
    except ImportError:
        return version_svn
    else:
	try:
	    version_eups = eups.setup("fitsthumb")
	except AttributeError:
	    return version_svn

    if version_eups == version_svn:
        return version_svn
    else:
	return "%s (setup: %s)" % (version_svn, version_eups)
%}

%import "lsst/afw/image/imageLib.i"
%import "lsst/afw/math/mathLib.i"
%import "lsst/afw/cameraGeom/cameraGeomLib.i"

%include "fitsthumb.i"


