// -*- lsst-c++ -*-
%define fitsthumbLib_DOCSTRING
"
Python interface to hsc::fitsthumb
"
%enddef

%feature("autodoc", "1");
%module(package="hsc.fitsthumb", docstring=fitsthumbLib_DOCSTRING) fitsthumbLib

%{
    #define SWIG_FILE_WITH_INIT
    #include "hsc/fitsthumb.h"
    #include "hsc/fitsthumb/SizeOption.h"
    #include "hsc/fitsthumb/ScaleOption.h"
    #include <stdexcept>

    // Because of SWIG's bug, this is necessary
    using namespace hsc::fitsthumb;
%}


%include "numpy.i"
%init %{
    import_array();
%}

%include "exception.i"
%exception {
    try {
        $action
    }
    catch (const std::bad_alloc& e) {
        SWIG_exception(SWIG_MemoryError, e.what());
    }
    catch (const std::out_of_range& e) {
        SWIG_exception(SWIG_IndexError, e.what());
    }
    catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (...) {
        SWIG_exception(SWIG_RuntimeError, "unknown exception object was thrown");
    }
}

%include "Image.i"

%include "hsc/fitsthumb/SizeOption.h"
%include "hsc/fitsthumb/ScaleOption.h"
%include "hsc/fitsthumb.h"

// The instantiation order must be float -> double
// because argument match condition of the float version is stricter.
//
// **Warning may arise from swig saying
//    "Warning 509: Overloaded method is effectively ignored..."
//   but the method is NOT ignored because of the difference of the
//   argument match condition between float & double

%template (createThumbnail) hsc::fitsthumb::createThumbnail<float>;
%template (createThumbnail) hsc::fitsthumb::createThumbnail<double>;

// deprecated API
%template (createFitsThumb) hsc::fitsthumb::createFitsThumb<float>;
%template (createFitsThumb) hsc::fitsthumb::createFitsThumb<double>;
