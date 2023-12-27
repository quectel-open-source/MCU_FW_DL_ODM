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

#ifndef __TARGET_CONNECTION_H_INCLUDED__
#define __TARGET_CONNECTION_H_INCLUDED__

#include "fplink_export.h"
#include "libusb-1.0\msvc\stdint.h"
#include <memory>
#include <QObject>
#include <list>
#include <vector>

#define RETURN_IF_FAIL(a)   do { int _result = (a); if (_result < 0) return _result; } while (0)
#define UNKNOWN_VALUE       (0xFFFFFFFF)

#define TIMEOUT_DEFAULT     0xfffffff0U
#define TIMEOUT_CHECK(a, d) ((a) == TIMEOUT_DEFAULT? (d) : (a))

typedef uint32_t target_address_t;

enum {
    CH_FLOWCTRL_NONE,
    CH_FLOWCTRL_XONXOFF,
    CH_FLOWCTRL_HW
};

enum {
    CH_USE_DEBUG_HOST   = 0x01,
    CH_USE_USB_MONITOR  = 0x02,
    CH_USE_LIBUSB0      = 0x04,
    CH_USE_LIBUSB1      = 0x08,
    CH_USE_SOCKET       = 0x10
};

enum {
    CH_STATUS_FAILED = -1000,
    CH_STATUS_TIMEOUT,
    CH_STATUS_AGAIN,
    CH_STATUS_NO_DEV,
    CH_STATUS_IO_ERR,
    CH_STATUS_RID_MISMATCH,
    CH_STATUS_PROG_ERROR,
    CH_STATUS_BOOT_MAGIC_MISMATCH,
    CH_STATUS_NOT_SUPPORT,
    CH_STATUS_POINTER_NULL,
    CH_STATUS_OPEN_FILE_FAILED,
    CH_STATUS_PARAM_INVALID,
    CH_STATUS_NOT_ALIGNED,
    CH_STATUS_CRC_ERROR,
    CH_STATUS_FLAG_MISSING,
    CH_STATUS_CANCELED,
    CH_STATUS_UNSUPPORTED_CHIP,
    CH_STATUS_INVALID_FACT,
    CH_STATUS_UNKNOWN_STATION,
    CH_STATUS_INVALID_MMI,
    CH_STATUS_SUCCESS = 0
};

FPLINK_API const char* TargetStrError(int status);

class FPLINK_API TargetConnection
{
public:
    TargetConnection() {}
    virtual ~TargetConnection() {}

    virtual uint32_t connectionMode() = 0;
    virtual void comBreak() = 0;
    virtual int reopen(int disappearTimeout = TIMEOUT_DEFAULT, int reappearTimeout = TIMEOUT_DEFAULT) = 0;

