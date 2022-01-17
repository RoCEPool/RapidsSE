/*============================================================================*/
/* Card_IndexPred.hpp -- Index Predicate                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_IndexPred : public Card_Pred
  {  
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexPred(eCK eKind);
   ~Card_IndexPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_IndexPred* _pCard_IndexPred(void* aAddr);
  bool _bIsCard_IndexPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
