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

namespace STORE
{
  class Card_AbsJoin;
  class Card_Aggreg;
  class Card_AndPred;
  class Card_As;

  class Card_BoolCalc;
  class Card_Bound;
  class Card_Bounds;

  class Card_Cast;
  class Card_CmpPred;
  class Card_Combined;
  class Card_Constraint;
  class Card_Correlation;
  class Card_CrossJoin;

  class Card_Debug;
  class Card_Deck;
  class Card_Direction;

  class Card_ELIST;
  class Card_Expr;

  class Card_Filter;
  class Card_Flat;
  class Card_Fold;
  class Card_FoldElist;
  class Card_From;

  class Card_GbyAgg;

  class Card_Hash;
  class Card_Having;
  class Card_HshCol;

  class Card_IdxCol;
  class Card_IdxColList;
  class Card_Index;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexNm;
  class Card_IndexScan;

  class Card_JoinHead;
  class Card_JoinNext;
  class Card_JoinOrder;
  class Card_JoinOrdering;
  class Card_JoinPermAll;
  class Card_JoinPermSeq;
  class Card_JoinPredList;

  class Card_Key;
  class Card_KeyExists;
  class Card_KeyNotExists;

  class Card_Level;
  class Card_Limit;
  class Card_Load;
  class Card_LoadJoined;
  class Card_LoadNetwork;
  class Card_LoadPermSeq;
  class Card_LoadUnicast;

  class Card_NCorrCols;
  class Card_Needed;
  class Card_Next;
  class Card_Note;
  class Card_NotPred;

  class Card_On;
  class Card_OP;
  class Card_OrdAgg;
  class Card_OrPred;
  class Card_OuterColNm;
  class Card_OuterJoin;

  class Card_PRED;
  class Card_Project;
  class Card_Projection;

  class Card_RpnCalc;
  class Card_RpnTerms;

  class Card_Scan;
  class Card_Section;
  class Card_Send;
  class Card_Size;
  class Card_Sort;
  class Card_Stage;
  class Card_SubSet;

  class Card_TableList;
  class Card_TableNm;
  class Card_TablePtn;
  class Card_TblSyn;
  class Card_Term;
  class Card_Top;
  class Card_Topology;
  class Card_Type;

  class Card_Unique;

  class Card_Where;

  class CardIter;
}

static Stz* P_AggOp        = "bAggOp"_Stz;
static Stz* P_As           = "szAs"_Stz;
static Stz* P_BreakPt      = "bBreakPt"_Stz;
static Stz* P_BumpedBy     = "nBumpedBy"_Stz;
static Stz* P_ChosenBound  = "bChosenBound"_Stz;
static Stz* P_ChosenIndex  = "bChosenIndex"_Stz;
static Stz* P_Column       = "bColumn"_Stz;
static Stz* P_CrlKeyExpr   = "bCrlKeyExpr"_Stz;
static Stz* P_CrlKeyPred   = "bCrlKeyPred"_Stz;
static Stz* P_CrlKeyTest   = "bCrlKeyTest"_Stz;
static Stz* P_CrlRslTest   = "bCrlRslTest"_Stz;
static Stz* P_DebugOption  = "szDebugOption"_Stz;
static Stz* P_Early        = "bEarly"_Stz;
static Stz* P_HideHeat     = "bHideHeat"_Stz;
static Stz* P_Injected     = "bInjected"_Stz;
static Stz* P_InjCrlKey    = "bInjCrlKey"_Stz;
static Stz* P_Invalid      = "bInvalid"_Stz;
static Stz* P_JoinedBuff   = "nJoinedBuff"_Stz;
static Stz* P_Level        = "eLevel"_Stz;
static Stz* P_Network      = "nNetwork"_Stz;
static Stz* P_ParamOdn     = "szParamOdn"_Stz;
static Stz* P_Permutation  = "i_Permutation"_Stz;
static Stz* P_ProjBump     = "nProjBump"_Stz;
static Stz* P_ProjBumped   = "nProjBumped"_Stz;
static Stz* P_PtndJoin     = "i_PtndJoin"_Stz;
static Stz* P_SendKind     = "eSendKind"_Stz;
static Stz* P_SentOut      = "nSentOut"_Stz;
static Stz* P_Smoothed     = "bSmoothed"_Stz;
static Stz* P_StageTopo    = "eStageTopo"_Stz;
static Stz* P_StmCol       = "szStmCol"_Stz;
static Stz* P_SubQryCall   = "bSubQryCall"_Stz;
static Stz* P_TableUsed    = "bTableUsed"_Stz;
static Stz* P_TblCol       = "szTblCol"_Stz;
static Stz* P_Type         = "szType"_Stz;
static Stz* P_Unproject    = "bUnproject"_Stz;

