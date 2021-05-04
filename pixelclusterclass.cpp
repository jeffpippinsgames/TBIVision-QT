#include "pixelclusterclass.h"

PixelClusterClass::PixelClusterClass()
{

}

void PixelClusterClass::pushPixelToBack(PixelFundamental_t _pixel)
{
    m_pixels.push_back(_pixel);
}

float PixelClusterClass::getRowCentroid()
{
    //calculate the weighted average using intensity as a weight
    int _numofpixels = m_pixels.size();
    if(_numofpixels == 0 ) return 0.0;
    float _weight_sum = 0.0;
    float _weight_data_product_sum = 0.0;
    int _element = 0;
    do
    {
        _weight_sum += m_pixels[_element].intensity;
        _weight_data_product_sum += (m_pixels[_element].row * m_pixels[_element].intensity);
        ++_element;
    }while(_element < _numofpixels);
    return _weight_data_product_sum/_weight_sum;
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
        if((m_pixels[_element].intensity < m_pixels[_element - 1].intensity) && !_hithighvalue)
        {
            _hithighvalue = true;
        }
        else if(_hithighvalue && (m_pixels[_element].intensity > m_pixels[_element - 1].intensity))
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




