#ifndef __PANORAMIQUE__
#define __PANORAMIQUE__

#includ "ImageInfo.h"
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
std::vector<cv::detail::ImageFeatures> features;
std::vector<ImageInfoCV *> img;
std::vector<cv::detail::MatchesInfo> appariement;
cv::detail::HomographyBasedEstimator estimateur;
cv::vector<detail::CameraParams> cameras;
cv::detail::BundleAdjusterBase adjuster;
cv::detail::ExposureCompensator correcteurExpo;
ccv::detail::SeamFinder couture;
detail::Blender blender;
};

#endif
