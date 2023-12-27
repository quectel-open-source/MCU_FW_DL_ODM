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

#ifndef __UARTCONNECTION_H_INCLUDED__
#define __UARTCONNECTION_H_INCLUDED__

#include <QtSerialPort/QSerialPort>
#include <QList>
#include <memory>
#include "fplink_export.h"
//#include "TargetConnection.h"
//#include "FactorySet.h"
#include <string>
#include <stdint.h>

#include <QStringList>

struct DownloadData{
    QByteArray mainBuf;
    qint32 mainBufCrc;
};

class FPLINK_API UartConnection
{

public:
    explicit UartConnection(std::shared_ptr<QSerialPort> device);
    ~UartConnection();

    qint32 defaultTimeout();
    qint32 maxWriteBlockSize();
    qint32 maxReadBlockSize();
    qint64 write(const char *data, qint64 len);
    qint64 write(const char *data);
    qint64 write(const QByteArray &data);

    QByteArray read(qint64 maxlen);
    QByteArray readAll();

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);

    int sendCMD(const QByteArray &cmd,const QByteArray &okMsg,QByteArray &readData,QByteArray &log,int waittime=200);
    int readBlock(uint32_t address, uint32_t size, QByteArray &rdData,uint32_t waittime);
    int writeBlock(uint32_t address,uint8_t *data,uint32_t size, QByteArray &log);
    int erase(quint32 address,quint32 size,uint32_t waittime);
    int getWriteMaxLen();
    int readFact();
    int writeFact(uint32_t addr,uint32_t size,uint8_t *fact);
    void closePort();
    void stop() { mProgramStopReq = true; }

Q_SIGNALS:
    void programProgress(int32_t finished);

 private:
    std::shared_ptr<QSerialPort> mDevice;

    bool mProgramStopReq;
    bool mStopWrite;
    uint32_t mWriteMaxSize;
};

FPLINK_API UartConnection* CreateSerialPortEngine(const QString& deviceName, qint32 baud);

#endif
