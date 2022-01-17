/*============================================================================*/
/* Card_OrPred.hpp -- Or Predicate                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_OrPred : public Card_Pred
  {  
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrPred(eCK eKind);
   ~Card_OrPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_OrPred* _pCard_OrPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
