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
  class Card_Stage;
  class Card_StreamJoin;
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

static Stz* P_AggOp        = "bAggOp"_Stz;
static Stz* P_As           = "szAs"_Stz;
static Stz* P_BreakPt      = "bBreakPt"_Stz;
static Stz* P_BumpedBy     = "nBumpedBy"_Stz;
static Stz* P_Cell         = "i_Cell"_Stz;
static Stz* P_ChosenBound  = "bChosenBound"_Stz;
static Stz* P_ChosenIndex  = "bChosenIndex"_Stz;
static Stz* P_Column       = "bColumn"_Stz;
static Stz* P_Count        = "nCount"_Stz;
static Stz* P_CrlKeyExpr   = "bCrlKeyExpr"_Stz;
static Stz* P_CrlKeyPred   = "bCrlKeyPred"_Stz;
static Stz* P_CrlKeyTest   = "bCrlKeyTest"_Stz;
static Stz* P_CrlRslTest   = "bCrlRslTest"_Stz;
static Stz* P_DebugOption  = "szDebugOption"_Stz;
static Stz* P_Early        = "bEarly"_Stz;
static Stz* P_EmittedCol   = "i_EmittedCol"_Stz;
static Stz* P_FixedPos     = "bFixedPos"_Stz;
static Stz* P_HideHeat     = "bHideHeat"_Stz;
static Stz* P_Injected     = "bInjected"_Stz;
static Stz* P_InjCrlKey    = "bInjCrlKey"_Stz;
static Stz* P_Invalid      = "bInvalid"_Stz;
static Stz* P_JoinedBuff   = "nJoinedBuff"_Stz;
static Stz* P_Key          = "bKey"_Stz;
static Stz* P_Level        = "eLevel"_Stz;
static Stz* P_Network      = "nNetwork"_Stz;
static Stz* P_Payload      = "bPayload"_Stz;
static Stz* P_ParamOdn     = "szParamOdn"_Stz;
static Stz* P_Permutation  = "i_Permutation"_Stz;
static Stz* P_Pointer      = "pProinter"_Stz;
static Stz* P_ProjBump     = "nProjBump"_Stz;
static Stz* P_ProjBumped   = "nProjBumped"_Stz;
static Stz* P_PtndJoin     = "i_PtndJoin"_Stz;
static Stz* P_ReqCol       = "bReqCol"_Stz;
static Stz* P_SendKind     = "eSendKind"_Stz;
static Stz* P_SentOut      = "nSentOut"_Stz;
static Stz* P_Smoothed     = "bSmoothed"_Stz;
static Stz* P_StageTopo    = "eStageTopo"_Stz;
static Stz* P_StmCol       = "szStmCol"_Stz;
static Stz* P_SubQryCall   = "bSubQryCall"_Stz;
static Stz* P_Synthetic    = "bSynthetic"_Stz;
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

static Stz* C_CARD_HISTORY = "<card_history>"_Stz;
static Stz* C_CODE_BLOCK = "<code_block>"_Stz;
static Stz* C_EP_DISABLE = "<ep_disable>"_Stz;
static Stz* C_EP_ENABLE  = "<ep_enable>"_Stz;
static Stz* C_FILTERPRED = "<filterPred>"_Stz;
static Stz* C_JOINORDER    = "<joinOrder>"_Stz;
static Stz* C_JOINPERMUTATIONS = "<joinPermutations>"_Stz;
static Stz* C_JO_DISABLE = "<jo_disable>"_Stz;
static Stz* C_JO_PICK    = "<jo_pick>"_Stz;
static Stz* C_JOWRITE    = "<joWrite>"_Stz;
static Stz* C_OPTIMISE_PASSTHRU = "<optimise_passThru>"_Stz;
static Stz* C_PIPELINE_DEF = "<pipeline_def>"_Stz;
static Stz* C_WEAVED     = "<weaved>"_Stz;
static Stz* C_WEAVER1    = "<weaver1>"_Stz;
static Stz* C_NOCODESYNTHESIS = "<noCodeSynthesis>"_Stz;

static d8 C_EXPANSION_COEFF = 1.60;  // crw -> row buffer
static d8 C_FILTER_COEFF    = 0.30;
static d8 C_JOIN_COEFF      = 2.00;  // bump the larger
static d8 C_JOIN_LONGER     = 1.50;  // joined row gets longer

