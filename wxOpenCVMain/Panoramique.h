#ifndef __PANORAMIQUE__
#define __PANORAMIQUE__

class ImageInfoCV;

#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/timelapsers.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/util.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"

class Panoramique {
public :
double warped_image_scale ,seam_work_aspect;
std::vector<int> bijection;
std::vector<cv::detail::ImageFeatures> features;
std::vector<ImageInfoCV *> tabImg;
std::vector<cv::detail::MatchesInfo> appariement;
cv::detail::HomographyBasedEstimator estimateur;
std::vector<cv::detail::CameraParams> cameras;
cv::Ptr<cv::detail::BundleAdjusterBase> adjuster;
cv::Ptr<cv::detail::ExposureCompensator> correcteurExpo;
cv::Ptr<cv::detail::SeamFinder> couture;
cv::detail::Blender blender;
std::vector <ImageInfoCV *> op;	/*!< Opérande du panorama */
std:: vector<int>	indOpFenetre;	/*!< Indices des fenêtres  des opérandes */
};

#endif
