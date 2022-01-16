/*============================================================================*/
/* Card_Exists.hpp -- Exists Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Exists : public Card
  {
  protected:
  
  private:


  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Exists(eCK eKind);
   ~Card_Exists(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Exists* _pCard_Exists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
