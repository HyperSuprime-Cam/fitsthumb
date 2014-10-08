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
    #include <stdexcept>
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

%include "hsc/fitsthumb.h"

%template (createFitsThumb) hsc::fitsthumb::createFitsThumb<float>;
%template (createFitsThumb) hsc::fitsthumb::createFitsThumb<double>;

%template (createLogFitsThumb) hsc::fitsthumb::createLogFitsThumb<float>;
%template (createLogFitsThumb) hsc::fitsthumb::createLogFitsThumb<double>;
