/*============================================================================*/
/* Card_ComparePred.hpp -- Compare Predicate                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_ComparePred : public Card_Pred
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ComparePred(eCK eKind);
   ~Card_ComparePred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_ComparePred* _pCard_ComparePred(void* aAddr);
  bool _bIsCard_ComparePred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
