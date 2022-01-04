/*============================================================================*/
/* Card_MainSubquery.hpp -- Main Subquery                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_MainSubquery : public Card
  {  
  protected:
    s32 m_sQueryNm;

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MainSubquery(eCK eKind);
   ~Card_MainSubquery(void) noexcept(false);
    
    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_MainSubquery* _pCard_MainSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
