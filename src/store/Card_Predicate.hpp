/*============================================================================*/
/* Card_Predicate.hpp -- Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Predicate : public Card
  {  
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Predicate(eCK eKind);
   ~Card_Predicate(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_Predicate* _pCard_Predicate(void* aAddr);
  bool _bIsCard_Predicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
