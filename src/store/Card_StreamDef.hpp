/*============================================================================*/
/* Card_StreamDef.hpp -- Stream Definition Card                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "StreamDef.hpp"

namespace STORE
{
  class Card_StreamDef : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamDef(StreamDef* pStreamDef);
   ~Card_StreamDef(void) noexcept(false);
  };

  Card_StreamDef* _pCard_StreamDef(void* aAddr);
  bool _bIsCard_StreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
