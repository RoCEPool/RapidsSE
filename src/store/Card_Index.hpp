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
    void vX_tblCol_To_expCol(CardIter* pCI);
    bool bHasEquivalentBound(Card_RlnConj* p_rlnConj);
    bool bUsesSqlRsl(void);
    void vSet_Choose_index(void);
    bool bTblCol_IsKeyIn_index(Stz* szTblCol);

    Index*    pGetIndex(Card_TableNm* p_tableNm);

    void vEnsureTypeId_expCol(CardIter *pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Index* _pCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
