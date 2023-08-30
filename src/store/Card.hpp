/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "ePB.h"
#include "eGC.h"
#include "eQO.h"
#include "eRO.h"
#include "eTE.h"
#include "eYG.h"
#include "eXK.h"

#include "ChildCards.hpp"

#include "Ribbon.hpp"

#include "Mark.hpp"
#include "Symbol.hpp"

namespace STORE
{
  class Card_AbsJoin;
  class Card_Aggreg;
  class Card_AndPred;
  class Card_As;

  class Card_BoolCalc;
  class Card_Bound;
  class Card_Bounds;

  class Card_CmpPred;
  class Card_Combined;
  class Card_Constraint;
  class Card_Correlation;
  class Card_JoinEstimate;
  class Card_CostJoining;
  class Card_CostNetwork;
  class Card_CostStream;
  class Card_CostUnicast;
  class Card_Count;
  class Card_CrossJoin;

  class Card_StreamHead;
  class Card_Debug;
  class Card_Deck;
  class Card_Direction;

  class Card_ELIST;
  class Card_Expr;

  class Card_Filter;
  class Card_FiltPred;
  class Card_FiltPredList;
  class Card_Flat;
  class Card_Fold;
  class Card_FoldElist;
  class Card_From;

  class Card_GbyAgg;

  class Card_Hash;
  class Card_Hashing;
  class Card_Having;
  class Card_HshCol;

  class Card_IdxCol;
  class Card_IdxColList;
  class Card_Index;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexNm;
  class Card_IndexScan;

  class Card_JoinCast;
  class Card_JoinOrder;
  class Card_JoinOrdering;
  class Card_StreamSet;
  class Card_StreamPer;
  class Card_JoinPredList;

  class Card_Key;
  class Card_KeyCells;
  class Card_KeyExists;
  class Card_KeyNotExists;

  class Card_Level;
  class Card_Limit;

  class Card_Map;
  class Card_Mapping;

  class Card_NCorrCols;
  class Card_Needed;
  class Card_Cast;
  class Card_Note;
  class Card_NotPred;

  class Card_On;
  class Card_OP;
  class Card_OrdAgg;
  class Card_OrPred;
  class Card_OuterColNm;
  class Card_OuterJoin;

  class Card_PayloadCells;
  class Card_PRED;
  class Card_Project;
  class Card_Projection;

  class Card_RowCount;
  class Card_RowSize;
  class Card_RpnCalc;
  class Card_RpnTerms;

  class Card_Scan;
  class Card_Section;
  class Card_Send;
  class Card_Sort;
  class Card_Spare;
  class Card_Stage;
  class Card_StreamJoin;
  class Card_StreamKey;
  class Card_StreamPtn;
  class Card_SubSet;

  class Card_Table;
  class Card_TableList;
  class Card_TableNm;
  class Card_TablePtn;
  class Card_TableSize;
  class Card_TblSyn;
  class Card_Term;
  class Card_Top;
  class Card_Topology;

  class Card_Unique;

  class Card_Where;

  class CardIter;
}

