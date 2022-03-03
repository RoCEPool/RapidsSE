/*============================================================================*/
/* Card_OrderElist.hpp -- List of Order Expressions                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_OrderElist : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrderElist(void);
    Card_OrderElist(StreamDef* pStreamDef);
   ~Card_OrderElist(void) noexcept(false);
    
    bool bSimpleDef(void);
    
  private:
    void vPopulate(StreamDef* pStreamDef);
  };

  Card_OrderElist* _pCard_OrderElist(void* aAddr);
  bool _bIsCard_OrderElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
