/*============================================================================*/
/* Card_Filter.hpp -- Filter Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Filter : public Card_Unit
  {
  protected:
  
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(eCK eKind);
   ~Card_Filter(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