// These would really benefit from being const and being in a namespace
// or class to avoid polluting the global namespace.
extern Stz* P_AggOp;
extern Stz* P_As;
extern Stz* P_BreakPt;
extern Stz* P_BumpedBy;
extern Stz* P_Cell;
extern Stz* P_ChosenBound;
extern Stz* P_ChosenIndex;
extern Stz* P_Column;
extern Stz* P_Count;
extern Stz* P_CrlKeyExpr;
extern Stz* P_CrlKeyPred;
extern Stz* P_CrlKeyTest;
extern Stz* P_CrlRslTest;
extern Stz* P_DebugOption;
extern Stz* P_DisamCol;
extern Stz* P_Early;
extern Stz* P_EmittedCol;
extern Stz* P_FixedPos;
extern Stz* P_HashOf;
extern Stz* P_HideHeat;
extern Stz* P_Injected;
extern Stz* P_InjCrlKey;
extern Stz* P_Invalid;
extern Stz* P_JoinedBuff;
extern Stz* P_Key;
extern Stz* P_Level;
extern Stz* P_Network;
extern Stz* P_Payload;
extern Stz* P_ParamOdn;
extern Stz* P_Permutation;
extern Stz* P_Pointer;
extern Stz* P_ProjBump;
extern Stz* P_ProjBumped;
extern Stz* P_PtndJoin;
extern Stz* P_ReqCol;
extern Stz* P_SendKind;
extern Stz* P_SentOut;
extern Stz* P_Smoothed;
extern Stz* P_StageTopo;
extern Stz* P_StmCol;
extern Stz* P_SubQryCall;
extern Stz* P_Synthetic;
extern Stz* P_TableUsed;
extern Stz* P_TblCol;
extern Stz* P_Type;
extern Stz* P_Unproject;
extern Stz* P_WasKeyCol;
extern Stz* P_WasStmCol;

extern Stz* C_GLEAN_CUE_FROM;
extern Stz* C_OPTIMISER_TRACE;
extern Stz* C_GLEAN_TRACE;
extern Stz* C_GLEAN_XFORM;
extern Stz* C_GLEAN_XHAND;
extern Stz* C_NA;

extern Stz* C_CARD_HISTORY;
extern Stz* C_CODE_BLOCK;
extern Stz* C_EP_DISABLE;
extern Stz* C_EP_ENABLE;
extern Stz* C_FILTERPRED;
extern Stz* C_JOINORDER;
extern Stz* C_JOINPERMUTATIONS;
extern Stz* C_JO_DISABLE;
extern Stz* C_JO_PICK;
extern Stz* C_JOWRITE;
extern Stz* C_OPTIMISE_PASSTHRU;
extern Stz* C_PIPELINE_DEF;
extern Stz* C_WEAVED;
extern Stz* C_NOCODESYNTHESIS;

extern d8 C_EXPANSION_COEFF;  // crw -> row buffer
extern d8 C_FILTER_COEFF;
extern d8 C_JOIN_COEFF;  // bump the larger
extern d8 C_JOIN_LONGER;  // joined row gets longer

extern Stz* MARK_AND;
extern Stz* MARK_AT;
extern Stz* MARK_BLANK;
extern Stz* MARK_CHANGED;
extern Stz* MARK_COMPARE;
extern Stz* MARK_DELETED;
extern Stz* MARK_DUPLICATE;
extern Stz* MARK_NOT;
extern Stz* MARK_OR;
extern Stz* MARK_UP;
extern Stz* MARK_UNVALID;
extern Stz* MARK_VALID;

namespace STORE
{
  static const bool C_CONTINUE_RECURSION  = true;
  static const bool C_TERMINATE_RECURSION = false;

  class Card : public Object
             , public ChildCards
  {
  protected:
    static i8 m__iSeqGen;

    char    md_Bearing[256];

    eCK     m_eCard;
    i8      m_i_Child;

    Mark    m_xMark1;
    Mark    m_xMark2;

    Symbol  m_xSymbol;

    Card*   m_pLastPrintParent;

    i8      m_iFixedDepth;

    i8 md__SeqNo;
    i8 md__FirstDetected;
    i8 md__LeakDetected;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard, i8 iFixedDepth);

   ~Card(void) noexcept(false);

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static eYG  e_aggLevel_To_stageTopo(eAL eLevel);

