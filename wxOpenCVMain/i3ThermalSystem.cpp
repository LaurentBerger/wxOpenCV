#include "i3ThermalSystem.hpp"
#include <wx/file.h>

using namespace cv;

I3ThermalSystem::I3ThermalSystem()
{
}
;

I3ThermalSystem::I3ThermalSystem(const String& filename)
{
dataI3=NULL; 
irFile.Open("\\\\.\\pipe\\thermalExpert",wxFile::read_write);

if (irFile.IsOpened())
{
    dataI3 =Mat(Size(384, 288),CV_16UC1,Scalar::all(0));
}
else
{
    dataI3= Mat();
}

}


I3ThermalSystem::~I3ThermalSystem()
{
    irFile.Close();
}

bool I3ThermalSystem::open(const String& filename){return false;};

bool I3ThermalSystem::isOpened() const { return irFile.IsOpened(); };

    /** @brief Closes video file or capturing device.

    The method is automatically called by subsequent VideoCapture::open and by VideoCapture
    destructor.

    The C function also deallocates memory and clears \*capture pointer.
     */
void I3ThermalSystem::release() {irFile.Close();};

    /** @brief Grabs the next frame from video file or capturing device.

    @return `true` (non-zero) in the case of success.

    The method/function grabs the next frame from video file or camera and returns true (non-zero) in
    the case of success.

    The primary use of the function is in multi-camera environments, especially when the cameras do not
    have hardware synchronization. That is, you call VideoCapture::grab() for each camera and after that
    call the slower method VideoCapture::retrieve() to decode and get frame from each camera. This way
    the overhead on demosaicing or motion jpeg decompression etc. is eliminated and the retrieved frames
    from different cameras will be closer in time.

    Also, when a connected camera is multi-head (for example, a stereo camera or a Kinect device), the
    correct way of retrieving data from it is to call VideoCapture::grab() first and then call
    VideoCapture::retrieve() one or more times with different values of the channel parameter.

    @ref tutorial_kinect_openni
     */
 bool I3ThermalSystem::grab() { return irFile.IsOpened(); };

    /** @brief Decodes and returns the grabbed video frame.

    @param [out] image the video frame is returned here. If no frames has been grabbed the image will be empty.
    @param flag it could be a frame index or a driver specific flag
    @return `false` if no frames has been grabbed

    The method decodes and returns the just grabbed frame. If no frames has been grabbed
    (camera has been disconnected, or there are no more frames in video file), the method returns false
    and the function returns an empty image (with %cv::Mat, test it with Mat::empty()).

    @sa read()

    @note In @ref videoio_c "C API", functions cvRetrieveFrame() and cv.RetrieveFrame() return image stored inside the video
    capturing structure. It is not allowed to modify or release the image! You can copy the frame using
    :ocvcvCloneImage and then do whatever you want with the copy.
     */
 bool I3ThermalSystem::retrieve(OutputArray image, int flag ) 
 { 
     image.create(288, 384, CV_16UC1);
     if (!image.isContinuous())
     {
         return false;
     }
     char c = 1;
     irFile.Write(&c, 1);
     int nb=irFile.Read(image.getMat().ptr(0), 384 * 288 * 2);
     return nb== 384 * 288 * 2;
 }

    /** @brief Stream operator to read the next video frame.
    @sa read()
    */
 I3ThermalSystem& I3ThermalSystem::operator >> ( Mat& image) { 
     if (image.size() != cv::Size(384, 288) || image.type() == CV_16UC1)
     {
         image.release();
         image.create(cv::Size(384, 288), CV_16UC1);
     }
     if (!image.isContinuous())
     {
         image.release();
         return *this;
     }
     char c = 1;
     irFile.Write(&c, 1);
     irFile.Read(image.ptr(0), 384 * 288 * 2);
     return *this;
     return *this;
 };

    /** @overload
    @sa read()
    */
I3ThermalSystem& I3ThermalSystem::operator >> ( UMat& image) 
{ 
    if (image.size() != cv::Size(384, 288) || image.type() == CV_16UC1)
    {
        image.release();
        image.create(cv::Size(384, 288),CV_16UC1);
    }
    if (!image.isContinuous())
    {
        image.release();
        return *this;
    }
    char c=1;
    irFile.Write(&c,1);
    irFile.Read(dataI3.ptr(0),384*288*2);
    dataI3.copyTo(image);
    return *this; 
}


    /** @brief Grabs, decodes and returns the next video frame.

    @param [out] image the video frame is returned here. If no frames has been grabbed the image will be empty.
    @return `false` if no frames has been grabbed

    The method/function combines VideoCapture::grab() and VideoCapture::retrieve() in one call. This is the
    most convenient method for reading video files or capturing data from decode and returns the just
    grabbed frame. If no frames has been grabbed (camera has been disconnected, or there are no more
    frames in video file), the method returns false and the function returns empty image (with %cv::Mat, test it with Mat::empty()).

    @note In @ref videoio_c "C API", functions cvRetrieveFrame() and cv.RetrieveFrame() return image stored inside the video
    capturing structure. It is not allowed to modify or release the image! You can copy the frame using
    :ocvcvCloneImage and then do whatever you want with the copy.
     */
bool I3ThermalSystem::read(OutputArray image) { return false; };

    /** @brief Sets a property in the VideoCapture.

    @param propId Property identifier from cv::VideoCaptureProperties (eg. cv::CAP_PROP_POS_MSEC, cv::CAP_PROP_POS_FRAMES, ...)
    or one from @ref videoio_flags_others
    @param value Value of the property.
    @return `true` if the property is supported by backend used by the VideoCapture instance.
    @note Even if it returns `true` this doesn't ensure that the property
    value has been accepted by the capture device. See note in VideoCapture::get()
     */
bool I3ThermalSystem::set(int propId, double value) { return false; };

    /** @brief Returns the specified VideoCapture property

    @param propId Property identifier from cv::VideoCaptureProperties (eg. cv::CAP_PROP_POS_MSEC, cv::CAP_PROP_POS_FRAMES, ...)
    or one from @ref videoio_flags_others
    @return Value for the specified property. Value 0 is returned when querying a property that is
    not supported by the backend used by the VideoCapture instance.

    @note Reading / writing properties involves many layers. Some unexpected result might happens
    along this chain.
    @code {.txt}
    `VideoCapture -> API Backend -> Operating System -> Device Driver -> Device Hardware`
    @endcode
    The returned value might be different from what really used by the device or it could be encoded
    using device dependant rules (eg. steps or percentage). Effective behaviour depends from device
    driver and API Backend

    */
double I3ThermalSystem::get(int propId) const { return 0; };

    /** @brief Open video file or a capturing device or a IP video stream for video capturing with API Preference

    @overload

    Parameters are same as the constructor VideoCapture(const String& filename, int apiPreference)
    @return `true` if the file has been successfully opened

    The method first calls VideoCapture::release to close the already opened file or camera.
    */
 bool I3ThermalSystem::open(const String& filename, int apiPreference) { return 0; };
