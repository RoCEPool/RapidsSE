/*============================================================================*/
/* Card_Mapping.hpp -- Mapping                                                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Mapping : public Card_symbol<Card_Mapping>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Mapping(void);
   ~Card_Mapping(void) noexcept(false);

    void vToColMap(ColMap* pColMap);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Mapping* _pCard_Mapping(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
