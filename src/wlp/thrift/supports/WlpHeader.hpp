/*============================================================================*/
/* WlpHeader.hpp - A header that is sent before every wireline message        */
/*============================================================================*/

#pragma once

namespace WLP
{
  union WlpHeader {
    struct {
      i8 m_iMsgSize : 32;
      i8 m_iMsgType : 8;
      i8 m_iSpare   : 24;
    };

    i8 m_iAsI8;
  };

  static const i8 C_WLPHEADER_SIZE = 8;
}