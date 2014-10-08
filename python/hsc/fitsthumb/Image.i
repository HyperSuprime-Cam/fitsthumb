%{
    #include "hsc/fitsthumb/Image.h"
%}

namespace hsc { namespace fitsthumb {
    template <class T>
    struct Image {};
}}

/* plagiarized from numpy.i:
 * Typemap suite for (DATA_TYPE* IN_ARRAY2, DIM_TYPE DIM1, DIM_TYPE DIM2)
 */

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY,
           fragment="NumPy_Macros")
  hsc::fitsthumb::Image<float> const&
{
  $1 = is_array($input) && array_type($input) == NPY_FLOAT;
}
%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY,
           fragment="NumPy_Macros")
  hsc::fitsthumb::Image<double> const&
{
  $1 = is_array($input) || PySequence_Check($input);
}

%define %fitsthumb_numpy_typemaps(DATA_TYPE, DATA_TYPECODE)
%typemap(in,
         fragment="NumPy_Fragments")
  hsc::fitsthumb::Image<DATA_TYPE> const&
  (PyArrayObject* array=NULL, int is_new_object=0)
{
  DATA_TYPE*     data  ;
  std::size_t    height;
  std::size_t    width ;
  std::ptrdiff_t strideH;
  std::ptrdiff_t strideW;

  array = obj_to_array_allow_conversion($input, DATA_TYPECODE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 2) || !require_native(array)) SWIG_fail;
  data   = (DATA_TYPE*) array_data(array);
  height = array_size(array,0);
  width  = array_size(array,1);

  strideH = array_stride(array, 0);
  strideW = array_stride(array, 1);

  if(strideH % sizeof(DATA_TYPE) != 0){
    PyErr_SetString(PyExc_AssertionError, "Data not properly aligned");
    SWIG_fail;
  }
  if(strideW != sizeof(DATA_TYPE)){
    PyErr_SetString(PyExc_AssertionError, "Image must be contiguous horizontally");
    SWIG_fail;
  }

  try {
    $1 = new hsc::fitsthumb::Image<DATA_TYPE>(data, height, width, strideH / sizeof(DATA_TYPE));
  }
  catch(...){
    $1 = NULL;
  }

  if(! $1){
    PyErr_SetString(PyExc_MemoryError, "Not enough memory");
    SWIG_fail;
  }

}
%typemap(freearg)
  hsc::fitsthumb::Image<DATA_TYPE> const&
{
  if($1) delete $1;
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}

%enddef    /* %fitsthumb_numpy_typemaps() macro */

%fitsthumb_numpy_typemaps(float             , NPY_FLOAT )
%fitsthumb_numpy_typemaps(double            , NPY_DOUBLE)
