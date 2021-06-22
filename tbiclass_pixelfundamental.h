#ifndef TBICLASS_PIXELFUNDAMENTAL_H
#define TBICLASS_PIXELFUNDAMENTAL_H

struct TBIPixelFundamental
{
    int row;
    int col;
    int intensity;

    TBIPixelFundamental(){row = 0; col=0; intensity = -1;}
    TBIPixelFundamental(int _row, int _col, int _intensity){row = _row; col = _col; intensity = _intensity;}
    TBIPixelFundamental(const TBIPixelFundamental& _pixel){row = _pixel.row; col = _pixel.col; intensity = _pixel.intensity;}
};

#endif // TBICLASS_PIXELFUNDAMENTAL_H
