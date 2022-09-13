/*============================================================================*/
/* Card_StreamColList.hpp -- Stream Column                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_AggElist;

  class Card_StreamColList : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamColList(void);
    Card_StreamColList(StreamDef* pStreamDef);
    Card_StreamColList(Card_AggElist* pCard_AggElist);
   ~Card_StreamColList(void) noexcept(false);

    void vPopulate(StreamDef* pStreamDef);
    void vPopulate(Card_AggElist* pCard_AggElist);
  };

  Card_StreamColList* _pCard_StreamColList(void* aAddr);
  bool _bIsCard_StreamColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