static char MARK_AT         = '@';
static char MARK_BLANK      = ' ';
static char MARK_CHANGED    = '`';
static char MARK_UP         = '^';

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
    Card(eCK eCard, Stz* szSymbol);
    Card(eCK eCard, Stz* szSymbol, Stz* szPpyNm, i8 iPpy);
    Card(eCK eCard, Stz* szSymbol, Stz* szPpyNm, Stz* szPpy);
    void vInit(eCK eCard);

   ~Card(void) noexcept(false);

    static void vCardLeaks(void);

    static void vConsistencyCheck(Card* p_deck, i8 iTransformNo);

    static char cAggrLevelToChar(eAL eLevel);

    static eYG  eAggrLevelToStageTopology(eAL eLevel);


    static eCK  eTo_eCK(Stz* szCardNm);

    static eAL  eStageTopologyToAggrLevel(eYG eStageTopo);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card* pAppendClone(Card* p_card);
    void vAppendClone(Card* p_card);
    Card* pAppendNewCard(eCK eCard, Stz* szSymbol=nullptr);
    void vAppendNewCard(eCK eCard, Stz* szSymbol=nullptr);
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
    eCT  eColType(void);

    void  vDump(const char* azFunc, i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCard(void);
    Card* pDupCardRecursive(void);
    Card_SubSet* pDupSubCardsRecursive(i8 i_From, i8 i_Till=C_UNKNOWN);

    Card* pFindCardRecursive(eCK eCard);
    Card* pFindParentRecursive(Card* pSubCard);
    Card* pFindSymbolRecursive(Stz* szSymbol, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindSymbolRecursive(eCK eCard, char cSymbol, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindSymbolRecursive(eCK eCard, Stz* szSymbol, Card*& rpParent, Card* _opaque_=nullptr);
    Card* pFindPpyRecursive(Stz* szPpy, eCK eOfCard=CK_NONE);

    eAL  eGetLevel(void);
    Stz* szGetType(void);

    bool bHasSubRecursive(Card* pSubCard);
    bool bHasSubWithSymbolRecursive(eCK eCard, Stz* szSymbol);
    bool bHasSymbolic(void);
    bool bHasType(void);

    bool bIsCard(eCK eCard);
    bool bIsOne(eCK eCard, ...);
    bool bIsSymbol(char cSymbol);
    bool bIsSymbol(Symbol* pSymbol);
    bool bIsSymbol(Stz* szSymbol);


    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    void vMark2(char cMark2);

    void vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent,
                             bool& rbPrune, eAL eLevel,
                             bool bPrintBelow, i8 iXform, Stz* szXfunc);
    void vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm);
    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);
    void vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon, eAL eLevel=AL_NONE);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameSymbol(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintSymbol(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks=false);

    void vRenumberRoot(i8 i_Parent);
    void vRenumberRecursive(Card* pParent);
    void vReplaceSubCard(Card* pSub, Card* pWith);

    bool bSameSymbol(Card* pArg);
    bool bSameSymbolRecursive(Card* pCard_b, i8 iDepth= 0, bool bTestL0= false);
    bool bSameSymbolPpyRecursive(Card* pCard_OTHER); // not needed??

    bool bSameTopology(Card* p_arg);

    void vSetType(Card* p_card_arg);
    void vSetType(Ppy xPpy_type);
    void vSetType(Stz* szType_arg);

    Symbol* pSymbol(void);

    char cSymbol(void);
    i8   iSymbol(void);
    Stz* szSymbol(void);
    void vSymbol(Stz* szSymbol);
    void vSymbol(Symbol* pSymbol);
    void vSymbolMark1(Stz* szSymbol, char cMark1);
    void vSymbolSprint(Stz* szFormat, ...);

    bool bUpdateSymbolRecursive(eCK eSub, Stz* szSymbol, Card_Term* p_term_with);

    void vValidate_mayHave(eCK eCard_first, ...);
    eCK  eValidate_mustEither(eCK eCard_first, ...);
    void vValidate_mustHave(eCK eCard_first, ...);
    void vValidate_notHave(eCK eCard, ...);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

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
    Card_HshCol*        pSub_hshCol(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_Index*         pSub_index(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexNm*       pSub_indexNm(void);
    Card_JoinCast*      pSub_locusCast(void);
    Card_StreamJoin*     pSub_joinLocus(void);
    Card_StreamSet*   pSub_joinPermAll(void);
    Card_StreamPer*   pSub_joinPermList(void);
    Card_Key*           pSub_key(void);
    Card_KeyCells*      pSub_keyCells(void);
    Card_KeyExists*     pSub_keyExists(void);
    Card_KeyNotExists*  pSub_keyNotExists(void);
    Card_Level*         pSub_level(void);
    Card_JoinEstimate*     pSub_joinEstimate(void);
    Card_CostJoining*   pSub_costJoining(void);
    Card_CostNetwork*   pSub_costNetwork(void);
    Card_CostStream*     pSub_costQuery(void);
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
