/*============================================================================*/
/* Card_NoncSubquery.hpp -- Non-Correlated Subquery                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_NoncSubquery : public Card
  {  
  protected:
    s32 m_sQueryNm;

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NoncSubquery(eCK eKind);
   ~Card_NoncSubquery(void) noexcept(false);
    
    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_NoncSubquery* _pCard_NoncSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
