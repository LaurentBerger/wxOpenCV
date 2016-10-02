#ifndef __I3THERMALSYSTEM__
#define __I3THERMALSYSTEM__
#include <opencv2/opencv.hpp>
#include <wx/file.h>

#include <fstream>

class I3ThermalSystem: public cv::VideoCapture
{
    wxFile irFile;
    cv::Mat dataI3;

public:
    /** @brief Default constructor
    @note In @ref videoio_c "C API", when you finished working with video, release CvCapture structure with
    cvReleaseCapture(), or use Ptr\<CvCapture\> that calls cvReleaseCapture() automatically in the
    destructor.
     */
    I3ThermalSystem();

    /** @overload
    @brief  Open video file or a capturing device or a IP video stream for video capturing

    Same as VideoCapture(const String& filename, int apiPreference) but using default Capture API backends
    */
    CV_WRAP I3ThermalSystem(const cv::String& filename);

    virtual ~I3ThermalSystem();

    /** @brief  Open video file or a capturing device or a IP video stream for video capturing

    @overload

    Parameters are same as the constructor VideoCapture(const String& filename)
    @return `true` if the file has been successfully opened

    The method first calls VideoCapture::release to close the already opened file or camera.
     */
    virtual bool open(const cv::String& filename);

    virtual bool isOpened() const;

    /** @brief Closes video file or capturing device.

    The method is automatically called by subsequent VideoCapture::open and by VideoCapture
    destructor.

    The C function also deallocates memory and clears \*capture pointer.
     */
    virtual void release();

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
    virtual bool grab();

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
    virtual bool retrieve(cv::OutputArray image, int flag = 0);

    /** @brief Stream operator to read the next video frame.
    @sa read()
    */
    virtual I3ThermalSystem& operator >> (cv::Mat& image);

    /** @overload
    @sa read()
    */
    virtual I3ThermalSystem& operator >> (cv::UMat& image);

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
    virtual bool read(cv::OutputArray image);

    /** @brief Sets a property in the VideoCapture.

    @param propId Property identifier from cv::VideoCaptureProperties (eg. cv::CAP_PROP_POS_MSEC, cv::CAP_PROP_POS_FRAMES, ...)
    or one from @ref videoio_flags_others
    @param value Value of the property.
    @return `true` if the property is supported by backend used by the VideoCapture instance.
    @note Even if it returns `true` this doesn't ensure that the property
    value has been accepted by the capture device. See note in VideoCapture::get()
     */
    virtual bool set(int propId, double value);

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
    virtual double get(int propId) const;

    /** @brief Open video file or a capturing device or a IP video stream for video capturing with API Preference

    @overload

    Parameters are same as the constructor VideoCapture(const String& filename, int apiPreference)
    @return `true` if the file has been successfully opened

    The method first calls VideoCapture::release to close the already opened file or camera.
    */
    virtual bool open(const cv::String& filename, int apiPreference);

protected:
};
#endif
