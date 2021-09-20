#include "tbicore_mary.h"
#include <QDebug>

/**************************************************************
Mary(QObject *parent) : QObject(parent)
Public
Description:
  Mary Constructor
**************************************************************/
Mary::Mary(QObject *parent) : QObject(parent)
{

    if(m_show_debug) qDebug() << "Mary::Mary() Mary Object Created.";

}

/**************************************************************
~Mary(QObject *parent) : QObject(parent)
Public
Description:
  Mary Destructor
**************************************************************/
Mary::~Mary()
{
    if(m_show_debug) qDebug() << "Mary::~Mary Mary Object Destroyed.";
}

/**************************************************************
SetMaryDefaultValues()
Public
Description:
  Sets The Default Values and Triggers the Changes.
**************************************************************/
void Mary::SetDefaultValues()
{

}

/**************************************************************
saveMaryToFile()
Signal
Description:
  saves Mary To the default File.
**************************************************************/
void Mary::saveSettingsToDefaultFile()
{
    QString _filepath = qApp->applicationDirPath() + "/savefile/marydefault.tbi";
    QFile _savefile(_filepath);
    _savefile.open(QIODevice::WriteOnly);
    QDataStream _ds(&_savefile);

    m_max->saveToFile(_ds);
    m_toby->saveToFile(_ds);

    _ds.setVersion(QDataStream::Qt_5_12);
    _savefile.close();
    if(m_show_debug) qDebug() << "Mary::saveMaryToFile() Settings Saved To " << _filepath;
}

/**************************************************************
loadMaryFromFile()
Signal
Description:
  Loads Mary From The Default File
**************************************************************/
void Mary::loadSettingsFromDefaultFile()
{
    QString _filepath = qApp->applicationDirPath() + "/savefile/marydefault.tbi";
    QFile _savefile(_filepath);
    if(!_savefile.exists())
    {
        if(m_show_debug) qDebug("Mary::loadMaryFromFile() marydefualt.tbi Does Not Exsist. Loading Default Values.");
        return;
    }
    _savefile.open(QIODevice::ReadOnly);
    QDataStream _ds(&_savefile);
    m_max->loadFromFile(_ds);
    m_toby->loadFromFile(_ds);
    _savefile.close();
    if(m_show_debug) qDebug("Mary::loadMaryFromFile() marydefualt.tbi Loaded.");
}
