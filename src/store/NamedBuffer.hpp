/*============================================================================*/
/* NamedBuffer.hpp -- Named Buffer                                            */
/*============================================================================*/
#pragma once

#include "RowBuffer.hpp"

namespace STORE
{
  class NamedBuffer : public RowBuffer
  {
  public:
    //NamedBuffer(void);
    NamedBuffer(StreamDef* pStreamDef, i8 iLogCtl=C_LOG);
   ~NamedBuffer(void) noexcept(false);
  };

  NamedBuffer* _pNamedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
