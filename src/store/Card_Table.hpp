/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "Scope.hpp"

#include "Card_OP.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Table : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
   ~Card_Table(void) noexcept(false);

    static Table* pAccess_Table(Stz* szTableNm);

    static void vGetSizes(Table* pTable, i8& jTableSize, i8& jRowSize, i8& nRowCount);

    static void vAffixTableColOrd(CardIter* pCI);
    static void vAffixTableSyn(CardIter* pCI, Symbol* pSymbol_tableSyn);
    static void vAffixTableSyn_passThru(CardIter* pCI, Symbol* pSymbol_tableSyn);
    static void vAppendIndexList(CardIter* pCI);
    static void vDeriveBoundsFromFilter(CardIter* pCI);
    static Card_OP* pFind_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before);
    static void vFixTablePos(CardIter* pCI);
    static i8 nTables(CardIter* pCI);
    static i8 nTables_foldBreak(CardIter* pCI);
    static i8 nTables_reverse_foldBreak(CardIter* pCI);

    void vAddPredToTableFilter(CardIter* pCI, Card_PRED* p_PRED);

    void vBounds_AddPred(Card_Index* p_index,
                           Card_Bounds* p_bounds, Card_PRED *p_PRED);
    Stz* szColNm(i8 i_Col_base0);
    bool bIsTblColOf(Stz* szTblCol);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vDeriveBoundsFrom_filter(CardIter *pCI);
    bool bUsesSqyRsl(void);
    void vMorph_table_To_EEop(CardIter *pCI);
    bool bScopeContains_Pred(Card_PRED* p_pred, Scope* pScope);
    bool bScopeContains_term(Card_Term* p_term, Scope* pScope);
    void vSet_Syn(CardIter *pCI, Symbol* pSymbol_tableSyn);
    void vSmooth(CardIter* pCI);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
