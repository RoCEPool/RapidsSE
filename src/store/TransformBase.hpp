/*============================================================================*/
/* TransformBase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"


namespace STORE
{
  class Table;

  class TransformBase : public Object
  {
  public:

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TransformBase(eCN eClass);
   ~TransformBase(void) noexcept(false);

    Table* A_pAccess_Table(Stz* szTableNm);

    Card_IndexList* A_pProduce_indexList_ofTable(Table* pTable);
    Card_Size*      A_pProduce_size_ofTable(Table* pTable);
    Card_Stream*    A_pProduce_stream_ofTable(Table* pTable);
    Card_TablePtn*  A_pProduce_tablePtn_ofTable(Table* pTable, char cTableSyn);

    bool T_bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef);
    i8   T_iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef);
    char T_cColumnRef_TableSyn(Card_ColumnRef* p_columnRef);
    char T_cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef);
    void T_vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn);

    ePB  T_eTerm_Bound(Card_Term* p_term);
    i8   T_iTerm_IndexColNo(Card_Term* p_term);
    bool T_bTerm_IsIndexColOrdinal(Card_Term* p_term);
    bool T_bTerm_IsParamOrdinal(Card_Term* p_term);
    bool T_bTerm_IsStreamColOrdinal(Card_Term* p_term);
    bool T_bTerm_IsTableColOrdinal(Card_Term* p_term);
    eRO  T_eTerm_RelOp(Card_Term* p_term);
    i8   T_iTerm_StreamColNo(Card_Term* p_term);
    i8   T_iTerm_TableColNo(Card_Term* p_term);
    char T_cTerm_TableSyn(Card_Term* p_term);
    eTE  T_eTerm_TermEval(Card_Term* p_term);

    void T_vPRED_CullByKids(Card_PRED* p_PRED);
    Card_CmpPred* T_pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex, ePB eBound);
    bool T_bPRED_HasBounds (Card_PRED* p_PRED, Index* pIndex);
    static bool T_bPRED_IsPred(Card* pArgCard);

    Card_CmpPred* T_pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound);
    bool T_bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex);

    Card_CmpPred* T_pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound);
    bool T_bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex);

    void T_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);
    bool T_bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    Card_CmpPred* T_pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound);
    bool T_bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex);
    bool T_bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool T_bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool T_bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
