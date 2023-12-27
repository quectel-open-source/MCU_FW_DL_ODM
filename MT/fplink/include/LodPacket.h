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

#ifndef __LOD_PACKET_H_INCLUDED__
#define __LOD_PACKET_H_INCLUDED__

#include "fplink_export.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QPair>
#include <QVector>
#include <map>
#include <vector>
#include <list>
#include <string>
#include "libusb-1.0\msvc\stdint.h"

class FPLINK_API LodHeader
{
public:
    LodHeader();
    ~LodHeader();

    void insertField(const std::string& name, const std::string& value);
    const std::list<std::pair<std::string, std::string>>& getFields() const;

    bool validate();
    std::list<std::string> dump();

private:
    std::list<std::pair<std::string, std::string>> mFields;
};

class FPLINK_API LodPacket
{
public:
    LodPacket(const LodPacket& o): base(o.base), data(o.data) {}
    LodPacket(uint32_t base): base(base) {}
    ~LodPacket() {}

    uint32_t size();
    uint32_t fcs();
    bool copyData(LodPacket* o);

    static bool BaseLessThan(LodPacket* a, LodPacket* b);

    uint32_t base;
    std::vector<uint32_t> data;
};

class FPLINK_API LodData
{
public:
    LodData();
    ~LodData();

    bool load(char*fname);
    bool store(char *fname);

    void addPacket(LodPacket* packet);
    const std::list<LodPacket*>& packets() const;
    void clear();

    LodHeader* getHeader();
    void setHeader(LodHeader* header);
    uint32_t dataSize();

    LodData* clone();
    void mergeForFlash();
    bool mergeForDownload( uint32_t flashSize);
    bool mergeForUpdate( );

private:
    bool checkSectors();
    uint32_t calcChecksum();

    LodHeader* mHeader; // hold by me
    std::list<LodPacket*> mPackets; // hold by me
};

#endif
