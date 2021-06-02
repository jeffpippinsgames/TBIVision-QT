#include "pixelclusterclass.h"

PixelClusterClass::PixelClusterClass()
{
    m_pixels.clear();
    m_highestintensityindex = -1;
    m_highestintensityvalue = -1;
    m_rowcentroid = -1;
}

void PixelClusterClass::pushPixelToBack(PixelFundamental_t _pixel)
{
    m_pixels.push_back(_pixel);
    if(_pixel.intensity >= m_highestintensityvalue)
    {
        m_highestintensityvalue = _pixel.intensity;
        m_highestintensityindex = m_pixels.size() - 1;
    }
}

void PixelClusterClass::setRowCentroid()
{
    //calculate the weighted average using intensity as a weight
    int _numofpixels = m_pixels.size();
    if(_numofpixels == 0 ) return;
    float _weight_sum = 0.0;
    float _weight_data_product_sum = 0.0;
    int _element = 0;
    do
    {
        _weight_sum += m_pixels[_element].intensity;
        _weight_data_product_sum += (m_pixels[_element].row * m_pixels[_element].intensity);
        ++_element;
    }while(_element < _numofpixels);
    m_rowcentroid = _weight_data_product_sum/_weight_sum;
}



bool PixelClusterClass::isGausian()
{
    int _numofpixels = m_pixels.size();
    if(_numofpixels == 0 ) return false;
    if(_numofpixels < 3) return true;
    int _element = 1;
    bool _hithighvalue = false;


    do
    {
        if((m_pixels[_element].intensity < m_pixels[_element-1].intensity) && !_hithighvalue)
        {
            _hithighvalue = true;
        }
        else if(_hithighvalue && (m_pixels[_element].intensity >= m_pixels[_element-1].intensity))
        {
            return false;
        }
        ++_element;
    }while(_element < _numofpixels);

    if(!_hithighvalue) return false;

    return true;
}

void PixelClusterClass::clear()
{
    m_pixels.clear();
    m_highestintensityindex = -1;
    m_highestintensityvalue = -1;
    m_rowcentroid = -1;
}

int PixelClusterClass::size()
{
    return m_pixels.size();
}

void PixelClusterClass::DrawToMat(cv::Mat &_mat)
{
    if(_mat.channels() != 1) return;
    if(m_pixels.size() == 0) return;

    int _max_elements = _mat.rows * _mat.cols;
    int _dataindex;
    int _element = 0;
    uint8_t * _data = (uint8_t*)_mat.data;

    do
    {
        _dataindex = (m_pixels[_element].row * _mat.cols) + m_pixels[_element].col;
        if(_dataindex < _max_elements)
        {
            _data[_dataindex] = m_pixels[_element].intensity;
        }
        ++_element;
    }while(_element < (int)m_pixels.size());
}

void PixelClusterClass::EraseFromMat(cv::Mat &_mat)
{
    if(_mat.channels() != 1) return;
    if(m_pixels.size() == 0) return;

    int _max_elements = _mat.rows * _mat.cols;
    int _dataindex;
    int _element = 0;
    uint8_t * _data = (uint8_t*)_mat.data;

    do
    {
        _dataindex = (m_pixels[_element].row * _mat.cols) + m_pixels[_element].col;
        if(_dataindex < _max_elements)
        {
            _data[_dataindex] = 0;
        }
        ++_element;
    }while(_element < (int)m_pixels.size());
}

int PixelClusterClass::getColumn()
{
    if(m_pixels.size() == 0) return -1;
    return m_pixels[0].col;
}

bool PixelClusterClass::addPixelForGausianCluster(PixelFundamental_t _pixel)
{
    //There are no pixels add it automaticly
    if(m_pixels.size() == 0)
    {
        m_highestintensityvalue = _pixel.intensity;
        m_highestintensityindex = 0;
        m_pixels.push_back(_pixel);
        return true;
    }

    /*
     The Gausian Distrubtion pixel add requires a complete gausian distribution.
     It Requires a build up to a maximum value then a build up to a  min value.
     */

    //The High Side Rising the when the new incomming intensity is greater or equal the last pixel entered
    if((_pixel.intensity >= m_pixels[m_pixels.size()-1].intensity) && (m_highestintensityindex == (int)(m_pixels.size() - 1))) //The New Incomming Pixel is More or equally intense than the highest inten
    {
        //Update the highest intensity values and index
        m_highestintensityvalue = _pixel.intensity;
        m_highestintensityindex = m_pixels.size();
        //add the pixel to the cluster
        m_pixels.push_back(_pixel);
        return true;
    }
    //Else the pixel is less than the high spot. The High Intensity has already been hit.
    //Keep Adding pixels until the intensity increases then return a false
    else
    {
        //The New incomming pixel is greater than the last pixel entered. We cannot add it. Return False;
        if(_pixel.intensity > m_pixels[m_pixels.size()-1].intensity)
        {
            return false;
        }

        //Make sure there is a lead up of intensities.
        //The high value index cannot be 0
        if(m_pixels.size() == 1)
        {
            //Clear the vector and return false
            this->clear();
            return false;
        }

        //add the new pixel
        m_pixels.push_back(_pixel);
        return true;
    }

    //The pixel was added. return true.
    return false;
}

int PixelClusterClass::getHighestInstensityPixelValue()
{
    return m_highestintensityvalue;

}

int PixelClusterClass::getHighestIntensityPixelIndex()
{
    return m_highestintensityindex;
}




