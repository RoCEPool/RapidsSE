/*============================================================================*/
/* Card_ProjectPred.hpp -- Project Predicate                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ProjectPred : public Card
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ProjectPred(void);
   ~Card_ProjectPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_ProjectPred* _pCard_ProjectPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
