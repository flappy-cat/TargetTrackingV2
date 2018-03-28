#pragma once
#include<QByteArray>
#include <vector>
using namespace std;

class CDataParserBase
{
public:
    CDataParserBase();
    virtual ~CDataParserBase();

    QByteArray m_bufData;
    QByteArray m_BufSychronized;

    void SetReceiveBufferSize( quint32 size); //she zhi xu yao jie xi shu ju de daxiao
    quint32 GetReceiveBufferSize() const; //huo de shuju de daxiao
    quint32 m_unBufDataSize;
    quint32 m_unSychronizedBytes; //tiao zhen guocheng shu liang
    virtual bool  ParseDownData(QByteArray& bufData) ; //
    virtual bool  ParseUpData(QByteArray& bufData) ; //
    quint32 m_unDataSize; //
    quint32 m_resBytes;
};
