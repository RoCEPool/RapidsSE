/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "ePB.h"
#include "eGC.h"
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
  class Card_AggElist;
  class Card_Aggreg;
  class Card_Aggregates;
  class Card_AndPred;
  class Card_As;

  class Card_BolPred;
  class Card_BoolCalc;
  class Card_Bound;
  class Card_Bounds;

  class Card_CalcOp;
  class Card_CandIndex;
  class Card_Cast;
  class Card_CmpPred;
  class Card_Clause;
  class Card_ColumnRef;
  class Card_Combined;
  class Card_Comment;
  class Card_Constraint;
  class Card_CoPtnJoin;
  class Card_CoPtnJoinList;
  class Card_CoPtnPred;
  class Card_CrossJoin;

  class Card_Debug;
  class Card_Deck;
  class Card_Direction;

  class Card_EBound;
  class Card_ELIST;
  class Card_Exists;
  class Card_Expr;

  class Card_Filter;
  class Card_Fold;
  class Card_FoldElist;

  class Card_GbyAgg;
  class Card_GbyElist;
  class Card_Genesis;
  class Card_Group;

  class Card_Hash;
  class Card_Having;

  class Card_IdxCol;
  class Card_IdxColList;
  class Card_IdxColNm;
  class Card_Index;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexNm;
  class Card_IndexScan;

  class Card_JoinOrder;
  class Card_JoinOrdering;
  class Card_JoinPredList;

  class Card_KeyExists;
  class Card_KeyNotExists;

  class Card_LBound;
  class Card_Level;
  class Card_Limit;

  class Card_Manner;

  class Card_NCorrCols;
  class Card_Next;
  class Card_Note;
  class Card_NotPred;
  class Card_NParmCols;

  class Card_On;
  class Card_OrdAgg;
  class Card_Order;
  class Card_OrPred;
  class Card_ObyElist;

  class Card_PRED;
  class Card_Project;
  class Card_ProjElist;
  class Card_PtndJoin;
  class Card_PmzWhere;

  class Card_RBound;
  class Card_Reason;
  class Card_RpnCalc;
  class Card_RpnTerms;

  class Card_SbyElist;
  class Card_Scan;
  class Card_Size;
  class Card_Send;
  class Card_Sort;
  class Card_RoPred;
  class Card_Stage;
  class Card_Stream;
  class Card_StmCol;
  class Card_StmColList;
  class Card_StmColNm;
  class Card_StmColOdn;
  class Card_OrdElmOdn;
  class Card_SubSet;

  class Card_TableList;
  class Card_TableNm;
  class Card_TablePtn;
  class Card_TblColOdn;
  class Card_TblSyn;
  class Card_Term;
  class Card_Topology;
  class Card_Twist;
  class Card_Type;

  class Card_UBound;
  class Card_Unique;

  class Card_Where;

  class Card_XBound;

  class Card_Av_AggP;
  class Card_Av_AggPN;
  class Card_Av_AggPNO;
  class Card_Av_AggregP;
  class Card_Av_AggregPN;
  class Card_Av_AggregPNO;
  class Card_Av_Filter;
  class Card_Av_FilterProject;
  class Card_Av_GbyAggP;
  class Card_Av_GbyAggPN;
  class Card_Av_GbyPNO;
  class Card_Av_JoinSeries;
  class Card_Av_Limit;
  class Card_Av_Project;
  class Card_Av_ScanJoinSeries;
  class Card_Av_ScanOnly;
  class Card_Av_Sort;
}

