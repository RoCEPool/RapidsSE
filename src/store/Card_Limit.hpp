/*============================================================================*/
/* Card_Limit.hpp -- Limit Card                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Limit : public Card_Unit
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Limit(eCK eKind, Stz* szRows);
   ~Card_Limit(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_Limit* _pCard_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
