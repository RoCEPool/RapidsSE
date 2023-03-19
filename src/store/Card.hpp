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
#include "PpySet.hpp"

#include "Ribbon.hpp"

namespace STORE
{
  class Card_AbsJoin;
  class Card_Aggreg;
  class Card_AndPred;
  class Card_As;

  class Card_BoardNo;
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
  class Card_Next;
  class Card_Note;
  class Card_NotPred;

  class Card_On;
  class Card_OrdAgg;
  class Card_OrdElmOdn;
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
static Stz* P_BumpedBy     = "nBumpedBy"_Stz;
static Stz* P_ChosenIndex  = "bChosenIndex"_Stz;
static Stz* P_CrlKeyExpr   = "bCrlKeyExpr"_Stz;
static Stz* P_CrlKeyPred   = "bCrlKeyPred"_Stz;
static Stz* P_CrlKeyTest   = "bCrlKeyTest"_Stz;
static Stz* P_CrlRslTest   = "bCrlRslTest"_Stz;
static Stz* P_DebugOption  = "szDebugOption"_Stz;
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
static Stz* P_SubQryCall   = "bSubQryCall"_Stz;
static Stz* P_TableUsed    = "bTableUsed"_Stz;
static Stz* P_Unproject    = "bUnproject"_Stz;

static Stz* C_GLEAN_CUE_FROM    = "<glean_cue_from>"_Stz;
static Stz* C_GLEAN_INTERACTIVE = "<glean_interactive>"_Stz;
static Stz* C_GLEAN_TRACE       = "<glean_trace>"_Stz;
static Stz* C_GLEAN_XFORM       = "<glean_xform>"_Stz;
static Stz* C_GLEAN_XHAND       = "<glean_xhand>"_Stz;
static Stz* C_NA                = "n/a"_Stz;

static char MARK_AT         = '@';
static char MARK_BLANK      = ' ';
static char MARK_CALL       = '~';
static char MARK_CHANGED    = '`';
static char MARK_DOWNWARDS  = 'v';
static char MARK_EXE        = '$';
static char MARK_LEFTWARDS  = '<';
static char MARK_LINK       = '%';
static char MARK_MANY       = '*';
static char MARK_MORE       = '+';
static char MARK_REPLACE    = '#';
static char MARK_RIGHTWARDS = '>';
static char MARK_TWIST      = '{';
static char MARK_UP         = '^';

static Stz* SEP_DERIVED    = "+ + + + + + + + + "_Stz;
static Stz* SEP_SCRATCH    = "? ? ? ? ? ? ? ? ? "_Stz;

namespace STORE
{
  static const bool C_CONTINUE_RECURSION  = true;
  static const bool C_PRINT_HAND          = true;
  static const bool C_PRINT_XFORM         = true;
  static const bool C_TRACE_Y             = true;
  static const bool C_UPDATE_MEMBER       = true;

  static const bool C_QUIET_PHASE         = false;
  static const bool C_QUIET_XFORM         = false;
  static const bool C_TERMINATE_RECURSION = false;
  static const bool C_TRACE_N             = false;

  static const i8 C_NO    = 0;
  static const i8 C_MAYBE = 1;
  static const i8 C_YES   = 2;

  static const bool C_ENABLED  = true;
  static const bool C_DISABLED = false;

  static const i8 C_MQ_BOARD   = 0;
  static const i8 C_SQ_BOARD   = 1;


  class Card : public Object
             , public ChildCards
  {
  protected:
    static i8 m__iSeqGen;

    char md_Bearing[256];

    eCK  m_eCard;
    i8   m_iCardNo;

    s256 m_sValue;
    char m_cMark1;
    char m_cMark2;

    TagSet* m_pProperties;
    PpySet  m_oPpySet;

    Card* m_pLastPrintParent;

    i8    md__SeqNo;
    i8    md__FirstDetected;
    i8    md__LeakDetected;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard);
    Card(eCK eCard, Stz* szValue);
    Card(eCK eCard, Stz* szValue, Stz* szPpyNm, i8 iPptVal);
    Card(eCK eCard, Stz* szValue, Stz* szPpyNm, Object* pPptVal);
    Card(eCK eCard, Stz* szValue, Stz* szPpyNm, Stz* szPptVal);
    void vInit(eCK eCard);

