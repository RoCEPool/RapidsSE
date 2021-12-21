/*============================================================================*/
/* Card_OrdAgg.hpp -- Ordered Aggregate                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Project.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_OrdAgg : public Card_Project
  {  
  protected:
    eAL m_eAggLevel;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAgg(eCK eKind);
   ~Card_OrdAgg(void) noexcept(false);

    eAL  eAggLevel(void);
    void vAggLevel(eAL eAggLevel);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_OrdAgg* _pCard_OrdAgg(void* aAddr);
  bool _bIsCard_OrdAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
