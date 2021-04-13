#ifndef MAX_H
#define MAX_H

#include <QObject>
#include <QImage>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

using namespace cv;

class MaxColumnite
{

};

/**************************************************************
Max
Description:
  max is the welder. The seam tracking functionality is
  encapsulated inside Max.
 **************************************************************/
class Max : public QObject
{


    Q_OBJECT
public:
    explicit Max(QObject *parent = nullptr);

private:
    cv::Mat m_raw_mat;
    cv::Mat m_blurred_mat;
    cv::Mat m_threshold_mat



signals:

};

#endif // MAX_H
