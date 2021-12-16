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
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrPred(eCK eKind);
   ~Card_OrPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_OrPred* _pCard_OrPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
