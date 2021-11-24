/*============================================================================*/
/* Card_Unit.hpp -- Abstract Unit                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Unit : public Card
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
  
    Card_Unit(eCN eClass, Stz* m_sCardNm);
   ~Card_Unit(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Unit* _pCard_Unit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
