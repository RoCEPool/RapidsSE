/*============================================================================*/
/* WlpHeader.hpp - A header that is sent before every wireline message        */
/*============================================================================*/

#pragma once

namespace WLP
{
  static const i8 C_WLPHEADER_SIZE = 8;

  union WlpHeader {
    struct {
      u4 m_uMsgSize : 32;  // !!! BIG ENDIAN !!! Don't use directly.
      u1 m_uMsgType : 8;   // !!! BIG ENDIAN !!! Don't use directly.
      u4 m_uSpare   : 24;
    };

    i8 m_iAsI8;

    char* pcMsgBody() { return ((char*) &m_iAsI8) + C_WLPHEADER_SIZE; };

    i8 iMsgSize()     {
      i8 iVal = ntohl(m_uMsgSize);
      return iVal; }
    i8 iMsgType()     { return (i8) m_uMsgType; }

    void vMsgSize(i8 iMsgSize)  { m_uMsgSize = htonll(iMsgSize); }
    void vMsgType(i8 iMsgType)  { m_uMsgType = htonll(iMsgType); }
  };

  static_assert(sizeof(WlpHeader) == C_WLPHEADER_SIZE, "Expecting sizeof(WlpHeader) == C_WLPHEADER_SIZE");
}