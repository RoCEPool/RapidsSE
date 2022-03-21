/*============================================================================*/
/* Card_ObyElist.hpp -- List of Order-By Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_ObyElist : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ObyElist(void);
    Card_ObyElist(Index* pIndex);
   ~Card_ObyElist(void) noexcept(false);
    
    
  private:
    void vPopulate(Index* pIndex);
  };

  Card_ObyElist* _pCard_ObyElist(void* aAddr);
  bool _bIsCard_ObyElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
