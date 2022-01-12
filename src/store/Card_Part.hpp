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
    Card_Part(eCK eKind, eCN eClass, Stz* szCardNm,
              Stz* szProperty, i8 iValue);
    Card_Part(eCK eKind, eCN eClass, Stz* szCardNm,
              Stz* szProperty, Object* pValue);
    Card_Part(eCK eKind, eCN eClass, Stz* szCardNm,
              Stz* szProperty, Stz* szValue);
   ~Card_Part(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_Part* _pCard_Part(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