   ~Card(void) noexcept(false);

    static Stz* szBound(Stz* szRelnOp);

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static char cAggrLevelToChar(eAL eLevel);

    static eYG  eAggrLevelToStageTopology(eAL eLevel);

    static eRO  eEvalRelnOp(Stz* szValue);
    static eQO  eEvalSubQryOp(Stz* szValue);

    static i8   iGetColOdn(Stz* szValue);

    static bool bIsBoolFn(Stz* szValue);           // between, ... matchTrailing

    static bool bIsIndexColOrdinal(Stz* szValue);  // $In

    static bool bIsKeyOrdinal(Stz* szValue);       // #Kn

    static bool bIsColOrdinal(Stz* szValue); // %n

    static bool bIsCorrParamOrdinal(Stz* szValue); // %n

    static bool bIsRelnOp(Stz* szValue);           // <, <=, ... >

    static bool bIsResultOrdinal(Stz* szValue);    // #n

    static bool bIsSubQueryOp(Stz* szValue);       // exists, in, ...

    static bool bIsSubQueryPattern(Stz* szValue);  // __subqueryN

    static bool bIsSqyColOrdinal(Stz* szValue);    // __n

    static bool bIsStreamColOrdinal(Stz* szValue); // $n

    static bool bIsTableColOrdinal(Stz* szValue);  // $c.n

    static char cTableColSyn(Stz* szValue);

    static eCK  eTo_eCK(Stz* szCardNm);

