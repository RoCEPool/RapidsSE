/*============================================================================*/
/* Card_ByPred.hpp -- By Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_ByPred : public Card_Pred
  {  
  protected:
    
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ByPred(void);
   ~Card_ByPred(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_ByPred* _pCard_ByPred(void* aAddr);
  bool _bIsCard_ByPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