    virtual int readInternal8(uint32_t targetAddress, uint8_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int read8(uint32_t targetAddress, uint8_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int read16(uint32_t targetAddress, uint16_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int read32(uint32_t targetAddress, uint32_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int readBlock(uint32_t targetAddress, void* data, size_t size, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;

    virtual int writeInternal8(uint32_t targetAddress, uint8_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int write8(uint32_t targetAddress, uint8_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int write16(uint32_t targetAddress, uint16_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int write32(uint32_t targetAddress, uint32_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int writeBlock(uint32_t targetAddress, void* data, size_t size, uint32_t timeoutMs = TIMEOUT_DEFAULT) = 0;
    virtual int writeData(uint32_t targetAddress,void *data, size_t size,uint8_t opt,uint8_t flowid, uint32_t timeoutMs= TIMEOUT_DEFAULT) {return CH_STATUS_IO_ERR;}

    virtual void clearEvents() = 0;
    virtual int waitAnyEvent(const std::list<uint32_t>& selection, uint32_t& event, uint32_t timeoutMs) = 0;
    virtual int waitOneEvent(uint32_t event, uint32_t timeoutMs) = 0;

    template<typename T1, typename T2>
    int readTarget(T1* target, T2& val, uint32_t timeoutMs = TIMEOUT_DEFAULT)
    {
        static_assert(sizeof(T1) == sizeof(T2), "ReadTarget size not match");
        static_assert(sizeof(T1) <= 4, "ReadTarget unknown size");

        switch (sizeof(T1)) {
        case 1:
            return read8(uint32_t(target), (uint8_t&)val, timeoutMs);
        case 2:
            return read16(uint32_t(target), (uint16_t&)val, timeoutMs);
        case 4:
        default:
            return read32(uint32_t(target), (uint32_t&)val, timeoutMs);
        }
    }

    template<typename T1, typename T2>
    int writeTarget(T1* target, T2 val, uint32_t timeoutMs = TIMEOUT_DEFAULT)
    {
        static_assert(sizeof(T1) == sizeof(T2), "WriteTarget size not match");
        static_assert(sizeof(T1) <= 4, "WriteTarget unknown size");

        switch (sizeof(T1)) {
        case 1:
            return write8(uint32_t(target), val, timeoutMs);
        case 2:
            return write16(uint32_t(target), val, timeoutMs);
        case 4:
        default:
            return write32(uint32_t(target), val, timeoutMs);
        }
    }
};

class FPLINK_API TargetConnectionWrapper: public TargetConnection
{
public:
    TargetConnectionWrapper();
    ~TargetConnectionWrapper();

    bool openDevice(char * deviceName);

    uint32_t connectionMode();
    void comBreak();
    int reopen(int disappearTimeout = TIMEOUT_DEFAULT, int reappearTimeout = TIMEOUT_DEFAULT);

    int readInternal8(uint32_t targetAddress, uint8_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int read8(uint32_t targetAddress, uint8_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int read16(uint32_t targetAddress, uint16_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int read32(uint32_t targetAddress, uint32_t& val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int readBlock(uint32_t targetAddress, void* data, size_t size, uint32_t timeoutMs = TIMEOUT_DEFAULT);

    int writeInternal8(uint32_t targetAddress, uint8_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int write8(uint32_t targetAddress, uint8_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int write16(uint32_t targetAddress, uint16_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int write32(uint32_t targetAddress, uint32_t val, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int writeBlock(uint32_t targetAddress, void* data, size_t size, uint32_t timeoutMs = TIMEOUT_DEFAULT);
    int writeData(uint32_t targetAddress,void *data, size_t size,uint8_t opt,uint8_t flowid, uint32_t timeoutMs= TIMEOUT_DEFAULT);

    void clearEvents();
    int waitAnyEvent(const std::list<uint32_t>& selection, uint32_t& event, uint32_t timeoutMs);
    int waitOneEvent(uint32_t event, uint32_t timeoutMs);

private:
    TargetConnection* mImpl;
};

FPLINK_API TargetConnection* CreateTargetConnection(char *deviceName);

// Scan serial devices (which may not be DebugHost device)
FPLINK_API std::list<std::string> SerialDeviceScan();
FPLINK_API std::list<std::string> SerialDeviceScan(const std::list<std::string>& exclusive);

// Scan DebugHost devices. There will be a transaction to verify
// whether it is DebugHost Device, and filter the desired chip type.
//
// NOTE: when there are serial port on PC which is sensitive to
// data received, don't call this function.
FPLINK_API std::list<std::string> DebugHostDeviceScan();
FPLINK_API std::list<std::string> DebugHostDeviceScan(const std::list<std::string>& exclusive);

// Scan for all known devices
FPLINK_API std::list<std::string> UsbDeviceScan();

FPLINK_API std::list<std::string> CoolhostScan(const std::string& host = "127.0.0.1", uint16_t portStart = 7701, uint16_t = 7732);
FPLINK_API QStringList QSerialPortDeviceScan(const QStringList& allPorts,const QStringList& exclusive,qint32 baudRate=921600,bool portLimit=true,bool upperAtcmd = true);

#endif
