#ifndef DATAPARSER4FIRECTRL_H
#define DATAPARSER4FIRECTRL_H
#include "dataparserbase.h"

#define FIRECTRL_BUF_SIZE 1024

class DataParser4FireCtrl : public CDataParserBase
{
public:
    DataParser4FireCtrl();
    ~DataParser4FireCtrl();

public:
    quint8 nBufIndex;
    quint16 frameLen;
    //save data sychronized
    QByteArray dataBuffer;
    bool ParseDownData(QByteArray& bufData);
    bool CheckFrameFireCtrl(QByteArray& bufData);
    void ParseFrameFireCtrl(QByteArray& bufData);
};

#endif // DATAPARSER4FIRECTRL_H
