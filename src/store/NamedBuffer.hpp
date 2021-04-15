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
    NamedBuffer(StreamDef* pStreamDef);
   ~NamedBuffer(void) noexcept(false);
  };

  NamedBuffer* _pNamedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
