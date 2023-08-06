/*============================================================================*/
/* Card_Mapping.hpp -- Mapping                                                */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Mapping : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Mapping(void);
   ~Card_Mapping(void) noexcept(false);

    ColMap* pToColMap(ColMap* pColMap,
                      Card_Key* p_key,
                      Card* p_OP);
  };

  Card_Mapping* _pCard_Mapping(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
