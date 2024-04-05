/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once


#include "Card_symbol.hpp"
#include "Scope.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Table : public Card_symbol<Card_Table>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
   ~Card_Table(void) noexcept(false);

    static Table* pAccess_Table(Stz* szTableNm);

    static void vGetSizes(Table* pTable, i8& jTableSize, i8& jRowSize, i8& nRowCount);

    static void vAffixTableColOrd(CardIter* pCI);
    static void vAffixTableSyn_passThru(CardIter* pCI, Symbol* pSymbol_tableSyn);
    static void vAppendIndexList(CardIter* pCI);
    static void vDeriveBoundsFromFilter(CardIter* pCI);
    static void vFixTablePos(CardIter* pCI);
    static i8 nTables(CardIter* pCI);
    static i8 nTables_reverse_foldBreak(CardIter* pCI);

    void vBounds_AddConj(Card_Index* p_index,
                         Card_Bounds* p_bounds, Card_CONJ *p_PRED);
    Stz* szColNm(i8 i_Col_base0);
    Card_Index* pFindChosenIndex(void);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vDerive_bounds_From_filter(CardIter *pCI);
    bool bUsesSqyRsl(void);
    void vMorph_table_To_EEop(CardIter *pCI);
    bool bScopeContains_Conj(Card_CONJ* p_pred, Scope* pScope);
    bool bScopeContains_term(Card_Term* p_term, Scope* pScope);
    bool bSame(Symbol* pSymbol_tableSyn);
    void vSet_Syn(Symbol* pSymbol_tableSyn);
   Table* pGet_Table(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
