/*============================================================================*/
/* Card_StreamDef.hpp -- Stream Definition                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OBJECT.hpp"

namespace STORE
{
  class Card_StreamDef : public Card_OBJECT
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamDef(StreamDef* pStreamDef);
   ~Card_StreamDef(void) noexcept(false);
    
    void vApplyOrder(StreamDef* pStreamDef_apply);
  };

  Card_StreamDef* _pCard_StreamDef(void* aAddr);
  bool _bIsCard_StreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
