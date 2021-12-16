/*============================================================================*/
/* Card_Pred.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Pred : public Card
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
  
    Card_Pred(eCK eKind, eCN eClass, Stz* m_sCardNm);
   ~Card_Pred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Pred* _pCard_Pred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
