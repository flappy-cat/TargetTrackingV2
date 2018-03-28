#ifndef DATAPARSER4PAYLOAD_H
#define DATAPARSER4PAYLOAD_H
#include "dataparserbase.h"

#define PAYLOAD_BUF_SIZE 50

class DataParser4Payload : public CDataParserBase
{
public:
    DataParser4Payload();
    ~DataParser4Payload();
public:
    quint8 nBufIndex;
    quint16 frameLen;
    //save data sychronized
    QByteArray dataBuffer;
    bool ParseDownData(QByteArray& bufData);
    bool CheckFramePayload(QByteArray& bufData);
    void ParseFramePayload(QByteArray& bufData);
};

#endif // DATAPARSER4PAYLOAD_H
