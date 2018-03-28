
#include "dataparserbase.h"


CDataParserBase::CDataParserBase()
{
    m_unSychronizedBytes = 0;
    m_resBytes = 0;
    m_unDataSize = 40;
    m_unBufDataSize = 1024;
    m_bufData.clear();
    m_BufSychronized.clear();
}


CDataParserBase::~CDataParserBase()
{
}

void CDataParserBase::SetReceiveBufferSize(quint32 size)
{
    m_unDataSize = size;
}


quint32 CDataParserBase::GetReceiveBufferSize() const
{
    return m_unDataSize;
}

bool CDataParserBase::ParseDownData(QByteArray &bufData)
{
    return true;
}
bool CDataParserBase::ParseUpData(QByteArray &bufData)
{
    return  true;
}