    static eAL  eStageTopologyToAggrLevel(eYG eStageTopo);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vAddPredUnder_main(Card_PRED *p_PRED);
    void vAppendDupSubCards(Card* pFromCard);
    void vAppendMain(Card* pNewMainCard, bool bRenumber= true);
    Card* pAppendNewCardTail(eCK eCard, Stz* szValue=nullptr);
    void vAppendNewCardTail(eCK eCard, Stz* szValue=nullptr);
    void vAppendNewCardTop(eCK eCard, Stz* szValue=nullptr);
    bool bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);
    bool bApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn);

    Stz* szBearing(void);
    void vBearing(void);
    void vBearing(Card* pParentCard);

    eCK  eCard(void);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    void vClearMarksRecursive(void);
    //void vConvert_stmColOdn_To_tblColOdn_original(CardIter* pCI);
    void vCopyProperties(Card* pFrom);

    void vDeleteFilterIfEmpty(Card_Filter* p_filter);
    void vDump(i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDup(void);
    Card* pDupCardAndSubCards(void);
    Card_SubSet* pDupSubCards(void);
    Card_SubSet* pDupSubCards(i8 i_From, i8 i_Till=C_UNKNOWN);

    bool bEquivalentTo(Card* pCard_OTHER);

    Card* pFindCardRecursive(eCK eCard);
    Card* pFindParent(Card* pSubCard);
    Card* pFindValueRecursive(Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindValueRecursive(eCK eCard, char cWithValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindValueRecursive(eCK eCard, Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindPpyRecursive(Stz* szPpy, eCK eOfCard=CK_NONE);

    bool bHasChildWithValue(Stz* szValue);
    bool bHasSubRecursive(Card* pSubCard);
    bool bHasSubRecursive(Card* pSubCard, Card*& rpParent);
    bool bHasSubWithValue(eCK eCard, Stz* szValue);
    bool bHasValue(void);

    bool bIsAggOp(void);
    bool bIsCard(eCK eCard);
    bool bIsCommaSeparator(void);
    bool bIsConjunction(void);
    bool bIsPRED(void);
    bool bIsValue(char cValue);
    bool bIsValue(Stz* szValue);

    Card* pLastPrintParent(void);
    void vLastPrintParent(Card* pLastPrintParent);

    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    i8   iMark2(void);
    void vMark2(i8 iMark2);
    void vMark2(char cMark2);
    void vMarkResetRecursive(void);
    bool bMerge(Card* pCard);
    void vMorphChildrenTo(eCK eFrom, eCK eTo);
    void vMoveToLastChild(Card* pChildCard);

    bool bNeedsBrace(void);

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
    TagSet* pProperties(void);
    void vPropertiesEnsure(void);

    void vRenumberRecursive(Card* pParent=nullptr);
    void vRemove_filter_IfEmpty(Card_Filter* p_filter);
    void vReplaceSubCard(Card* pSub, Card* pWith);

    bool bSame(Stz* szArg);
    bool bSameValue(Card* pCard);
    void vSetIndexColOrdinal(i8 iIdxColOdn);
    void vSetKeyOrdinal(i8 iKeyOdn);
    void vSetParamOrdinal(i8 iParamOdn);
    void vSetStreamColOrdinal(i8 iStmColOdn);
    void vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn);
    void vSetTableColSyn(char cTblSyn);
    void vSetValueMark1(Stz* szValue, char cMark1);

    void vTakeAllSubCards(Card* pCard, bool bDeleteCard=false);

    Stz* sz(void);
    char cValue(void);
    i8   iValue(void);
    Stz* szValue(void);
    void vValue(char cValue);
    void vValue(i8 iValue);
    void vValue(Stz* szValue);
    void vValueSprint(Stz* szFormat, ...);
    bool bValuesSameRecursive(Card* pCard_b, i8 iDepth= 0, bool bTestL0= false);

    void Y_vCut(Stz* szPpyNm);
    void Y_vCutRecursive(Stz* szPpyNm);

    char Y_cGet(i8 i_Ppy);
    char Y_cGet(Stz* szPpyNm);
    i8   Y_iGet(i8 i_Ppy);
    i8   Y_iGet(Stz* szPpyNm);
    Object* Y_pGet(i8 i_Ppy);
    Object* Y_pGet(Stz* szPptNm);
    Stz* Y_szGet(i8 i_Ppy);
    Stz* Y_szGet(Stz* szPptNm);

    bool Y_bHas(Stz* szPpyNm);

    Stz* Y_szNm(i8 i_Ppy);

    eTN  Y_ePpy(i8 i_Ppy);
    i8   Y_nPpy(void);

    void Y_vSet(Stz* szPpyNm, char cPpy);
    void Y_vSet(Stz* szPpyNm, i8 iPpy);
    void Y_vSet(Stz* szPpyNm);
    void Y_vSet(Stz* szPpyNm, Object* pPpy);
    void Y_vSet(Stz* szPpyNm, Stz* szPpy);
    void Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...);

    Card_SubSet* pYankSubCards(i8 i_From, i8 i_Till=C_UNKNOWN);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_Aggreg*        pSub_aggreg(void);
    Card_As*            pSub_as(void);
    Card_BoardNo*       pSub_boardNo(void);
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
    Card_Next*          pSub_next(void);
    Card_Note*          pSub_note(void);
    Card_NotPred*       pSub_notPred(void);
    Card_On*            pSub_on(void);
    Card_OrdElmOdn*     pSub_ordElmOdn(void);
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
/* macro for find childCard function                                          */
/*----------------------------------------------------------------------------*/
#define __pChildFunction_Card(SPACENM,CARDNM,CN) \
  Card_##CARDNM* SPACENM::_pSub_##CARDNM(Card* pCard) \
  { \
    return _pCard_##CARDNM(pCard->pFindChild(CN)); \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for find subCard function                                            */
/*----------------------------------------------------------------------------*/
#define __pSubFunction_Card(SPACENM,CARDNM,CN) \
  Card_##CARDNM* SPACENM::_pSub_##CARDNM(Card* pCard) \
  { \
    return _pCard_##CARDNM(pCard->pFindSubCard(CN)); \
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
