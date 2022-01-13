/*============================================================================*/
/* Card_CorrSubquery.hpp -- Correlated Subquery                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_CorrSubquery : public Card
  {  
  protected:

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CorrSubquery(eCK eKind, Stz* szQueryNm);
   ~Card_CorrSubquery(void) noexcept(false);
    
    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_CorrSubquery* _pCard_CorrSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
