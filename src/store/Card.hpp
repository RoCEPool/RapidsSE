/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "eFC.h"
#include "eGC.h"
#include "ePB.h"
#include "eQO.h"
#include "eRC.h"
#include "eRO.h"
#include "eTE.h"
#include "eXK.h"
#include "eYG.h"

#include "ChildCards.hpp"
#include "Mark.hpp"
#include "Ribbon.hpp"
#include "Symbol.hpp"

namespace STORE
{
  class Card_AbsJoin;
  class Card_Address;
  class Card_Aggreg;
  class Card_AndConj;
  class Card_As;

  class Card_BoolPred;
  class Card_Bound;
  class Card_Bounds;

  class Card_Caller;
  class Card_CallOp;
  class Card_Clause;
  class Card_RlnConj;
  class Card_Combined;
  class Card_CONJUNCTION;
  class Card_Constraint;
  class Card_CorrCols;
  class Card_Correlation;
  class Card_CostJoining;
  class Card_CostNetwork;
  class Card_CostStream;
  class Card_CostUnicast;
  class Card_Count;
  class Card_CrossJoin;

  class Card_Debug;
  class Card_Deck;
  class Card_Decision;
  class Card_Direction;

  class Card_Emit;
  class Card_EXPRLIST;
  class Card_Expr;

  class Card_Filter;
  class Card_FiltPred;
  class Card_FiltPredList;
  class Card_Flat;
  class Card_Fold;
  class Card_From;

  class Card_GbyAgg;

  class Card_Hash;
  class Card_Hashing;
  class Card_Having;
  class Card_HshCol;
  class Card_HshColList;

  class Card_IdxCol;
  class Card_IdxColList;
  class Card_Index;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexNm;
  class Card_IndexScan;
  class Card_Inflate;
  class Card_Injectable;

  class Card_JoinCast;
  class Card_JoinOrder;
  class Card_JoinOrdering;
  class Card_JoinEstimate;
  class Card_JoinPredList;

  class Card_Key;
  class Card_KeyCells;
  class Card_KeyExists;
  class Card_KeyNotExists;

  class Card_Level;
  class Card_Limit;

  class Card_Map;
  class Card_Mapping;

  class Card_Disposition;
  class Card_Needed;
  class Card_Cast;
  class Card_Note;
  class Card_NotConj;

  class Card_On;
  class Card_OP;
  class Card_OrdAgg;
  class Card_OrConj;
  class Card_OuterColNm;
  class Card_OuterJoin;

  class Card_Payload;
  class Card_PayloadCells;
  class Card_Project;

  class Card_RelnOp;
  class Card_Required;
  class Card_RowCount;
  class Card_RowSize;
  class Card_RpnCalc;
  class Card_RpnExpr;

  class Card_Scan;
  class Card_Section;
  class Card_Send;
  class Card_Sort;
  class Card_Spare;
  class Card_SqyCalls;
  class Card_SqyInfo;
  class Card_SqyNo;
  class Card_Stage;
  class Card_Stream;
  class Card_StreamHead;
  class Card_StreamJoin;
  class Card_StreamKey;
  class Card_StreamPer;
  class Card_StreamPtn;
  class Card_StreamSet;
  class Card_SubQuery;
  class Card_SubSet;
  class Card_Synthesis;

  class Card_Table;
  class Card_TableInfo;
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

  class Card_XCard;

  class CardIter;
}

// These would really benefit from being const and being in a namespace
// or class to avoid polluting the global namespace.
extern Stz* P_AggOp;
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
extern Stz* P_Early;
extern Stz* P_EmittedCol;
extern Stz* P_FixedPos;
extern Stz* P_FusedProject;
extern Stz* P_HashOf;
extern Stz* P_InjCrlKey;
extern Stz* P_Injected;
extern Stz* P_Invalid;
extern Stz* P_Itemised;
extern Stz* P_Key;
extern Stz* P_KeyCol;
extern Stz* P_Level;
extern Stz* P_ParamOdn;
extern Stz* P_Payload;
extern Stz* P_Permutation;
extern Stz* P_Pointer;
extern Stz* P_ProjBump;
extern Stz* P_ProjBumped;
extern Stz* P_SendKind;
extern Stz* P_Smoothed;
extern Stz* P_StageTopo;
extern Stz* P_SqyCall;
extern Stz* P_Unproject;

extern Stz* C_CARD_HISTORY;
extern Stz* C_CODE_BLOCK;
extern Stz* C_FILTERPRED;
extern Stz* C_GLEAN_CUE_FROM;
extern Stz* C_JOWRITE;
extern Stz* C_JO_DISABLE;
extern Stz* C_JO_PICK;
extern Stz* C_NA;
extern Stz* C_HASCODEFILE;
extern Stz* C_OPTIMISER_EP_DISABLE;
extern Stz* C_OPTIMISER_TRACE;
extern Stz* C_OPTIMISE_PASSTHRU;
extern Stz* C_PIPELINE_DEF;
extern Stz* C_WEAVED;

