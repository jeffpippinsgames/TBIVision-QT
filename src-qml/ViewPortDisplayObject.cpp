#include "ViewPortDisplayObject.h"
//#include <opencv2/imgproc.hpp>

using namespace cv;
//-----------------------------------------------------------------------------
ViewPortDisplayObject::ViewPortDisplayObject()
{
        m_frame = new QImage(720, 540, QImage::Format_RGB888);
}
//-----------------------------------------------------------------------------
void ViewPortDisplayObject::NewFrameFromCVMatHandler(cv::Mat const& _matsrc)
{   
    Mat tempmat(_matsrc.cols,_matsrc.rows,_matsrc.type()); // make the same cv::Mat
    cvtColor(_matsrc, tempmat, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    m_frame = new QImage((uchar*) tempmat.data, tempmat.cols, tempmat.rows, tempmat.step, QImage::Format_RGB888); //create new Image
    m_frame->bits(); //Force a Deep Copy
    update();
    NewFrameRecieved(); //Fire Signal
}
//-----------------------------------------------------------------------------
void ViewPortDisplayObject::paint(QPainter *painter) //Overloaded paint
{
    painter->drawImage(0,0, *m_frame);
}
//-----------------------------------------------------------------------------
