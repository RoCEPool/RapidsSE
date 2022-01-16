/*============================================================================*/
/* Card_Having.hpp -- Having Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Having : public Card_Unit
  {
  protected:
  
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Having(eCK eKind);
   ~Card_Having(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Having* _pCard_Having(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