extern d8 C_EXPANSION_COEFF;  // crw -> row buffer
extern d8 C_FILTER_COEFF;
extern d8 C_JOIN_COEFF;  // bump the larger
extern d8 C_JOIN_LONGER;  // joined row gets longer

extern Stz* MARK_AND;
extern Stz* MARK_AT;
extern Stz* MARK_BLANK;
extern Stz* MARK_CHANGED;
extern Stz* MARK_COMPARE;
extern Stz* MARK_ENUM;
extern Stz* MARK_DELETED;
extern Stz* MARK_DUPLICATE;
extern Stz* MARK_FOUND;
extern Stz* MARK_NOT;
extern Stz* MARK_OR;
extern Stz* MARK_PREDICATE;
extern Stz* MARK_UNVALID;
extern Stz* MARK_UP;
extern Stz* MARK_VALID;

extern Stz* C_SQCRLKEY;

namespace STORE
{
  static const bool C_CONTINUE_RECURSION  = true;
  static const bool C_TERMINATE_RECURSION = false;

  static const bool C_LEFT_ORIENTATION  = true;
  static const bool C_RIGHT_ORIENTATION = false;

  class Card : public Object
             , public ChildCards
  {
  protected:
    static i8 m__iSeqGen;

    char    md_Coord[256];

    eCK     m_eCard;
    i8      m_i_Child;
    i8      m_iFixedDepth;

    Mark    m_xMark1;
    Mark    m_xMark2;

    Symbol  m_xSymbol;

    eFC     m_eConjunction;  // form of conjunction
    eRC     m_eRotationConj; // rotation of conjunction

    i8 md__SeqNo;
    i8 md__FirstDetected;
    i8 md__LeakDetected;
    i8      m_iFirstPrint;
    eCK     m_eFirst;
    i8      m_iLastPrint;
    bool    m_bAddedCorrectly;
    bool    m_bRemovedCorrectly;
    i8      m_iItemise;
    Card*   m_pLastPrintParent;

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

    static bool bIsMain(eCK eCard);

    static eYG  e_aggLevel_To_stageTopo(eAL eLevel);

    static eAL  e_stageTopo_To_aggLevel(eYG eStageTopo);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vAddedCorrectly(bool bAppended=true);
    Card* pAppendNewCard(eCK eCard);
    bool bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);
    bool bApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn);

    eCK  eCard(void);
    Card* pCard(eCK eCard);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    void vCheckCorrectness(void);
    i8   i_Child(void);
    void v_Child(i8 i_Child);
    void vColType(Card* pCard_arg);
    eFC  eConjunction(void);
    void vConjunction(eFC eConjunction);
    Stz* szCoord(void);

    void  vDump(const char* azFunc, Stz* szTitle=nullptr, i8 iDepth=0);
    Card* pDupCard(void);
    Card* pDupCardRecursive(void);
    Card_SubSet* pDupSubCardsRecursive(i8 i_From, i8 i_Till=C_UNKNOWN);

    Stz* szFacet(eSF eFacet_xchk);
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
    bool bIsConj(void);
    bool bIsMain(void);
    bool bIsOne(i8 /*eCK*/ iCard, ...);

    i8   iItemise(void);
    void vItemise(i8 iItemise);

    void vMark(Stz* szMark1, Stz* szMark2);

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
    void vMarksTestRecursive(void);

    void vOnAdd_child(Card* pParent=nullptr);
    void vOnAdd_parent(Card* pChild=nullptr);
    void vOnRemove_child(Card* pParent=nullptr);
    void vOnRemove_parent(Card* pChild=nullptr);

    void vPrintCardRecursive(CardIter* pCI,
                             Ribbon* pRibbon, i8 iDepth, Card* pParent,
                             bool& rbPrune, eAL eLevel,
                             bool bPrintBelow, i8 iXform, Stz* szXfunc);
    void vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);

    void vRemovedCorrectly(bool bRemoved=true);
    void vRenumberRoot(i8 i_Parent);
    void vRenumberRecursive(Card* pParent);
    void vReplaceSubCard(Card* pSub, Card* pWith);
    eRC  eRotationConj(void);
    void vRotationConj(eRC eRotationConj);

    Symbol* pSymbol(void);
    bool bSymbolMatches(Symbol* pSymbol_arg);
    bool bSymbolMatches(Card* pCard_arg);

    bool bUpdateSymbolRecursive(eCK eSub, Symbol* pSymbol, Symbol* pWith);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

    void vValidate_hasChildren(void);
    void vValidate_hasType(void);
    void vValidate_mayHave(i8 /*eCK*/ iCard_first, ...);
    eCK  eValidate_mustEither(i8 /*eCK*/ iCard_first, ...);
    void vValidate_mustHave(i8 /*eCK*/ iCard_first, ...);
    eCK  eValidate_mustSome(i8 /*eCK*/ iCard_first, ...);
    void vValidate_noChildren(void);
    void vValidate_notHave(i8 /*eCK*/ iCard, ...);
    eCK  eValidate_parentEither(CardIter* pCI, i8 /*eCK*/ iCard_first, ...);

    void vValidationFailed(Stz* szFormat, ...);
    void vValidationPassed(void);

  private:
  private:
    void vCoord(void);
    void vCoord(Card* pParentCard);

    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon, eAL eLevel=AL_NONE);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameSymbol(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintSymbol(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks=false);

  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  public:

    Card_Address*       pSub_address(void);
    Card_Aggreg*        pSub_aggreg(void);
    Card_BoolPred*      pSub_boolPred(void);
    Card_Bound*         pSub_bound(void);
    Card_Bounds*        pSub_bounds(void);
    Card_Cast*          pSub_cast(void);
    Card_Caller*        pSub_caller(void);
    Card_CallOp*        pSub_callOp(void);
    Card_Clause*        pSub_clause(void);
    Card_RlnConj*       pSub_cmpConj(void);
    Card_Constraint*    pSub_constraint(void);
    Card_CorrCols*      pSub_corrCols(void);
    Card_Correlation*   pSub_correlation(void);
    Card_CostJoining*   pSub_costJoining(void);
    Card_CostNetwork*   pSub_costNetwork(void);
    Card_CostStream*    pSub_costQuery(void);
    Card_CostUnicast*   pSub_costUnicast(void);
    Card_Count*         pSub_count(void);
    Card_CrossJoin*     pSub_crossJoin(void);
    Card_Decision*      pSub_decision(void);
    Card_Direction*     pSub_direction(void);
    Card_Emit*          pSub_emit(void);
    Card_Expr*          pSub_expr(void);
    Card_FiltPred*      pSub_filtPred(void);
    Card_FiltPredList*  pSub_filtPredList(void);
    Card_Filter*        pSub_filter(void);
    Card_Flat*          pSub_flat(void);
    Card_Fold*          pSub_fold(void);
    Card_From*          pSub_from(void);
    Card_GbyAgg*        pSub_gbyAgg(void);
    Card_Hashing*       pSub_hashing(void);
    Card_HshCol*        pSub_hshCol(void);
    Card_HshColList*    pSub_hshColList(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_Inflate*       pSub_inflate(void);
    Card_Injectable*    pSub_injectable(void);
    Card_JoinCast*      pSub_joinCast(void);
    Card_JoinEstimate*  pSub_joinEstimate(void);
    Card_Key*           pSub_key(void);
    Card_KeyCells*      pSub_keyCells(void);
    Card_KeyExists*     pSub_keyExists(void);
    Card_KeyNotExists*  pSub_keyNotExists(void);
    Card_Level*         pSub_level(void);
    Card_Map*           pSub_map(void);
    Card_Mapping*       pSub_mapping(void);
    Card_Needed*        pSub_needed(void);
    Card_NotConj*       pSub_notConj(void);
    Card_Note*          pSub_note(void);
    Card_On*            pSub_on(void);
    Card_OuterColNm*    pSub_outerColNm(void);
    Card_OuterJoin*     pSub_outerJoin(void);
    Card_Payload*       pSub_payload(void);
    Card_PayloadCells*  pSub_payloadCells(void);
    Card_RelnOp*        pSub_relnOp(void);
    Card_Required*      pSub_required(void);
    Card_RowCount*      pSub_rowCount(void);
    Card_RowSize*       pSub_rowSize(void);
    Card_RpnCalc*       pSub_rpnCalc(void);
    Card_Scan*          pSub_scan(void);
    Card_Section*       pSub_section(void);
    Card_Spare*         pSub_spare(void);
    Card_SqyCalls*      pSub_sqyCalls(void);
    Card_SqyInfo*       pSub_sqyInfo(void);
    Card_SqyNo*         pSub_sqyNo(void);
    Card_Stream*        pSub_stream(void);
    Card_StreamHead*    pSub_streamHead(void);
    Card_StreamJoin*    pSub_streamJoin(void);
    Card_StreamKey*     pSub_streamKey(void);
    Card_StreamPer*     pSub_streamPer(void);
    Card_StreamPtn*     pSub_streamPtn(void);
    Card_StreamSet*     pSub_streamSet(void);
    Card_SubQuery*      pSub_subQuery(void);
    Card_Synthesis*     pSub_synthesis(void);
    Card_Table*         pSub_table(void);
    Card_TableInfo*     pSub_tableInfo(void);
    Card_TableNm*       pSub_tableNm(void);
    Card_TablePtn*      pSub_tablePtn(void);
    Card_TableSize*     pSub_tableSize(void);
    Card_TblSyn*        pSub_tblSyn(void);
    Card_Term*          pSub_term(void);
    Card_Top*           pSub_top(void);
    Card_Topology*      pSub_topology(void);
    Card_Unique*        pSub_unique(void);
    Card_Where*         pSub_where(void);
    Card_XCard*         pSub_xcard(void);

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