static Stz* P_AggOp        = "bAggOp"_Stz;
static Stz* P_BumpedBy     = "nBumpedBy"_Stz;
static Stz* P_BumpRight    = "nBumpRight"_Stz;
static Stz* P_BumpedRight  = "nBumpedRight"_Stz;
static Stz* P_ChosenIndex  = "bChosenIndex"_Stz;
static Stz* P_CorrPred     = "bCorrPred"_Stz;
static Stz* P_CorrStmCol   = "bCorrStmCol"_Stz;
static Stz* P_DebugOption  = "szDebugOption"_Stz;
static Stz* P_Focus        = "szFocus"_Stz;
static Stz* P_HideHeat     = "bHideHeat"_Stz;
static Stz* P_Inject       = "bInject"_Stz;
static Stz* P_Injected     = "bInjected"_Stz;
static Stz* P_Invalid      = "bInvalid"_Stz;
static Stz* P_JoinedBuff   = "nJoinedBuff"_Stz;
static Stz* P_Level        = "eLevel"_Stz;
static Stz* P_Network      = "nNetwork"_Stz;
static Stz* P_OdnHi        = "iOdnHi"_Stz;
static Stz* P_OdnLo        = "iOdnLo"_Stz;
static Stz* P_ParamOdn     = "szParamOdn"_Stz;
static Stz* P_Permutation  = "i_Permutation"_Stz;
static Stz* P_ProjBump     = "nProjBump"_Stz;
static Stz* P_ProjBumped   = "nProjBumped"_Stz;
static Stz* P_PtndJoin     = "i_PtndJoin"_Stz;
static Stz* P_Rank         = "iRank"_Stz;
static Stz* P_Restream     = "bRestream"_Stz;
static Stz* P_Restreamed   = "bRestreamed"_Stz;
static Stz* P_SendKind     = "eSendKind"_Stz;
static Stz* P_SentOut      = "nSentOut"_Stz;
static Stz* P_StageTopo    = "eStageTopo"_Stz;
static Stz* P_TableUsed    = "bTableUsed"_Stz;

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

    static Table* pAccess_Table(Stz* szTableNm);

    static Stz* szBound(Stz* szRelnOp);

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static i8   iGetColOdn(Stz* szValue);

    static bool bIsIndexColOrdinal(Stz* szValue);  // $In

    static bool bIsKeyOrdinal(Stz* szValue);       // #n

    static bool bIsParamOrdinal(Stz* szValue);     // $Pn

    static bool bIsResultOrdinal(Stz* szValue);     // #n

    static bool bIsSubQueryName(Stz* szValue);     // __subqueryN

    static bool bIsSqyColOrdinal(Stz* szValue);    // __n

    static bool bIsStreamColOrdinal(Stz* szValue); // $n

    static bool bIsTableColOrdinal(Stz* szValue);  // $c.n

    static char cTableColSyn(Stz* szValue);

    static eCK  eTo_eCK(Stz* szCardNm);

    static char cTypeShort(Stz* szType);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vAppendDupSubCards(Card* pFromCard);
    void vAppendMain(Card* pNewMainCard, bool bRenumber= true);
    Card* pAppendNewCardTail(eCK eCard, Stz* szValue=nullptr);
    void vAppendNewCardTail(eCK eCard, Stz* szValue=nullptr);
    void vAppendNewCardTop(eCK eCard, Stz* szValue=nullptr);
    bool bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);
    void vApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn);

    Stz* szBearing(void);
    void vBearing(void);
    void vBearing(Card* pParentCard);

    eCK  eCard(void);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    void vClearMarksRecursive(void);
    void vCopyProperties(Card* pFrom);

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
    bool bIsValue(Stz* szValue);

    Card* pLastPrintParent(void);
    void vLastPrintParent(Card* pLastPrintParent);

    char cMark1(void);
    void vMark1(char cMark1);
    void vMark1SetIfRecursive(char cIfMark, char cSetTo);
    char cMark2(void);
    i8   iMark2(void);
    void vMark2(i8 iMark2);
    void vMark2(char cMark2);
    void vMark2SetIfRecursive(char cIfMark, char cSetTo);
    void vMarkResetRecursive(void);
    bool bMerge(Card* pCard);
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
    void vReplaceSubCard(Card* pSub, Card* pWith);

    bool bSame(Stz* szArg);

    void vTakeAllSubCards(Card* pCard, bool bDeleteCard=false);

    Stz* sz(void);
    char cValue(void);
    Stz* szValue(void);
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

    void vStream_PrintStream(Card_Stream* p_stream, Ribbon* pRibbon, i8 iDepth);
    void vTable_PrintTable(Card_Table* p_Table, Ribbon* pRibbon, i8 iDepth);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_AggElist*      pSub_aggElist(void);
    Card_Aggreg*        pSub_aggreg(void);
    Card_As*            pSub_as(void);
    Card_BolPred*       pSub_bolPred(void);
    Card_BoolCalc*      pSub_boolCalc(void);
    Card_Bound*         pSub_bound(void);
    Card_Bounds*        pSub_bounds(void);
    Card_CalcOp*        pSub_calcOp(void);
    Card_Cast*          pSub_cast(void);
    Card_Clause*        pSub_clause(void);
    Card_CmpPred*       pSub_cmpPred(void);
    Card_ColumnRef*     pSub_columnRef(void);
    Card_Comment*       pSub_comment(void);
    Card_Constraint*    pSub_constraint(void);
    Card_CrossJoin*     pSub_crossJoin(void);
    Card_Direction*     pSub_direction(void);
    Card_EBound*        pSub_eBound(void);
    Card_Expr*          pSub_expr(void);
    Card_Filter*        pSub_filter(void);
    Card_Fold*          pSub_fold(void);
    Card_FoldElist*     pSub_foldElist(void);
    Card_GbyAgg*        pSub_gbyAgg(void);
    Card_GbyElist*      pSub_gbyElist(void);
    Card_Genesis*       pSub_genesis(void);
    Card_Group*         pSub_group(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_IdxColNm*      pSub_idxColNm(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_KeyExists*     pSub_keyExists(void);
    Card_KeyNotExists*  pSub_keyNotExists(void);
    Card_LBound*        pSub_lBound(void);
    Card_Level*         pSub_level(void);
    Card_Manner*        pSub_manner(void);
    Card_NCorrCols*     pSub_nCorrCols(void);
    Card_Next*          pSub_next(void);
    Card_Note*          pSub_note(void);
    Card_NotPred*       pSub_notPred(void);
    Card_NParmCols*     pSub_nParmCols(void);
    Card_On*            pSub_on(void);
    Card_OrdElmOdn*     pSub_ordElmOdn(void);
    Card_Order*         pSub_order(void);
    Card_PmzWhere*      pSub_pmzWhere(void);
    Card_ProjElist*     pSub_projElist(void);
    Card_Reason*        pSub_reason(void);
    Card_RoPred*        pSub_roPred(void);
    Card_RpnCalc*       pSub_rpnCalc(void);
    Card_SbyElist*      pSub_sbyElist(void);
    Card_Scan*          pSub_scan(void);
    Card_Size*          pSub_size(void);
    Card_Stream*        pSub_stream(void);
    Card_StmCol*        pSub_stmCol(void);
    Card_StmColList*    pSub_stmColList(void);
    Card_StmColNm*      pSub_stmColNm(void);
    Card_StmColOdn*     pSub_stmColOdn(void);
    Card_Table*         pSub_table(void);
    Card_TableNm*       pSub_tableNm(void);
    Card_TablePtn*      pSub_tablePtn(void);
    Card_TblColOdn*     pSub_tblColOdn(void);
    Card_TblSyn*        pSub_tblSyn(void);
    Card_Term*          pSub_term(void);
    Card_Topology*      pSub_topology(void);
    Card_Twist*         pSub_twist(void);
    Card_Type*          pSub_type(void);
    Card_UBound*        pSub_uBound(void);
    Card_Unique*        pSub_unique(void);
    Card_Where*         pSub_where(void);
    Card_XBound*        pSub_xBound(void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_Av_AggP*          pSub_avAggP(void);
    Card_Av_AggPN*         pSub_avAggPN(void);
    Card_Av_AggPNO*        pSub_avAggPNO(void);
    Card_Av_AggregP*       pSub_avAggregP(void);
    Card_Av_AggregPN*      pSub_avAggregPN(void);
    Card_Av_AggregPNO*     pSub_avAggregPNO(void);
    Card_Av_Filter*        pSub_avFilter(void);
    Card_Av_FilterProject* pSub_avFilterProject(void);
    Card_Av_GbyAggP*       pSub_avGbyAggP(void);
    Card_Av_GbyAggPN*      pSub_avGbyAggPN(void);
    Card_Av_GbyPNO*        pSub_avGbyPNO(void);
    Card_Av_JoinSeries*    pSub_avJoinSeries(void);
    Card_Av_Limit*         pSub_avLimit(void);
    Card_Av_Project*       pSub_avProject(void);
    Card_Av_ScanJoinSeries* pSub_avScanJoinSeries(void);
    Card_Av_ScanOnly*      pSub_avScanOnly(void);
    Card_Av_Sort*          pSub_avSort(void);

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
