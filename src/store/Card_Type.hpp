/*============================================================================*/
/* Card_Type.hpp -- Type                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Part.hpp"
#include "eCT.h"


namespace STORE
{
  class Card_Type : public Card_Part
  {  
  protected:

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Type(eCK eKind, Stz* szType);
   ~Card_Type(void) noexcept(false);
        
  private:
    static void vDeleteThis_(Card* pCard);
  
    Stz* szType(void);

    Footer* aFooter(void);
  };

  Card_Type* _pCard_Type(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
