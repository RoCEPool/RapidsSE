/*============================================================================*/
/* Card_Part.hpp -- Part                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Part : public Card
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
  
    Card_Part(eCK eKind, eCN eClass, Stz* m_sCardNm);
   ~Card_Part(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Part* _pCard_Part(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
