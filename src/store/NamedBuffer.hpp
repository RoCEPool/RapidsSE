/*============================================================================*/
/* NamedBuffer.hpp -- Named Buffer                                            */
/*============================================================================*/
#pragma once

#include "RowBuffer.hpp"

namespace STORE
{
  class NamedBuffer : public RowBuffer
  {
  protected:
    
    // must be last
    Footer m_xFooter;

  public:
  
    NamedBuffer(eCN eClassNo);
    NamedBuffer(eCN eClassNo, StreamDef* pStreamDef);
   ~NamedBuffer(void) noexcept(false);
    
  //private:
    i8   iToOffset(Stz* aItem);
  
    Footer* aFooter(void);
  };

  NamedBuffer* _pNamedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
