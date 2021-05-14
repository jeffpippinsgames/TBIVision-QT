#ifndef PIXELFUNDAMENTAL_H
#define PIXELFUNDAMENTAL_H

struct PixelFundamental_t
{
    int row;
    int col;
    int intensity;

    PixelFundamental_t(int _row, int _col, int _intensity){row = _row; col = _col; intensity = _intensity;}
    PixelFundamental_t(const PixelFundamental_t& _pixel){row = _pixel.row; col = _pixel.col; intensity = _pixel.intensity;}
};

#endif // PIXELFUNDAMENTAL_H
