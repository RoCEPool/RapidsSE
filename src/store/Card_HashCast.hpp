/*============================================================================*/
/* Card_HashCast.hpp -- Hash Cast                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_HashCast : public Card
  {  
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_HashCast(eCK eKind);
   ~Card_HashCast(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_HashCast* _pCard_HashCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