static Stz* C_GLEAN_CUE_FROM    = "<glean_cue_from>"_Stz;
static Stz* C_GLEAN_INTERACTIVE = "<glean_interactive>"_Stz;
static Stz* C_GLEAN_TRACE       = "<glean_trace>"_Stz;
static Stz* C_GLEAN_XFORM       = "<glean_xform>"_Stz;
static Stz* C_GLEAN_XHAND       = "<glean_xhand>"_Stz;
static Stz* C_NA                = "n/a"_Stz;

static char MARK_AT         = '@';
static char MARK_BLANK      = ' ';
static char MARK_CHANGED    = '`';
static char MARK_UP         = '^';

namespace STORE
{
  static const bool C_CONTINUE_RECURSION  = true;
  static const bool C_TERMINATE_RECURSION = false;

  class Symbol {
  public:
    s256    m_sValue;
    TagSet* m_pProperties;

  public:
    void operator =(Stz* szArg);
    void operator =(Symbol* pSymbol);
    void operator =(Symbol& rxSymbol);

    // auto conversion
    operator Stz*(void);


    static eQO  e_EvalSubQryOp(Stz* szValue);
    static i8   i_GetColOdn(Stz* szValue);

    static bool b_IsStreamColOrdinal(Stz* szValue); // $n
    static bool b_IsSubQueryOrdinal(Stz* szValue);  // __subqueryN
    static bool b_IsTableColOrdinal(Stz* szValue);  // $c.n

    eRO  eEvalRelnOp(void);
    eQO  eEvalSubQryOp(void);

    i8   iGetColOdn(void);

    bool bIsAggOp(void);
    bool bIsBoolFn(void);           // between, ... matchTrailing
    bool bIsCommaSeparator(void);
    bool bIsIndexColOrdinal(void);  // $In
    bool bIsKeyOrdinal(void);       // $Kn
    bool bIsColOrdinal(void);       // $n or $c.n
    bool bIsCorrParamOrdinal(void); // %n
    bool bIsRelnOp(void);           // <, <=, ... >
    bool bIsResultOrdinal(void);    // #n
    bool bIsStreamColOrdinal(void); // $n
    bool bIsSubQueryOp(void);       // exists, in, ...
    bool bIsSubQueryPattern(void);  // __subqueryN
    bool bIsSqyColOrdinal(void);    // __n
    bool bIsTableColOrdinal(void);  // $c.n

    bool bNeedsBrace(void);

    void vSet(Symbol* pSymbol);

    void vSetIndexColOrdinal(i8 iIdxColOdn);
    void vSetKeyOrdinal(i8 iKeyOdn);
    void vSetStreamColOrdinal(i8 iStmColOdn);
    void vSetSubQueryOrdinal(i8 iSqNo);
    void vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn);
    void vSetTableColSyn(char cTblSyn);

    void vSprint(Stz* szFormat, ...);

    char cTableColSyn(void);

    Stz* szValue(void);

