/*============================================================================*/
/* Card_OrdAggList.hpp -- List of Ordered Aggregate                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_OrdAggList : public Card
  {  
  protected:
    eAL m_eAggLevel;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAggList(void);
   ~Card_OrdAggList(void) noexcept(false);

    eAL  eAggLevel(void);
    void vAggLevel(eAL eAggLevel);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_OrdAggList* _pCard_OrdAggList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
