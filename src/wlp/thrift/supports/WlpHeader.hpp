/*============================================================================*/
/* WlpHeader.hpp - A header that is sent before every wireline message        */
/*============================================================================*/

#pragma once
//#include "BASE.h"

namespace wlp
{
  union WlpHeader {
    struct {
      i8 m_iMsgSize : 32;
      i8 m_iMsgType : 8;
      i8 m_iSpare   : 24;
    };

    i8 m_iAsI8;
  };

  const i8 C_WLPHEADER_SIZE = 8;
}