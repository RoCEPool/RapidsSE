/*============================================================================*/
/* Card_Level.hpp -- Aggregate Level                                       */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Level : public Card_symbol<Card_Constraint>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Level(eAL eLevel);
   ~Card_Level(void) noexcept(false);

    static eYG  e_aggLevel_To_stageTopo(eAL eLevel);
    static eAL  e_stageTopo_To_aggLevel(eYG eStageTopo);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Level* _pCard_Level(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
