#ifndef PIXELANALYZER_H
#define PIXELANALYZER_H

#include <QObject>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "pcolumn.h"



//The Pixel Analyzer Class
class PixelAnalyzer: public QObject
{
    Q_OBJECT

public:
    PixelAnalyzer();
    ~PixelAnalyzer();

    void buildPixelColumns(cv::Mat &_mat);
    void AnalyzeMat(cv::Mat &_mat);

private:

    long m_total_intensity;
};

#endif // PIXELANALYZER_H
