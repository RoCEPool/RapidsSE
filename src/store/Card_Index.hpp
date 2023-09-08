/*============================================================================*/
/* Card_Index.hpp -- Index                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "IndexDef.hpp"

namespace STORE
{
  class Card_Index : public Card_symbol<Card_Index>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(void);
   ~Card_Index(void) noexcept(false);

    static i8 nCullNonusedBounds(CardIter* pCI);
    static Card_Index* pDerive(Index* pIndex);

    void vAdd_streamKey_exprs(CardIter* pCI);

    void vDeleteBounds(void);
    void vConvert_colOdn_To_stmOdn(CardIter* pCI);
    bool bHasEquivalentBound(Card_CmpPred* p_cmpPred);
    bool bUsesSqlRsl(void);
    void vSet_Choose_index(void);
    bool bTblColOdn_IsKeyIn_index(Stz* szTblColOdn);

    Index*    pGetIndex(Card_TableNm* p_tableNm);

    void vEnsureTypes_For_stmOdn(CardIter *pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Index* _pCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
