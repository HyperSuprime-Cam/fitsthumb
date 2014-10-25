#ifndef  gd37f497f_31f6_4532_a50c_9fc919b78ddc
#define  gd37f497f_31f6_4532_a50c_9fc919b78ddc

/** Size option that is used by Resize()
*/

namespace hsc { namespace fitsthumb {

namespace option {

    struct AbsoluteSize;

    struct Size
    {
        virtual bool IsAbsolute() const = 0;
        virtual AbsoluteSize ComputeActualSize(int srcWidth, int srcHeight) const = 0;
    };

    /** (width, height) of the output image.
        When either one of width or height <= 0,
            it is computed from the other so that the aspect ratio is conserved.
        When both of width and height <= 0,
            no resize occur.
    */
    struct AbsoluteSize : public Size
    {
        virtual bool IsAbsolute() const { return true; }
        virtual AbsoluteSize ComputeActualSize(int srcWidth, int srcHeight) const;

        int width ;
        int height;

        AbsoluteSize()
            : width ()
            , height()
        {}

        AbsoluteSize(int width_, int height_)
            : width (width_ )
            , height(height_)
        {}
    };

    /** (width, height) ratio relative to the input image.
        When either one of width or height <= 0,
            it is assumed to be the same as the other.
        (width <= 0 && height <= 0) means "no resize"
    */
    struct RelativeSize : public Size
    {
        virtual bool IsAbsolute() const { return false; }
        virtual AbsoluteSize ComputeActualSize(int srcWidth, int srcHeight) const;

        double width ;
        double height;

        RelativeSize()
            : width ()
            , height()
        {}

        RelativeSize(double ratio_)
            : width (ratio_)
            , height(ratio_)
        {}

        RelativeSize(double width_, double height_)
            : width (width_ )
            , height(height_)
        {}
    };

} // namespace option

}} // namespace hsc::fitsthumb
#endif //gd37f497f_31f6_4532_a50c_9fc919b78ddc