    void Y_vAll(Card* pCard_arg);
    void Y_vAll(TagSet* pProperties);
    void Y_vCut(Stz* szPpyNm);
    i8   Y_iGet(i8 i_Ppy);
    i8   Y_iGet(Stz* szPpyNm);
    Stz* Y_szGet(i8 i_Ppy);
    Stz* Y_szGet(Stz* szPptNm);
    bool Y_bHas(Stz* szPpyNm);
    i8   Y_nPpy(void);
    Stz* Y_szPpyNm(i8 i_Ppy);
    eTN  Y_ePpyTkn(i8 i_Ppy);
    void Y_vSet(Stz* szPpyNm);
    void Y_vSet(Stz* szPpyNm, i8 iPpy);
    void Y_vSet(Stz* szPpyNm, Stz* szPpy);
    void Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...);

  };

  class Card : public Object
             , public ChildCards
  {
  protected:
    static i8 m__iSeqGen;

    char    md_Bearing[256];

    eCK     m_eCard;
    i8      m_i_Child;

    char    m_cMark1;
    char    m_cMark2;
    Symbol  m_xSymbol;

    Card*   m_pLastPrintParent;

    i8 md__SeqNo;
    i8 md__FirstDetected;
    i8 md__LeakDetected;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard);
    Card(eCK eCard, Symbol* pSymbol);
    Card(eCK eCard, Stz* szValue);
    Card(eCK eCard, Stz* szValue, Stz* szPpyNm, i8 iPpy);
    Card(eCK eCard, Stz* szValue, Stz* szPpyNm, Stz* szPpy);
    void vInit(eCK eCard);

   ~Card(void) noexcept(false);

    static Stz* szBound(Stz* szRelnOp);

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static char cAggrLevelToChar(eAL eLevel);

    static eYG  eAggrLevelToStageTopology(eAL eLevel);


    static eCK  eTo_eCK(Stz* szCardNm);

    static eCT  eTo_eCT(Stz* szType);

    static eAL  eStageTopologyToAggrLevel(eYG eStageTopo);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card* pAppendNewCard(eCK eCard, Stz* szValue=nullptr);
    void vAppendNewCard(eCK eCard, Stz* szValue=nullptr);
    bool bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);
    bool bApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn);

    Stz* szBearing(void);
    void vBearing(void);
    void vBearing(Card* pParentCard);

    eCK  eCard(void);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   i_Child(void);
    void v_Child(i8 i_Child);
    void vClearMarksRecursive(void);

    void  vDump(i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCard(void);
    Card* pDupCardRecursive(void);
    Card_SubSet* pDupSubCardsRecursive(i8 i_From, i8 i_Till=C_UNKNOWN);

    Card* pFindCardRecursive(eCK eCard);
    Card* pFindParentRecursive(Card* pSubCard);
    Card* pFindValueRecursive(Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindValueRecursive(eCK eCard, char cWithValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindValueRecursive(eCK eCard, Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindPpyRecursive(Stz* szPpy, eCK eOfCard=CK_NONE);

    bool bHasSubRecursive(Card* pSubCard);
    bool bHasSubWithValueRecursive(eCK eCard, Stz* szValue);
    bool bHasValue(void);

    bool bIsCard(eCK eCard);
    bool bIsValue(char cValue);
    bool bIsValue(Stz* szValue);

    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    void vMark2(char cMark2);

    void vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent,
                             bool& rbPrune, eAL eLevel,
                             bool bPrintBelow);
    void vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm);
    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);
    void vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon, eAL eLevel=AL_NONE);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameValue(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintValue(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks=false);

    void vRenumberRecursive(Card* pParent=nullptr);
    void vReplaceSubCard(Card* pSub, Card* pWith);

    bool bSameValue(Card* pCard);
    bool bSameValueRecursive(Card* pCard_b, i8 iDepth= 0, bool bTestL0= false);
    bool bSameValuePpyRecursive(Card* pCard_OTHER); // not needed??

    Symbol* pSymbol(void);

    char cValue(void);
    i8   iValue(void);
    Stz* szValue(void);
    void vValue(char cValue);
    void vValue(i8 iValue);
    void vValue(Stz* szValue);
    void vValueMark1(Stz* szValue, char cMark1);
    void vValueSprint(Stz* szFormat, ...);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_Aggreg*        pSub_aggreg(void);
    Card_As*            pSub_as(void);
    Card_BoolCalc*      pSub_boolCalc(void);
    Card_Bound*         pSub_bound(void);
    Card_Bounds*        pSub_bounds(void);
    Card_Cast*          pSub_cast(void);
    Card_CmpPred*       pSub_cmpPred(void);
    Card_Constraint*    pSub_constraint(void);
    Card_Correlation*   pSub_correlation(void);
    Card_CrossJoin*     pSub_crossJoin(void);
    Card_Direction*     pSub_direction(void);
    Card_Expr*          pSub_expr(void);
    Card_Filter*        pSub_filter(void);
    Card_Flat*          pSub_flat(void);
    Card_Fold*          pSub_fold(void);
    Card_FoldElist*     pSub_foldElist(void);
    Card_From*          pSub_from(void);
    Card_GbyAgg*        pSub_gbyAgg(void);
    Card_HshCol*        pSub_hshCol(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_JoinHead*      pSub_joinHead(void);
    Card_JoinNext*      pSub_joinNext(void);
    Card_JoinPermAll*   pSub_joinPermAll(void);
    Card_JoinPermSeq*   pSub_joinPermList(void);
    Card_Key*           pSub_key(void);
    Card_KeyExists*     pSub_keyExists(void);
    Card_KeyNotExists*  pSub_keyNotExists(void);
    Card_Level*         pSub_level(void);
    Card_Load*          pSub_load(void);
    Card_LoadJoined*    pSub_loadJoined(void);
    Card_LoadNetwork*   pSub_loadNetwork(void);
    Card_LoadPermSeq*   pSub_loadPermSeq(void);
    Card_LoadUnicast*   pSub_loadUnicast(void);
    Card_NCorrCols*     pSub_nCorrCols(void);
    Card_Needed*        pSub_needed(void);
    Card_Next*          pSub_next(void);
    Card_Note*          pSub_note(void);
    Card_NotPred*       pSub_notPred(void);
    Card_On*            pSub_on(void);
    Card_OuterColNm*    pSub_outerColNm(void);
    Card_OuterJoin*     pSub_outerJoin(void);
    Card_Projection*    pSub_projection(void);
    Card_RpnCalc*       pSub_rpnCalc(void);
    Card_Scan*          pSub_scan(void);
    Card_Section*       pSub_section(void);
    Card_Size*          pSub_size(void);
    Card_Table*         pSub_table(void);
    Card_TableNm*       pSub_tableNm(void);
    Card_TablePtn*      pSub_tablePtn(void);
    Card_TblSyn*        pSub_tblSyn(void);
    Card_Term*          pSub_term(void);
    Card_Top*           pSub_top(void);
    Card_Topology*      pSub_topology(void);
    Card_Type*          pSub_type(void);
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
