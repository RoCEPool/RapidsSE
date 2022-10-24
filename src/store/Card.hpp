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
  class Card_Level;
  class Card_Aggreg;
  class Card_Aggregates;
  class Card_AndPred;
  class Card_As;

  class Card_BoolCalc;
  class Card_Bounds;

  class Card_CandIndex;
  class Card_CmpPred;
  class Card_ColumnRef;
  class Card_Combined;
  class Card_CoPtnJoin;
  class Card_CoPtnJoinList;
  class Card_CoPtnPred;

  class Card_Debug;
  class Card_Deck;
  class Card_Direction;

  class Card_EBound;
  class Card_ELIST;
  class Card_Exists;
  class Card_Expr;
  class Card_ExprBool;

  class Card_Filter;

  class Card_GbyElist;

  class Card_Hash;
  class Card_Having;

  class Card_IdxCol;
  class Card_IdxColList;
  class Card_IdxColNm;
  class Card_Index;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexNm;
  class Card_IndexPred;
  class Card_IndexScan;

  class Card_JoinOrderSeq;
  class Card_JoinPredList;

  class Card_LBound;
  class Card_Limit;

  class Card_OrdAgg;
  class Card_OrPred;
  class Card_ObyElist;

  class Card_PRED;
  class Card_Project;
  class Card_ProjElist;
  class Card_PtndJoin;

  class Card_RBound;
  class Card_RpnCalc;
  class Card_RpnTerms;

  class Card_SbyElist;
  class Card_Scan;
  class Card_Size;
  class Card_Send;
  class Card_Sort;
  class Card_Stage;
  class Card_Stream;
  class Card_StmColList;
  class Card_StmColOrd;
  class Card_SubSet;

  class Card_TableList;
  class Card_TableNm;
  class Card_TablePtn;
  class Card_Term;
  class Card_Topology;
  class Card_Type;

  class Card_UBound;
  class Card_Unique;
}

static Stz* P_AggLevel     = "eAggLevel"_Stz;
static Stz* P_AggOp        = "bAggOp"_Stz;
static Stz* P_DebugOption  = "szDebugOption"_Stz;
static Stz* P_Exclude      = "bExclude"_Stz;
static Stz* P_Invalid      = "bInvalid"_Stz;
static Stz* P_JoinedBuff   = "nJoinedBuff"_Stz;
static Stz* P_Network      = "nNetwork"_Stz;
static Stz* P_OrdHi        = "iOrdHi"_Stz;
static Stz* P_OrdLo        = "iOrdLo"_Stz;
static Stz* P_ParamOrd     = "szParamOrd"_Stz;
static Stz* P_Permutation  = "i_Permutation"_Stz;
static Stz* P_PtndJoin     = "i_PtndJoin"_Stz;
static Stz* P_Rank         = "iCandIndex_Rank"_Stz;
static Stz* P_SendKind     = "eSendKind"_Stz;
static Stz* P_SentOut      = "nSentOut"_Stz;
static Stz* P_StageTopo    = "eStageTopo"_Stz;
static Stz* P_TableUsed    = "bTableUsed"_Stz;

static char MARK_ASSIGNED   = '@';
static char MARK_BLANK      = ' ';
static char MARK_CHANGED    = '`';
static char MARK_DOWNWARDS  = 'v';
static char MARK_LEFTWARDS  = '<';
static char MARK_MANY       = '*';
static char MARK_MORE       = '+';
static char MARK_RIGHTWARDS = '>';
static char MARK_UPWARDS    = '^';

static Stz* SEP_DERIVED    = "+ + + + + + + + + "_Stz;
static Stz* SEP_SCRATCH    = "? ? ? ? ? ? ? ? ? "_Stz;

namespace STORE
{
  static const bool C_CONTINUE_RECURSION  = true;
  static const bool C_PRINT_PHASE         = true;
  static const bool C_PRINT_XFORM         = true;
  static const bool C_TRACE_Y             = true;
  static const bool C_UPDATE_MEMBER       = true;

