/*============================================================================*/
/* Card_As.hpp -- As                                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_As : public Card
  {  
  protected:
    s32 m_sAsNm;

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_As(Stz* szAsNm);
   ~Card_As(void) noexcept(false);
    
    Stz* szAsNm(void);
    void vAsNm(Stz* szAsNm);

    void vPrintAs(Ribbon* pRibbon);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_As* _pCard_As(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
