/* Copyright (C) 2015 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef __SETTING_LOADER_H_INCLUDED__
#define __SETTING_LOADER_H_INCLUDED__

#include "fplink_export.h"
#include <QString>
#include <QStringList>
#include <QSettings>
#include <stdint.h>

class FPLINK_API SettingLoader
{
public:
    SettingLoader(const QString& fname, bool isLoad);
    ~SettingLoader();

    bool isLoad() const ;
    void beginGroup(const QString& group);
    void endGroup();

    void value(const QString& key, QList<uint32_t>& val);
    void value(const QString& key, QByteArray& val);
    void value(const QString& key, QStringList& val);
    void value(const QString& key, QString& val);
    void value(const QString& key, std::string& val);
    void value(const QString& key, float& val);
    void value(const QString& key, int& val);
    void value(const QString& key, unsigned int& val);
    void value(const QString& key, bool& val);
    void valueN(const QString& key, QString* val, int n);
    void valueN(const QString& key, float* val, int n);
    void valueN(const QString& key, int* val, int n);
    void valueN(const QString& key, unsigned int* val, int n);

private:
    bool mIsLoad;
    QSettings mSetting;
};

#endif
