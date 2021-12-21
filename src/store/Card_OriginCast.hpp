/*============================================================================*/
/* Card_OriginCast.hpp -- Origin Cast                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_OriginCast : public Card
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OriginCast(eCK eKind);
   ~Card_OriginCast(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_OriginCast* _pCard_OriginCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