    static eAL  e_stageTopo_To_aggLevel(eYG eStageTopo);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card* pAppendNewCard(eCK eCard);
    bool bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);
    bool bApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn);

    Stz* szBearing(void);

    eCK  eCard(void);
    Card* pCard(eCK eCard);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   i_Child(void);
    void v_Child(i8 i_Child);

    void vColType(Card* pCard_arg);

    void  vDump(const char* azFunc, i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCard(void);
    Card* pDupCardRecursive(void);
    Card_SubSet* pDupSubCardsRecursive(i8 i_From, i8 i_Till=C_UNKNOWN);

    Card* pFindCardRecursive(eCK eOfCard);
    Card* pFindParentRecursive(Card* pSubCard);
    Card* pFindPpyRecursive(Stz* szPpy, eCK eOfCard=CK_NONE);
    Card* pFindMark1Recursive(char cMark1, eCK eOfCard=CK_NONE);
    Card* pFindMark1Recursive(Stz* szMark1, eCK eOfCard=CK_NONE);
    Card* pFindMark2Recursive(char cMark2, eCK eOfCard=CK_NONE);
    Card* pFindMark2Recursive(Stz* szMark2, eCK eOfCard=CK_NONE);
    Card* pFindSymbolRecursive(eCK eOfCard, Symbol* pSymbol, Card*& rpParent, Card* _opaque_=nullptr);
    bool  bFixedDepth(i8 iDepth);
    i8    iFixedDepth(void);
    void  vFixedDepth(i8 iFixedDepth);

    bool bHasSubRecursive(Card* pSubCard);

    bool bIsCard(eCK eCard);
    bool bIsOne(i8 /*eCK*/ iCard, ...);

    bool bMark1(Stz* szArg);
    char cMark1(void);
    Stz* szMark1(void);
    void vMark1(char cMark1);
    void vMark1(Stz* szMark1);
    void vMark1Recursive(char cMark1);
    void vMark1Recursive(Stz* szMark1);
    void vMark1RecursiveIf(char cMark, char cIf);
    void vMark1RecursiveIf(Stz* szMark1, Stz* szIf);

    bool bMark2(Stz* szArg);
    char cMark2(void);
    Stz* szMark2(void);
    void vMark2(char cMark2);
    void vMark2(Stz* szMark2);
    void vMark2Recursive(char cMark2);
    void vMark2Recursive(Stz* szMark2);
    void vMark2RecursiveIf(char cMark, char cIf);
    void vMark2RecursiveIf(Stz* szMark2, Stz* szIf);

    void vMarksClearRecursive(void);

    void vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent,
                             bool& rbPrune, eAL eLevel,
                             bool bPrintBelow, i8 iXform, Stz* szXfunc);
    void vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);

    void vRenumberRoot(i8 i_Parent);
    void vRenumberRecursive(Card* pParent);
    void vReplaceSubCard(Card* pSub, Card* pWith);

    bool bSameSymbol(Symbol* pSymbol);
    bool bSameSymbol(Card* pArg);

    Symbol* pSymbol(void);
    Stz* szSymbol(eSF eSymbol_xchk);

    bool bUpdateSymbolRecursive(eCK eSub, Symbol* pSymbol, Symbol* pWith);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

    void vValidate_mayHave(i8 /*eCK*/ iCard_first, ...);
    eCK  eValidate_mustEither(i8 /*eCK*/ iCard_first, ...);
    void vValidate_mustHave(i8 /*eCK*/ iCard_first, ...);
    void vValidate_notHave(i8 /*eCK*/ iCard, ...);

    void vValidationFailed(Stz* szFormat, ...);
    void vValidationPassed(void);

  private:
  private:
    void vBearing(void);
    void vBearing(Card* pParentCard);

    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon, eAL eLevel=AL_NONE);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameSymbol(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintSymbol(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks=false);

  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  public:

    Card_Aggreg*        pSub_aggreg(void);
    Card_As*            pSub_as(void);
    Card_BoolCalc*      pSub_boolCalc(void);
    Card_Bound*         pSub_bound(void);
    Card_Bounds*        pSub_bounds(void);
    Card_CmpPred*       pSub_cmpPred(void);
    Card_Constraint*    pSub_constraint(void);
    Card_Correlation*   pSub_correlation(void);
    Card_Count*         pSub_count(void);
    Card_CrossJoin*     pSub_crossJoin(void);
    Card_StreamHead*    pSub_dataStart(void);
    Card_Direction*     pSub_direction(void);
    Card_Expr*          pSub_expr(void);
    Card_Filter*        pSub_filter(void);
    Card_FiltPred*      pSub_filtPred(void);
    Card_FiltPredList*  pSub_filtPredList(void);
    Card_Flat*          pSub_flat(void);
    Card_Fold*          pSub_fold(void);
    Card_FoldElist*     pSub_foldElist(void);
    Card_From*          pSub_from(void);
    Card_GbyAgg*        pSub_gbyAgg(void);
    Card_Hashing*       pSub_hashing(void);
    Card_HshCol*        pSub_hshCol(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_JoinCast*      pSub_locusCast(void);
    Card_StreamJoin*    pSub_joinLocus(void);
    Card_StreamSet*     pSub_joinPermAll(void);
    Card_StreamPer*     pSub_joinPermList(void);
    Card_Key*           pSub_key(void);
    Card_KeyCells*      pSub_keyCells(void);
    Card_KeyExists*     pSub_keyExists(void);
    Card_KeyNotExists*  pSub_keyNotExists(void);
    Card_Level*         pSub_level(void);
    Card_JoinEstimate*  pSub_joinEstimate(void);
    Card_CostJoining*   pSub_costJoining(void);
    Card_CostNetwork*   pSub_costNetwork(void);
    Card_CostStream*    pSub_costQuery(void);
    Card_CostUnicast*   pSub_costUnicast(void);
    Card_Map*           pSub_map(void);
    Card_Mapping*       pSub_mapping(void);
    Card_NCorrCols*     pSub_nCorrCols(void);
    Card_Needed*        pSub_needed(void);
    Card_Cast*          pSub_next(void);
    Card_Note*          pSub_note(void);
    Card_NotPred*       pSub_notPred(void);
    Card_On*            pSub_on(void);
    Card_OuterColNm*    pSub_outerColNm(void);
    Card_OuterJoin*     pSub_outerJoin(void);
    Card_PayloadCells*  pSub_payloadCells(void);
    Card_Projection*    pSub_projection(void);
    Card_RowCount*      pSub_rowCount(void);
    Card_RowSize*       pSub_rowSize(void);
    Card_RpnCalc*       pSub_rpnCalc(void);
    Card_Scan*          pSub_scan(void);
    Card_Section*       pSub_section(void);
    Card_Spare*         pSub_spare(void);
    Card_StreamKey*     pSub_streamKey(void);
    Card_StreamPtn*     pSub_streamPtn(void);
    Card_Table*         pSub_table(void);
    Card_TableNm*       pSub_tableNm(void);
    Card_TablePtn*      pSub_tablePtn(void);
    Card_TableSize*     pSub_tableSize(void);
    Card_TblSyn*        pSub_tblSyn(void);
    Card_Term*          pSub_term(void);
    Card_Top*           pSub_top(void);
    Card_Topology*      pSub_topology(void);
    Card_Unique*        pSub_unique(void);
    Card_Where*         pSub_where(void);

  private:
    Stz* szDecodeEnum(Stz* szEnum, i8 iEnum);
  
  protected:
    Footer* aFooter(void);
  };

  Card* _pCard(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/* RASSERT_CARD macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_CARD(pOBJECT,eCARD) \
  \
  if (pOBJECT && CN_CARD != pOBJECT->eGetClassNo()) \
    BASE::vThrowType(FK_TYPE, \
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_CARD); \
  \
  if (pOBJECT && eCARD != ((Card*)pOBJECT)->eCard()) \
    BASE::vThrowType(FK_TYPE,\
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_CARD); \
  \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
#define __pCastFunction_Card(SPACENM,CLASSNM,CN) \
  CLASSNM* SPACENM::_p##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    \
    RASSERT_CARD(pObject,CN); \
    \
    return pObject; \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for deleting card                                                    */
/*----------------------------------------------------------------------------*/
#define DELETE_CARD(__pCard) \
  {                          \
    delete __pCard;          \
    __pCard = nullptr;       \
  }                          \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
