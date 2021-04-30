#ifndef PFUNDAMENTAL_H
#define PFUNDAMENTAL_H

#include <QObject>

class PFundamental : public QObject
{
    Q_OBJECT
public:
    explicit PFundamental(QObject *parent = nullptr);


signals:

public:
    int m_intensity;
    int m_row;
    int m_col;
};

#endif // PFUNDAMENTAL_H
