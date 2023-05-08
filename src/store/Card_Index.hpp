/*============================================================================*/
/* Card_Index.hpp -- Index                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Index : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(void);
   ~Card_Index(void) noexcept(false);

    static void vCullNonusedBounds(CardIter* pCI);
    static Card_Index* pDerive(Index* pIndex);

    void vAdd_key_exprs(CardIter* pCI);

    void vDeleteBounds(void);
    void vSet_Choose_index(void);
    bool bTblColOdn_IsKeyIn_index(Stz* szTblColOdn);
   };

  Card_Index* _pCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