  static const bool C_QUIET_PHASE         = false;
  static const bool C_QUIET_XFORM         = false;
  static const bool C_TERMINATE_RECURSION = false;
  static const bool C_TRACE_N             = false;

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

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static eCK  eTo_eCK(Stz* szCardNm);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vAppendDupSubCards(Card* pFromCard);
    void vAppendMain(Card* pNewMainCard, bool bRenumber= true);
    void vApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfApply);

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

    void vDeleteChild(eCK eCard);
    void vDeleteProperty(Stz* szPpyNm);
    void vDeletePropertyRecursive(Stz* szPpyNm);
    void vDump(i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCardAndSubCards(void);
    Card_SubSet* pDupSubCards(void);

    bool bEquivalentTo(Card* pCard_OTHER);

    Card* pFindParent(Card* pSubCard);
    Card* pFindValueRecursive(Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindValueRecursive(eCK eCard, Stz* szValue, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindPpyRecursive(Stz* szPpy, eCK eOfCard=CK_NONE);

    bool bHasChildWithValue(Stz* szValue);
    bool bHasProperty(Stz* szPpyNm);
    bool bHasSubRecursive(Card* pSubCard);
    bool bHasSubRecursive(Card* pSubCard, Card*& rpParent);
    bool bHasValue(void);

    bool bIsCard(eCK eCard);
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
    void vMoveToLastChild(Card* pChildCard);

    void vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent, bool& rbPrune);
    void vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm);
    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);
    void vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameValue(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintValue(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks=false);
    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    TagSet* pProperties(void);
    void vPropertiesEnsure(void);

    void vRenumberRecursive(Card* pParent=nullptr);

    bool bSame(Stz* szArg);

    Stz* sz(void);
    char cValue(void);
    Stz* szValue(void);
    void vValue(Stz* szValue);
    void vValueSprint(Stz* szFormat, ...);
    bool bValuesSameRecursive(Card* pCard_b, i8 iDepth= 0, bool bTestL0= false);

    Stz* Y_szNm(i8 i_Property);
    char Y_cVal(i8 i_Property);
    char Y_cVal(Stz* szPpyNm);
    i8   Y_iVal(i8 i_Property);
    i8   Y_iVal(Stz* szPpyNm);
    Object* Y_pVal(i8 i_Property);
    Object* Y_pVal(Stz* szPptNm);
    Stz* Y_szVal(i8 i_Property);
    Stz* Y_szVal(Stz* szPptNm);

    void Y_vSet(Stz* szPpyNm, char cPpyVal);
    void Y_vSet(Stz* szPpyNm, i8 iPpyVal);
    void Y_vSet(Stz* szPpyNm);
    void Y_vSet(Stz* szPpyNm, Object* pPpyVal);
    void Y_vSet(Stz* szPpyNm, Stz* szPpyVal);
    void Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_CmpPred* pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound);
    bool bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex);

    bool bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    void vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    i8   iCandIndex_Rank(Card_CandIndex* p_candIndex);
    void vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank);

    void vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);
    bool bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    Card_CmpPred* pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound);
    bool bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex);
    bool bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn);

    bool bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef);
    i8   iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef);
    char cColumnRef_TableSyn(Card_ColumnRef* p_columnRef);
    char cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef);
    void vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn);

    bool bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred);
    Card_CoPtnJoin* pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred);
    bool bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp);

    Card_StmColList* pELIST_DeriveStreamColList(Card_ELIST* p_ELIST);

    void vExpr_AsEnsure(Card_Expr* p_expr);

    void vFilter_CullPred(Card_Filter* p_filter, char cWithMark);
    bool bFilter_IsEmptyFilter(Card_Filter* p_filter);

    void vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void vJoinOrderSeq_ExtractJoinOrder(Card_PtndJoin* p_PtndJoin);

    void vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex);

    Card_CmpPred* pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound);
    bool bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex);

    void vPRED_ConcatTerms(Card_PRED* p_PRED, Stz* szTerms, i8 i_From, i8 i_Till= C_UNKNOWN);
    void vPRED_CullByKids(Card_PRED* p_PRED);
    Card_CmpPred* pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex, ePB eBound);
    bool bPRED_HasBounds (Card_PRED* p_PRED, Index* pIndex);
    static bool bPRED_IsPred(Card* pArgCard);

    bool bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar);
    void vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  

    Card_As*            pSub_as(void);
    Card_BoolCalc*      pSub_boolCalc(void);
    Card_Bounds*        pSub_bounds(void);
    Card_ColumnRef*     pSub_columnRef(void);
    Card_Filter*        pSub_filter(void);
    Card_GbyElist*      pSub_gbyElist(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_IdxColNm*      pSub_idxColNm(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_LBound*        pSub_lBound(void);
    Card_CmpPred*       pSub_predCmp(void);
    Card_RpnCalc*       pSub_rpnCalc(void);
    Card_SbyElist*      pSub_sbyElist(void);
    Card_Scan*          pSub_scan(void);
    Card_Size*          pSub_size(void);
    Card_Stream*        pSub_stream(void);
    Card_StmColList*    pSub_stmColList(void);
    Card_StmColOrd*     pSub_stmColOrd(void);
    Card_Table*         pSub_table(void);
    Card_TableNm*       pSub_tableNm(void);
    Card_TablePtn*      pSub_tablePtn(void);
    Card_Topology*      pSub_topology(void);
    Card_UBound*        pSub_uBound(void);

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
