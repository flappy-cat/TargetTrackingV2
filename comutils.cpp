#include "comutils.h"

quint16 FuncUtils::CRC_check(QByteArray bufData)
{
    quint32 i,j;
    quint16 reg_CRC = 0xffff;
    if(bufData.isEmpty ())
    {
        return 0;
    }
    quint32 dataLen = bufData.length ();
    for(i = 0;i<dataLen;i++)
    {
        reg_CRC ^= bufData[i];
        for(j=0; j<8; j++)
        {
            if(reg_CRC & 0x01)
            {
                reg_CRC = (reg_CRC >> 1 ^ 0xa001);
            }
            else
            {
                reg_CRC = reg_CRC >> 1;
            }
        }
    }

    return reg_CRC;
}
