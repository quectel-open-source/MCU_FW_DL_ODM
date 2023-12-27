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

#ifndef __FACTORY_SET_H_INCLUDED__
#define __FACTORY_SET_H_INCLUDED__

#include "fplink_export.h"
#include "TargetConnection.h"
#include <QStringList>
#include <string>
#include <vector>

class FlashProgram;
class FlashInfo;
class ChipInfo;

class FPLINK_API FactorySet
{
public:
    FactorySet() {}
    virtual ~FactorySet() {}

    // Load factory data from device.
    // CH_STATUS_INVALID_FACT: no valid factory data on device
    virtual int loadFromTarget(FlashProgram* fpc) = 0;

    // Store factory data to device.
    virtual int storeToTarget(FlashProgram* fpc) = 0;

    // Reset the whole factory information (not flush to device).
    virtual void resetFactoryInfo() = 0;

    // initialize  the factory information (not flush to device).
    virtual void initFactoryInfo() = 0;

    // Check station definition (with the global station setting)
    // Errors: CH_STATUS_INVALID_FACT
    virtual bool checkStationDefinition() = 0;

    // Check station flag
    // Errors: CH_STATUS_UNKNOWN_STATION
    virtual int checkStation(const std::string& name, bool& performed, bool& passed) = 0;

    // Set station performed to true, and passed to parameter
    // Errors: CH_STATUS_UNKNOWN_STATION
    virtual int setStationFlag(const std::string& name, bool passed) = 0;

    // Clear station flag
    // Errors: CH_STATUS_UNKNOWN_STATION
    virtual int clearStationFlag(const std::string &name) = 0;

    // Get IMEI is 15 digits ('0' ~ '9') string. When "n" is invalid, return empty string
    // The returned string checksum isn't checked.
    virtual std::string getIMEI(int n) = 0;
    // Set IMEI can be 15 digits (with checksum), or 14 digits (without checksum)
    virtual int setIMEI(int n, const std::string& imei,bool ckValid=true) = 0;

    // When checksum exists, it will be checked
    static bool IsIMEIValid(const std::string& imei);
    // Add the checksum to 14 bits. When input is valid, return empty string
    static std::string AddIMEIChecksum(const std::string& imei);

    virtual std::string getMBSN() = 0;
    virtual int setMBSN(const std::string& mbsn) = 0;

    virtual std::string getPSN() = 0;
    virtual int setPSN(const std::string& psn) = 0;

    // BT address is 12 hexdigital
    virtual bool isBtAddressSupported() = 0;
    virtual std::string getBtAddress() = 0;
    virtual int setBtAddress(const std::string& mac) = 0;
    virtual bool getStationNames( std::vector<std::string>  &stations ) = 0;
    virtual bool resetCustomInfo() {
        return false;
    }
};

FPLINK_CAPI FactorySet* CreateFactorySet(const FlashInfo& flashInfo, TargetConnection* conn, ChipInfo* chip);

#endif
