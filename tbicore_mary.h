#ifndef TBICORE_MARY_H
#define TBICORE_MARY_H

#include <QObject>
#include <QQuickItem>
#include <QFile>
#include <QString>
#include <QRect>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include <QFile>
#include "tbiparameterclass_gausiandecluster.h"
#include "tbiparameterclass_motioncontrolparams.h"
#include "tbiparameterclass_pyloncameraparams.h"
#include "tbiparameterclass_ranscaparms.h"
#include <QDataStream>
#include "tbicore_max.h"
#include "tbicore_toby.h"
#include "tbicore_gary.h"

/*
 A Note About Settings File Layout:

 Settings are saved as bin files with the .tbi extension
 The Application will look in the /savefile subfolder
 The arrangment of the file is as follows.

 Mary Will Attempt to load the file default.tbi
*/

/**************************************************************
Mary
Description:
  Mary is the Record Keeper.
  Mary Interacts with Everyone.


  A Class to Encapsulate the storage of settings and value.
  This class is in carge of file handling and
 **************************************************************/
class Mary : public QObject
{
    Q_OBJECT

public:
    //Constructor and Destructor--------------------------------------------------------------
    explicit Mary(QObject *parent = nullptr);
    ~Mary();

    //General Useful Methohds-----------------------------------------------------------------
    void SetDefaultValues();
    Q_INVOKABLE void saveSettingsToDefaultFile();
    Q_INVOKABLE void loadSettingsFromDefaultFile();
    void setTBICorePointers(Max *_max, Toby *_toby, Gary *_gary){m_max = _max; m_toby=_toby; m_gary=_gary;}

signals:
    void showDebugInfoChanged();

public slots:

private:
    const bool m_show_debug = false;
    Max *m_max;
    Toby *m_toby;
    Gary *m_gary;
};

#endif // TBICORE_MARY_H
