/*============================================================================*/
/* Card_Properties.hpp -- Set of Proprties                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Property.hpp"

namespace STORE { class Card_Atom; }

namespace STORE
{
  class Card_Properties : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Properties(eCK eKind);
   ~Card_Properties(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_Properties* _pCard_Properties(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
