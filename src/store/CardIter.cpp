/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"
#include "Cue.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardIter,CN_CARDITER)
__aNewFunction(CardIter,CN_CARDITER)
__vDeleteFunction(CardIter,CN_CARDITER)
__aFooterFunction(CardIter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(void)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Card*  pCard)
: Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  _checkInitialised_(CN_CARDITER);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(CardIter* pCardIter_src)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vSet(pCardIter_src);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Bench* pBench)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  // members from Xgearing
  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();

  Job* pJob = MY::pJob();
//  TagSet* pJobTags = pJob->pJobTags();

  m_iXform     = 0;
  m_iTrace     = 0;

  m_pPermGen   = nullptr;

  m_pColumn    = nullptr;
  m_pIdxVec    = nullptr;
  m_pIndex     = nullptr;
  m_pIndexDef  = nullptr;
  m_pStreamDef = nullptr;
  m_pTable     = nullptr;

  m_pCard_child    = nullptr;
  m_pCard_copied   = nullptr;
  m_pCard_dup      = nullptr;
  m_pCard_found    = nullptr;
  m_pCard_located  = nullptr;
  m_pCard_member   = nullptr;
  m_pCard_new      = nullptr;
  m_pCard_parent   = nullptr;
  m_pCard_popped   = nullptr;
  m_pCard_resolved = nullptr;
  m_pCard_temp     = nullptr;
  m_pCard_yanked   = nullptr;


  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard    = nullptr;
  G.pDeck     = nullptr;
  G.pSubQuery = nullptr;
  G_vClearMembers();
  G_vClearRegisters();


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Bench* pBench)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  // members from Xgearing
  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();

  Job* pJob = MY::pJob();
//  TagSet* pJobTags = pJob->pJobTags();

  m_iXform     = 0;
  m_iTrace     = 0;

  m_pPermGen   = nullptr;

  m_pColumn    = nullptr;
  m_pIdxVec    = nullptr;
  m_pIndex     = nullptr;
  m_pIndexDef  = nullptr;
  m_pStreamDef = nullptr;
  m_pTable     = nullptr;

  m_pCard_child    = nullptr;
  m_pCard_copied   = nullptr;
  m_pCard_dup      = nullptr;
  m_pCard_found    = nullptr;
  m_pCard_located  = nullptr;
  m_pCard_member   = nullptr;
  m_pCard_new      = nullptr;
  m_pCard_parent   = nullptr;
  m_pCard_popped   = nullptr;
  m_pCard_resolved = nullptr;
  m_pCard_temp     = nullptr;
  m_pCard_yanked   = nullptr;

  m_pExpr_fold = nullptr;
  m_pExpr_corr = nullptr;

  m_pCorrelation = nullptr;

  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard    = nullptr;
  G.pDeck     = nullptr;
  G.pSubQuery = nullptr;
  m_pTerm_sqCall       = nullptr;


  G_vClearMembers();
  G_vClearRegisters();


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendChild(eCK eParent, eCK eChild)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pParent = G_pGetMember(eParent);
  Card* pChild  = G_pGetMember(eChild);

  pParent->vAppend(pChild);

  if (! bIsTracing())
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pParent = G_pGetMember(eParent);

  pParent->vAppend(pChild);

  if (! bIsTracing())
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vAppend(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild, Stz* szValue)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild, szValue);

  pParent->vAppend(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* CardIter::pBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* setup board iteration                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vBoardIteration(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.i_Board = C_UNKNOWN;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChangeCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_pCard = pCard;

  vRestart();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearMembers(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.p_absJoin          = nullptr;
  G.p_agg              = nullptr;
  G.p_aggreg           = nullptr;
  G.p_as               = nullptr;
  G.p_boardNo          = nullptr;
  G.p_boolCalc         = nullptr;
  G.p_bound            = nullptr;
  G.p_bounds           = nullptr;
  G.p_calcOp           = nullptr;
  G.p_call             = nullptr;
  G.p_cast             = nullptr;
  G.p_clause           = nullptr;
  G.p_cmpPred          = nullptr;
  G.p_colNm            = nullptr;
  G.p_combined         = nullptr;
  G.p_comment          = nullptr;
  G.p_constraint       = nullptr;
  G.p_coPtnJoin        = nullptr;
  G.p_coPtnJoinList    = nullptr;
  G.p_coPtnPred        = nullptr;
  G.p_coPtnPredList    = nullptr;
  G.p_correlation      = nullptr;
  G.p_crossJoin        = nullptr;
  G.p_crossPtnJoin     = nullptr;
  G.p_crossPtnJoinList = nullptr;
  G.p_direction        = nullptr;
  G.p_expr             = nullptr;
  G.p_exprList         = nullptr;
  G.p_filter           = nullptr;
  G.p_flat             = nullptr;
  G.p_fold             = nullptr;
  G.p_foldElist        = nullptr;
  G.p_from             = nullptr;
  G.p_gbyAgg           = nullptr;
  G.p_genesis          = nullptr;
  G.p_group            = nullptr;
  G.p_hash             = nullptr;
  G.p_having           = nullptr;
  G.p_idxCol           = nullptr;
  G.p_idxColList       = nullptr;
  G.p_idxColNm         = nullptr;
  G.p_index            = nullptr;
  G.p_indexJoin        = nullptr;
  G.p_indexList        = nullptr;
  G.p_indexNm          = nullptr;
  G.p_indexScan        = nullptr;
  G.p_joinHead         = nullptr;
  G.p_joinNext         = nullptr;
  G.p_joinOrder        = nullptr;
  G.p_joinOrdering     = nullptr;
  G.p_joinPermAll      = nullptr;
  G.p_joinPermSeq      = nullptr;
  G.p_joinPredList     = nullptr;
  G.p_key              = nullptr;
  G.p_keyExists        = nullptr;
  G.p_keyNotExists     = nullptr;
  G.p_level            = nullptr;
  G.p_limit            = nullptr;
  G.p_load             = nullptr;
  G.p_loadJoined       = nullptr;
  G.p_loadNetwork      = nullptr;
  G.p_loadPermSeq      = nullptr;
  G.p_loadUnicast      = nullptr;
  G.p_manner           = nullptr;
  G.p_nCorrCols        = nullptr;
  G.p_next             = nullptr;
  G.p_note             = nullptr;
  G.p_notPred          = nullptr;
  G.p_nParmCols        = nullptr;
  G.p_on               = nullptr;
  G.p_ordAgg           = nullptr;
  G.p_ordElmOdn        = nullptr;
  G.p_order            = nullptr;
  G.p_outerColNm       = nullptr;
  G.p_outerJoin        = nullptr;
  G.p_partition        = nullptr;
  G.p_permutation      = nullptr;
  G.p_permutation_low  = nullptr;
  G.p_pmzWhere         = nullptr;
  G.p_project          = nullptr;
  G.p_projection       = nullptr;
  G.p_ptnCol           = nullptr;
  G.p_ptnColList       = nullptr;
  G.p_ptnColNm         = nullptr;
  G.p_ptndJoin         = nullptr;
  G.p_ptndJoinSet      = nullptr;
  G.p_reason           = nullptr;
  G.p_relnOp           = nullptr;
  G.p_rpnCalc          = nullptr;
  G.p_scan             = nullptr;
  G.p_section          = nullptr;
  G.p_send             = nullptr;
  G.p_size             = nullptr;
  G.p_sort             = nullptr;
  G.p_stage            = nullptr;
  G.p_stmCol           = nullptr;
  G.p_stmColNm         = nullptr;
  G.p_stmColOdn        = nullptr;
  G.p_subQuery         = nullptr;
  G.p_subSet           = nullptr;
  G.p_table            = nullptr;
  G.p_tableList        = nullptr;
  G.p_tableNm          = nullptr;
  G.p_tablePtn         = nullptr;
  G.p_tblColOdn        = nullptr;
  G.p_term             = nullptr;
  G.p_top              = nullptr;
  G.p_topology         = nullptr;
  G.p_twist            = nullptr;
  G.p_type             = nullptr;
  G.p_unique           = nullptr;
  G.p_where            = nullptr;
  G.p_xBound           = nullptr;

  G.p_av_aggP          = nullptr;
  G.p_av_aggPN         = nullptr;
  G.p_av_aggPNO        = nullptr;
  G.p_av_aggregP       = nullptr;
  G.p_av_aggregPN      = nullptr;
  G.p_av_aggregPNO     = nullptr;
  G.p_av_filter        = nullptr;
  G.p_av_filterProject = nullptr;
  G.p_av_gbyAggP       = nullptr;
  G.p_av_gbyAggPN      = nullptr;
  G.p_av_gbyPNO        = nullptr;
  G.p_av_joinSeries    = nullptr;
  G.p_av_limit         = nullptr;
  G.p_av_project       = nullptr;
  G.p_av_scanJoinSeries = nullptr;
  G.p_av_scanOnly      = nullptr;
  G.p_av_sort          = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearRegisters(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.p_Located          = nullptr;
  G.e_Located          = CK_NONE;
  G.p_PriorToMain      = nullptr;
  G.e_PriorToMain      = CK_NONE;
  G.p_MainCard         = nullptr;
  G.e_MainCard         = CK_NONE;
  G.i_MainCard         = C_UNKNOWN;
  G.p_SubLocated       = nullptr;
  G.e_SubLocated       = CK_NONE;
  G.p_SubLocatedParent = nullptr;
  G.e_SubLocatedParent = CK_NONE;
  G.p_SubOfMain        = nullptr;
  G.e_SubOfMain        = CK_NONE;
  G.p_Sub2OfMain       = nullptr;
  G.e_Sub2OfMain       = CK_NONE;
  G.p_Sub3OfMain       = nullptr;
  G.e_Sub3OfMain       = CK_NONE;
  G.p_SubsequentToMain = nullptr;
  G.e_SubsequentToMain = CK_NONE;

  G.p_SubMember        = nullptr;
  G.e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;
  m_pTerm_2            = nullptr;
  m_pTerm_corrCol     = nullptr;

  G.sz_SubQueryNm      = nullptr;

  G.e_Level = AL_NONE;

  G.i_stmColOdn = C_UNKNOWN;
  G.i_FoldCard = C_UNKNOWN;

  G.p_PRED = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearRegistersIf(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (pCard == G.p_PriorToMain) {
    G.p_PriorToMain = nullptr;
    G.e_PriorToMain = CK_NONE;
  }
  if (pCard == G.p_MainCard) {
    G.p_MainCard = nullptr;
    G.e_MainCard = CK_NONE;
  }
  if (pCard == G.p_SubOfMain) {
    G.p_SubOfMain = nullptr;
    G.e_SubOfMain = CK_NONE;
  }
  if (pCard == G.p_Sub2OfMain) {
    G.p_Sub2OfMain = nullptr;
    G.e_Sub2OfMain = CK_NONE;
  }
  if (pCard == G.p_Sub3OfMain) {
    G.p_Sub3OfMain = nullptr;
    G.e_Sub3OfMain = CK_NONE;
  }
  if (pCard == G.p_SubsequentToMain) {
    G.p_SubsequentToMain = nullptr;
    G.e_SubsequentToMain = CK_NONE;
  }
  if (pCard == G.p_SubMember) {
    G.p_SubMember = nullptr;
    G.e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void CardIter::G_vCopyCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! pCard) {
    m_pCard_copied = nullptr;
    return;
  }

  m_pCard_copied = pCard->pDupCardAndSubCards();
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                   */
/*----------------------------------------------------------------------------*/
eOA CardIter::eCueUser(Stz* szPrompt)
{
  _precondition_(_MAGIC_, _FOOTER_);

  TokenVec* pTknVec = nullptr;

  if (m_pCardCtl->bCueDisabled())
    return OA_CONTINUE_TRANSFORMING;

  szPrompt->vConcat(" >> "_Stz);

  m_pCardCtl->vCued(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_Input:

  s4K sInput;
  Cue::vGetInput(szPrompt,sInput);

  if (0 == sInput.nLength())
    return OA_CONTINUE_TRANSFORMING;

  if (sInput.nLength() > 1)
    vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! pTknVec);
  pTknVec = AM::TOKENISE::pTokeniseSen(sInput);
  i8 nTokens = pTknVec->nTokens();

  if (0 == nTokens)
    return OA_CONTINUE_TRANSFORMING;

  OptimiserFun* pOptFun = nullptr;
  if (1 == nTokens)
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          nullptr);
  else
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          pTknVec->pGet(1)->szChars());
  if (! pOptFun) {
    vTrace("Unknown command\n"_Stz);
    delete pTknVec;
    pTknVec = nullptr;
    goto L_Input;
  }

  eOA eOptAct = OA_CONTINUE_TRANSFORMING;
  bool bExecuted = pOptFun->m_fpOptFun(m_pBench->pOptimiser(),pTknVec,eOptAct);

  delete pTknVec;
  pTknVec = nullptr;

  if (OA_INPUT_AGAIN == eOptAct)
    goto L_Input;

  if (OA_DISABLE_CUEING == eOptAct)
    m_pCardCtl->vCueDisabled(true);

  return eOptAct;
}

/*----------------------------------------------------------------------------*/
/* delete member of given kind; clear-if registers; update M member to null   */
/*----------------------------------------------------------------------------*/
void CardIter::G_vDeleteMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  G_vClearRegistersIf(pMember);
  G_vUpdateMember(eMember, nullptr, __func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete given member; clear-if registers; update M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vDeleteMember(Card* pMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  eCK eMember = pMember->eCard();

  G_vClearRegistersIf(pMember);
  G_vUpdateMember(eMember, nullptr, __func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete main when given sub is empty                                        */
/*----------------------------------------------------------------------------*/
void CardIter::G_vDeleteWhenEmptySub(eCK eMain, eCK eSub)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(this->bJustRestarted());

  if (! G_bEachMainWithSub(eMain,eSub))
    return;

  G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
  G.e_SubOfMain = G.p_SubOfMain->eCard();


  if (G.p_SubOfMain->nChildCards())
    return;

  G_vRemoveDeleteMain(eMain);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vDontDescend(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_bDontDescend = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vDuplicateMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pMember  = G_pGetMember(eMember);

  m_pCard_dup = pMember->pDupCardAndSubCards();

  if (! bIsTracing())
    return;

  vTrace("  %s %s from %s\n"_Stz,
         __func__,m_pCard_dup->szBearing(),pMember->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachBoard(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  if (pEachBoard())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Board* CardIter::pEachBoard(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  i8 i_Board;

  G.pBoard = pEachBoard(i_Board);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Board* CardIter::pEachBoard(i8& ri_Board)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.i_Board)
    G.i_Board = 0;
  else
    G.i_Board++;

  if (G.i_Board >= m_pBench->nBoards()) {
    G.pBoard = nullptr;
    ri_Board  = C_UNKNOWN;
    G.i_Board = 0;

    return nullptr;
  }

  G.pBoard = m_pBench->pGetBoard(G.i_Board);
  ri_Board = G.i_Board;

  vChangeCard(G.pBoard->pDeck());

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (pEachChild())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  G.m_pChild = pEachChild(i_Main);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild = nullptr;
    ri_Main  = C_UNKNOWN;
    G.m_bActive = false;

    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  Card* pMain = pEachChild(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  for (;;) {

    if (G.m_i_Child >= G.m_pCard->nChildCards()) {
      G.m_pChild  = nullptr;
      ri_Main   = C_UNKNOWN;
      G.m_bActive = false;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = G.m_i_Child;
      return G.m_pChild;
    }
    G.m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild  = nullptr;
    ri_Main   = C_UNKNOWN;
    reCard    = CK_NONE;
    G.m_bActive = false;
    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

  reCard = G.m_pChild->eCard();
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_foldBreak(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  G.m_pChild = pEachChild(i_Main);

  if (G.m_pChild->bIsCard(CK_FOLD)) {
    G.m_pChild = nullptr;
    G.m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_foldBreak(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  for (;;) {

    if (G.m_i_Child >= G.m_pCard->nChildCards()) {
      G.m_pChild = nullptr;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    if (G.m_pChild->bIsCard(CK_FOLD)) {
      G.m_pChild = nullptr;
      G.m_i_Child = C_UNKNOWN;
      return nullptr;
    }


    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G.m_pChild;
    }

    G.m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachChild_reverse(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (pEachChild_reverse())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  G.m_pChild = pEachChild_reverse(i_Main);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = G.m_pCard->nChildCards() - 1;
  else
    G.m_i_Child--;

  if (G.m_i_Child < 0) {
    G.m_pChild = nullptr;
    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

  ri_Main = G.m_i_Child;

  return G.m_pChild;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = G.m_pCard->nChildCards() - 1;
  else
    G.m_i_Child--;

  for (;;) {

    if (G.m_i_Child < 0) {
      G.m_pChild = nullptr;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G.m_pChild;
    }

    G.m_i_Child--;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse_foldBreak(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = G.m_pCard->nChildCards() - 1;
  else
    G.m_i_Child--;

  for (;;) {

    if (G.m_i_Child < 0) {
      G.m_pChild = nullptr;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    if (G.m_pChild->bIsCard(CK_FOLD)) {
      G.m_pChild = nullptr;
      G.m_i_Child = C_UNKNOWN;
      return nullptr;
    }


      eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G.m_pChild;
    }

    G.m_i_Child--;
  }

  NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild())) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_foldBreak(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild_foldBreak())) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_foldBreak(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild_foldBreak(eOfMainKind))) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild_reverse(eOfMainKind))) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse_foldBreak(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pEachChild_reverse_foldBreak(eOfMainKind))) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild_reverse(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub_reverse_foldBreak(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild_reverse_foldBreak(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  // did not find card
  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMainWithSub(eOfMainKind, eAndSubKind)) {
    if (G.p_SubOfMain->szValue()->bSame(szSubValue))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWith2Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWith3Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2,
                                   eCK eAndSubKind3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;
    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eAndSubKind3)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vUpdateMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachSubOfMain(eCK eSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eSubKind3)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vUpdateMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vUpdateMember(eSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachSubQuery(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  if (pEachSubQuery())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* CardIter::pEachSubQuery(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  i8 i_SubQuery;

  G.pSubQuery = pEachSubQuery(i_SubQuery);

  return G.pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* CardIter::pEachSubQuery(i8& ri_SubQuery)
{
  _precondition_( _MAGIC_, _FOOTER_);

  //RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.i_SubQuery)
    G.i_SubQuery = 0;
  else
    G.i_SubQuery++;

  if (G.i_SubQuery >= m_pBench->nBoards()) {
    G.pSubQuery = nullptr;
    ri_SubQuery  = C_UNKNOWN;
    G.i_SubQuery = 0;

    return nullptr;
  }

  Board* pSubBoard = m_pBench->pGetBoard(G.i_SubQuery);
  G.pSubQuery = pSubBoard->pSubQuery();
  ri_SubQuery = G.i_SubQuery;

  return G.pSubQuery;
}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vEndTransform(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.pDeck->vRenumberRecursive();

  //m_pCardCtl->vUseXform();
  // UNDO: Card::vConsistencyCheck(m_pDeck, m_iXform);

  vPrintBoard();

  //m_pCardCtl->vUseDefault();

}

/*----------------------------------------------------------------------------*/
/* find sub of member with given property                                     */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! (m_pCard_member = G_pGetMember(eMember))) {
    m_pCard_found = nullptr;
    return false;
  }

  if (! (m_pCard_found = m_pCard_member->pFindPpyRecursive(szPpy)))
    return false;

  G_vUpdateMember(m_pCard_found->eCard(), m_pCard_found, __func__);
  return true;
}

/*----------------------------------------------------------------------------*/
/* focus on main                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vFocusOnMain(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.e_MainCard = G.p_MainCard->eCard();
  G.i_MainCard = this->i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* CardIter::G_pGetMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return G.p_absJoin;
    case CK_AGG:              return G.p_agg;
    case CK_AGGREG:           return G.p_aggreg;
    case CK_AS:               return G.p_as;
    case CK_BOARDNO:          return G.p_boardNo;
    case CK_BOOLCALC:         return G.p_boolCalc;
    case CK_BOUND:            return G.p_bound;
    case CK_BOUNDS:           return G.p_bounds;
    case CK_CALCOP:           return G.p_calcOp;
    case CK_CALL:             return G.p_call;
    case CK_CAST:             return G.p_cast;
    case CK_CLAUSE:           return G.p_clause;
    case CK_CMPPRED:          return G.p_cmpPred;
    case CK_COMMENT:          return G.p_comment;
    case CK_COLNM:            return G.p_colNm;
    case CK_COMBINED:         return G.p_combined;
    case CK_CONSTRAINT:       return G.p_constraint;
    case CK_COPTNJOIN:        return G.p_coPtnJoin;
    case CK_COPTNJOINLIST:    return G.p_coPtnJoinList;
    case CK_COPTNPRED:        return G.p_coPtnPred;
    case CK_COPTNPREDLIST:    return G.p_coPtnPredList;
    case CK_CORRELATION:      return G.p_correlation;
    case CK_CROSSJOIN:        return G.p_crossJoin;
    case CK_CROSSPTNJOIN:     return G.p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return G.p_crossPtnJoinList;
    case CK_DIRECTION:        return G.p_direction;
    case CK_EXPR:             return G.p_expr;
    case CK_EXPRLIST:         return G.p_exprList;
    case CK_FILTER:           return G.p_filter;
    case CK_FLAT:             return G.p_flat;
    case CK_FOLD:             return G.p_fold;
    case CK_FOLDELIST:        return G.p_foldElist;
    case CK_FROM:             return G.p_from;
    case CK_GBYAGG:           return G.p_gbyAgg;
    case CK_GENESIS:          return G.p_genesis;
    case CK_GROUP:            return G.p_group;
    case CK_HASH:             return G.p_hash;
    case CK_HAVING:           return G.p_having;
    case CK_IDXCOL:           return G.p_idxCol;
    case CK_IDXCOLLIST:       return G.p_idxColList;
    case CK_IDXCOLNM:         return G.p_idxColNm;
    case CK_INDEX:            return G.p_index;
    case CK_INDEXJOIN:        return G.p_indexJoin;
    case CK_INDEXLIST:        return G.p_indexList;
    case CK_INDEXNM:          return G.p_indexNm;
    case CK_INDEXSCAN:        return G.p_indexScan;
    case CK_JOINHEAD:         return G.p_joinHead;
    case CK_JOINNEXT:         return G.p_joinNext;
    case CK_JOINORDER:        return G.p_joinOrder;
    case CK_JOINORDERING:     return G.p_joinOrdering;
    case CK_JOINPERMALL:      return G.p_joinPermAll;
    case CK_JOINPERMSEQ:      return G.p_joinPermSeq;
    case CK_JOINPREDLIST:     return G.p_joinPredList;
    case CK_KEY:              return G.p_key;
    case CK_KEYEXISTS:        return G.p_keyExists;
    case CK_KEYNOTEXISTS:     return G.p_keyNotExists;
    case CK_LEVEL:            return G.p_level;
    case CK_LIMIT:            return G.p_limit;
    case CK_LOAD:             return G.p_load;
    case CK_LOADJOINED:       return G.p_loadJoined;
    case CK_LOADNETWORK:      return G.p_loadNetwork;
    case CK_LOADPERMSEQ:      return G.p_loadPermSeq;
    case CK_LOADUNICAST:      return G.p_loadUnicast;
    case CK_MANNER:           return G.p_manner;
    case CK_NCORRCOLS:        return G.p_nCorrCols;
    case CK_NEXT:             return G.p_next;
    case CK_NOTE:             return G.p_note;
    case CK_NOTPRED:          return G.p_notPred;
    case CK_NPARMCOLS:        return G.p_nParmCols;
    case CK_ON:               return G.p_on;
    case CK_ORDAGG:           return G.p_ordAgg;
    case CK_ORDELMODN:        return G.p_ordElmOdn;
    case CK_ORDER:            return G.p_order;
    case CK_OUTERCOLNM:       return G.p_outerColNm;
    case CK_OUTERJOIN:        return G.p_outerJoin;
    case CK_PARTITION:        return G.p_partition;
    case CK_PERMUTATION:      return G.p_permutation;
    case CK_PMZWHERE:         return G.p_pmzWhere;
    case CK_PROJECT:          return G.p_project;
    case CK_PROJECTION:       return G.p_projection;
    case CK_PTNCOL:           return G.p_ptnCol;
    case CK_PTNCOLLIST:       return G.p_ptnColList;
    case CK_PTNCOLNM:         return G.p_ptnColNm;
    case CK_PTNDJOIN:         return G.p_ptndJoin;
    case CK_PTNDJOINSET:      return G.p_ptndJoinSet;
    case CK_REASON:           return G.p_reason;
    case CK_RELNOP:           return G.p_relnOp;
    case CK_RPNCALC:          return G.p_rpnCalc;
    case CK_SCAN:             return G.p_scan;
    case CK_SECTION:          return G.p_section;
    case CK_SEND:             return G.p_send;
    case CK_SIZE:             return G.p_size;
    case CK_SORT:             return G.p_sort;
    case CK_STAGE:            return G.p_stage;
    case CK_STMCOL:           return G.p_stmCol;
    case CK_STMCOLNM:         return G.p_stmColNm;
    case CK_STMCOLODN:        return G.p_stmColOdn;
    case CK_SUBQUERY:         return G.p_subQuery;
    case CK_SUBSET:           return G.p_subSet;
    case CK_TABLE:            return G.p_table;
    case CK_TABLELIST:        return G.p_tableList;
    case CK_TABLENM:          return G.p_tableNm;
    case CK_TABLEPTN:         return G.p_tablePtn;
    case CK_TBLCOLODN:        return G.p_tblColOdn;
    case CK_TERM:             return G.p_term;
    case CK_TOP:              return G.p_top;
    case CK_TOPOLOGY:         return G.p_topology;
    case CK_TWIST:            return G.p_twist;
    case CK_TYPE:             return G.p_type;
    case CK_UNIQUE:           return G.p_unique;
    case CK_WHERE:            return G.p_where;
    case CK_XBOUND:           return G.p_xBound;

    case CK_AV_AGGP:          return G.p_av_aggP;
    case CK_AV_AGGPN:         return G.p_av_aggPN;
    case CK_AV_AGGPNO:        return G.p_av_aggPNO;
    case CK_AV_AGGREGP:       return G.p_av_aggregP;
    case CK_AV_AGGREGPN:      return G.p_av_aggregPN;
    case CK_AV_AGGREGPNO:     return G.p_av_aggregPNO;
    case CK_AV_FILTER:        return G.p_av_filter;
    case CK_AV_FILTERPROJECT: return G.p_av_filterProject;
    case CK_AV_GBYAGGP:       return G.p_av_gbyAggP;
    case CK_AV_GBYAGGPN:      return G.p_av_gbyAggPN;
    case CK_AV_GBYPNO:        return G.p_av_gbyPNO;
    case CK_AV_JOINSERIES:    return G.p_av_joinSeries;
    case CK_AV_LIMIT:         return G.p_av_limit;
    case CK_AV_PROJECT:       return G.p_av_project;
    case CK_AV_SCANJOINSERIES: return G.p_av_scanJoinSeries;
    case CK_AV_SCANONLY:      return G.p_av_scanOnly;
    case CK_AV_SORT:          return G.p_av_sort;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main card?                      */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bHasPriorToMain(bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_PriorToMain = this->pPrevChild())) {
    G.e_PriorToMain = G.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_PriorToMain, G.p_PriorToMain, __func__);
    return true;
  }

  G.e_PriorToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main which is of given kind?    */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_PriorToMain = this->pPrevChildIf(eOfPriorKind))) {
    G.e_PriorToMain = G.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_PriorToMain, G.p_PriorToMain, __func__);
    return true;
  }

  G.e_PriorToMain = CK_NONE;
  if (bUpdateMember)
    G_vUpdateMember(eOfPriorKind, nullptr, __func__);

  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bHasSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChild())) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main which is of given kind?     */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChildIf(eOfSubsequentKind))) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  if (bUpdateMember)
    G_vUpdateMember(eOfSubsequentKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vInsertAfter(Card* pCard, Card* pAfter)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pDeck->vInsertAfter(pCard,pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pDeck->vInsertBefore(pCard,pBefore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bIsBoard(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (m_pBench->pGetBoard(szBoardNm))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bIsNextMain(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (this->pNextChildIf(eMember))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child &&
      nullptr == G.m_pChild &&
      true == G.m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bLess(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bLocateCard(eCK eLocate, i8 i_After, i8& ri_Located)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 nChildren = G.pDeck->nChildCards();
  for (i8 i_Main = i_After + 1; i_Main < nChildren; i_Main++) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    if (pMain->bIsCard(eLocate)) {
      G.p_Located = pMain;
      G.e_Located = eLocate;
      ri_Located = i_Main;
      return true;
    }
  }

  G.p_Located = nullptr;
  G.e_Located = CK_NONE;
  ri_Located = C_UNKNOWN;
  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bLocateCard(eCK eLocate, Stz* szWithValue, i8 i_After)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 nChildren = G.pDeck->nChildCards();
  for (i8 i_Main = i_After + 1; i_Main < nChildren; i_Main++) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    G.p_Located = pMain->pFindValueRecursive(eLocate,szWithValue,G.p_Located);
    if (!G.p_Located)
      continue;

    G.e_Located = eLocate;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bLocateCard(eCK eLocate, eCK eSubLocate, Stz* szWithValue, i8 i_After)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 nChildren = G.pDeck->nChildCards();
  for (i8 i_Main = i_After + 1; i_Main < nChildren; i_Main++) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    G.p_Located = pMain->pFindCardRecursive(eLocate);
    if (! G.p_Located)
      continue;

    G.e_Located = eLocate;

    G.p_SubLocated = G.p_Located->pFindValueRecursive(eSubLocate,szWithValue,G.p_SubLocatedParent);
    if (G.p_SubLocated) {
      G.e_SubLocated = eSubLocate;
      G.e_SubLocatedParent = G.p_SubLocatedParent ? G.p_SubLocatedParent->eCard() : CK_NONE;
      return true;
    }

  }

  G.p_Located = nullptr;
  G.e_Located = CK_NONE;
  G.p_SubLocated = nullptr;
  G.e_SubLocated = CK_NONE;
  G.p_SubLocatedParent = nullptr;
  G.e_SubLocatedParent = CK_NONE;

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given card                                  */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter)
{
  _precondition_(_MAGIC_, _FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szMemberNm(eCK eMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return "G.p_absJoin"_Stz;
    case CK_AGG:              return "G.p_agg"_Stz;
    case CK_AGGREG:           return "G.p_aggreg"_Stz;
    case CK_AS:               return "G.p_as"_Stz;
    case CK_BOARDNO:          return "G.p_boardNo"_Stz;
    case CK_BOOLCALC:         return "G.p_boolCalc"_Stz;
    case CK_BOUND:            return "G.p_bound"_Stz;
    case CK_BOUNDS:           return "G.p_bounds"_Stz;
    case CK_CALCOP:           return "G.p_calcOp"_Stz;
    case CK_CALL:             return "G.p_call"_Stz;
    case CK_CAST:             return "G.p_cast"_Stz;
    case CK_CLAUSE:           return "G.p_clause"_Stz;
    case CK_CMPPRED:          return "G.p_cmpPred"_Stz;
    case CK_COLNM:            return "G.p_colNm"_Stz;
    case CK_COMBINED:         return "G.p_combined"_Stz;
    case CK_COMMENT:          return "G.p_comment"_Stz;
    case CK_CONSTRAINT:       return "G.p_constraint"_Stz;
    case CK_COPTNJOIN:        return "G.p_coPtnJoin"_Stz;
    case CK_COPTNJOINLIST:    return "G.p_coPtnJoinList"_Stz;
    case CK_COPTNPRED:        return "G.p_coPtnPred"_Stz;
    case CK_COPTNPREDLIST:    return "G.p_coPtnPredList"_Stz;
    case CK_CORRELATION:      return "G.p_correlation"_Stz;
    case CK_CROSSJOIN:        return "G.p_crossJoin"_Stz;
    case CK_CROSSPTNJOIN:     return "G.p_crossPtnJoin"_Stz;
    case CK_CROSSPTNJOINLIST: return "G.p_crossPtnJoinList"_Stz;
    case CK_DIRECTION:        return "G.p_direction"_Stz;
    case CK_EXPR:             return "G.p_expr"_Stz;
    case CK_EXPRLIST:         return "G.p_exprList"_Stz;
    case CK_FILTER:           return "G.p_filter"_Stz;
    case CK_FLAT:             return "G.p_flat"_Stz;
    case CK_FOLD:             return "G.p_fold"_Stz;
    case CK_FOLDELIST:        return "G.p_foldElist"_Stz;
    case CK_FROM:             return "G.p_from"_Stz;
    case CK_GBYAGG:           return "G.p_gbyAgg"_Stz;
    case CK_GENESIS:          return "G.p_genesis"_Stz;
    case CK_GROUP:            return "G.p_group"_Stz;
    case CK_HASH:             return "G.p_hash"_Stz;
    case CK_HAVING:           return "G.p_having"_Stz;
    case CK_IDXCOLLIST:       return "G.p_idxColList"_Stz;
    case CK_IDXCOLNM:         return "G.p_idxColNm"_Stz;
    case CK_INDEX:            return "G.p_index"_Stz;
    case CK_INDEXJOIN:        return "G.p_indexJoin"_Stz;
    case CK_INDEXLIST:        return "G.p_indexList"_Stz;
    case CK_INDEXNM:          return "G.p_indexNm"_Stz;
    case CK_INDEXSCAN:        return "G.p_indexScan"_Stz;
    case CK_JOINHEAD:         return "G.p_joinHead"_Stz;
    case CK_JOINNEXT:         return "G.p_joinNext"_Stz;
    case CK_JOINORDER:        return "G.p_joinOrder"_Stz;
    case CK_JOINORDERING:     return "G.p_joinOrdering"_Stz;
    case CK_JOINPERMALL:      return "G.p_joinPermAll"_Stz;
    case CK_JOINPERMSEQ:      return "G.p_joinPermSeq"_Stz;
    case CK_JOINPREDLIST:     return "G.p_joinPredList"_Stz;
    case CK_KEY:              return "G.p_key"_Stz;
    case CK_KEYEXISTS:        return "G.p_keyExists"_Stz;
    case CK_KEYNOTEXISTS:     return "G.p_keyNotExists"_Stz;
    case CK_LEVEL:            return "G.p_level"_Stz;
    case CK_LIMIT:            return "G.p_limit"_Stz;
    case CK_LOAD:             return "G.p_load"_Stz;
    case CK_LOADJOINED:       return "G.p_loadJoined"_Stz;
    case CK_LOADNETWORK:      return "G.p_loadNetwork"_Stz;
    case CK_LOADPERMSEQ:      return "G.p_loadPermSeq"_Stz;
    case CK_LOADUNICAST:      return "G.p_loadUnicast"_Stz;
    case CK_MANNER:           return "G.p_manner"_Stz;
    case CK_NCORRCOLS:        return "G.p_nCorrCols"_Stz;
    case CK_NEXT:             return "G.p_next"_Stz;
    case CK_NOTE:             return "G.p_note"_Stz;
    case CK_NOTPRED:          return "G.p_notPred"_Stz;
    case CK_NPARMCOLS:        return "G.p_nParmCols"_Stz;
    case CK_ON:               return "G.p_on"_Stz;
    case CK_ORDAGG:           return "G.p_ordAgg"_Stz;
    case CK_ORDELMODN:        return "G.p_ordElmOdn"_Stz;
    case CK_ORDER:            return "G.p_order"_Stz;
    case CK_OUTERCOLNM:       return "G.p_outerColNm"_Stz;
    case CK_OUTERJOIN:        return "G.p_outerJoin"_Stz;
    case CK_PARTITION:        return "G.p_partition"_Stz;
    case CK_PERMUTATION:      return "G.p_permutation"_Stz;
    case CK_PMZWHERE:         return "G.p_pmzWhere"_Stz;
    case CK_PROJECT:          return "G.p_project"_Stz;
    case CK_PROJECTION:       return "G.p_projection"_Stz;
    case CK_PTNCOL:           return "G.p_ptnCol"_Stz;
    case CK_PTNCOLLIST:       return "G.p_ptnColList"_Stz;
    case CK_PTNCOLNM:         return "G.p_ptnColNm"_Stz;
    case CK_PTNDJOIN:         return "G.p_ptndJoin"_Stz;
    case CK_PTNDJOINSET:      return "G.p_ptndJoinSet"_Stz;
    case CK_REASON:           return "G.p_reason"_Stz;
    case CK_RELNOP:           return "G.p_relnOp"_Stz;
    case CK_RPNCALC:          return "G.p_rpnCalc"_Stz;
    case CK_SCAN:             return "G.p_scan"_Stz;
    case CK_SECTION:          return "G.p_section"_Stz;
    case CK_SEND:             return "G.p_send"_Stz;
    case CK_SIZE:             return "G.p_size"_Stz;
    case CK_SORT:             return "G.p_sort"_Stz;
    case CK_STAGE:            return "G.p_stage"_Stz;
    case CK_STMCOL:           return "G.p_stmCol"_Stz;
    case CK_STMCOLNM:         return "G.p_stmColNm"_Stz;
    case CK_STMCOLODN:        return "G.p_stmColOdn"_Stz;
    case CK_SUBQUERY:         return "G.p_subQuery"_Stz;
    case CK_SUBSET:           return "G.p_subSet"_Stz;
    case CK_TABLE:            return "G.p_table"_Stz;
    case CK_TABLELIST:        return "G.p_tableList"_Stz;
    case CK_TABLENM:          return "G.p_tableNm"_Stz;
    case CK_TABLEPTN:         return "G.p_tablePtn"_Stz;
    case CK_TBLCOLODN:        return "G.p_tblColOdn"_Stz;
    case CK_TERM:             return "G.p_term"_Stz;
    case CK_TOP:              return "G.p_top"_Stz;
    case CK_TOPOLOGY:         return "G.p_topology"_Stz;
    case CK_TWIST:            return "G.p_twist"_Stz;
    case CK_TYPE:             return "G.p_type"_Stz;
    case CK_UNIQUE:           return "G.p_unique"_Stz;
    case CK_WHERE:            return "G.p_where"_Stz;
    case CK_XBOUND:           return "G.p_xBound"_Stz;

    case CK_AV_AGGP:          return "G.p_av_aggP"_Stz;
    case CK_AV_AGGPN:         return "G.p_av_aggPN"_Stz;
    case CK_AV_AGGPNO:        return "G.p_av_aggPNO"_Stz;
    case CK_AV_AGGREGP:       return "G.p_av_aggregP"_Stz;
    case CK_AV_AGGREGPN:      return "G.p_av_aggregPN"_Stz;
    case CK_AV_AGGREGPNO:     return "G.p_av_aggregPNO"_Stz;
    case CK_AV_FILTER:        return "G.p_av_filter"_Stz;
    case CK_AV_FILTERPROJECT: return "G.p_av_filterProject"_Stz;
    case CK_AV_GBYAGGP:       return "G.p_av_gbyAggP"_Stz;
    case CK_AV_GBYAGGPN:      return "G.p_av_gbyAggPN"_Stz;
    case CK_AV_GBYPNO:        return "G.p_av_gbyPNO"_Stz;
    case CK_AV_JOINSERIES:    return "G.p_av_joinSeries"_Stz;
    case CK_AV_LIMIT:         return "G.p_av_limit"_Stz;
    case CK_AV_PROJECT:       return "G.p_av_project"_Stz;
    case CK_AV_SCANJOINSERIES: return "G.p_av_scanJoinSeries"_Stz;
    case CK_AV_SCANONLY:      return "G.p_av_scanOnly"_Stz;
    case CK_AV_SORT:          return "G.p_av_sort"_Stz;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child + 1 < G.m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* are there more main cards?                                                 */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bMoreMain(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (this->bMore())
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* morph card                                                                 */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMorphCard(Card* pCard, eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  pCard->vCard(eCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Next = G.m_i_Child + 1;

  if (i_Next >= G.m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = G.m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  m_pCard_new = new Card(eMember);

  if (bUpdateMember)
    G_vUpdateMember(eMember, m_pCard_new, __func__);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMemberBefore(eCK eMember, Stz* szValue,
                                  eCK eBefore, bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);

  Card* p_before = G_pGetMember(eBefore);

  G.pDeck->vInsertBefore(m_pCard_new,p_before);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMemberOf(eCK eMember, Stz* szValue,
                              eCK eOf, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);

  G_vAppendChild(eOf,eMember);
}

/*----------------------------------------------------------------------------*/
/* search for sub of member                                                   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  bool b = G_bPlumbForSubMember(pMember, eSubCard, bUpdateMember);

  return b;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_SubMember = pMember->pFindSubCard(eSubCard))) {
    G.e_SubMember = G.p_SubMember->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubMember, G.p_SubMember, __func__);
    return true;
  }

  if (bUpdateMember)
    G_vUpdateMember(eSubCard, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPlumbForSubValue(eCK eMember, eCK eSub, Stz* szValue, bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  bool b = G_bPlumbForSubValue(pMember, eSub, szValue, bUpdateMember);

  return b;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPlumbForSubValue(Card* pMember, eCK eSub, Stz* szValue, bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_SubMember = pMember->pFindSubCard(eSub))) {
    if (G.p_SubMember->bSame(szValue)) {
      G.e_SubMember = G.p_SubMember->eCard();
      if (bUpdateMember)
        G_vUpdateMember(G.e_SubMember, G.p_SubMember, __func__);
      return true;
    }
  }

  if (bUpdateMember)
    G_vUpdateMember(eSub, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPopIter(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  this->vRestore();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Prev = G.m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = G.m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/* print all boards of bench                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vPrintBench(eBK eBoard, i8 i_Board)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! m_pCardCtl->bBenchPrint())
    return;

  char cPrefix = m_pBench->cPrefix();

  s256 sPrompt;

  i8 nBoards = m_pBench->nItems();

  for (;;) {
    if (C_UNKNOWN != i_Board) {
      Board *pBoard = m_pBench->pGetBoard(eBoard, i_Board);
      pBoard->vPrintBoard(szXhand());
    }
    else {
      for (i8 i_Each = 0; m_pBench->bBoardExists(eBoard, i_Each); i_Each++) {
        Board *pBoard = m_pBench->pGetBoard(eBoard, i_Each);
        pBoard->vPrintBoard(szXhand());
      }
    }

    if (! m_pCardCtl->bCueUser(szXhand(), C_NA)) {
      m_pCardCtl->vFormPrompt(sPrompt,
                              cPrefix, szXhand(), C_NA,
                              i_Board, nBoards, C_UNKNOWN);
      printf("%s\n", sPrompt.azChar());
      break;
    }

    m_pCardCtl->vFormPrompt(sPrompt,
                            cPrefix, szXhand(), C_NA,
                            i_Board, nBoards, C_UNKNOWN);
    if (OA_REPRINT_BOARD != eCueUser(sPrompt))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPrintBoard(i8 iLineNo)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (! m_pCardCtl->bBoardPrint())
    return;

  char cPrefix = m_pBench->cPrefix();

  s256 sPrompt;

  i8 nBoards = m_pBench->nItems();

  for (;;) {
    G.pBoard->vPrintBoard(szXdesc());

    if (! m_pCardCtl->bCueUser(szXhand(), szXform())) {
      m_pCardCtl->vFormPrompt(sPrompt,
                              cPrefix, szXhand(), szXform(),
                              G.pBoard->i_Board(), nBoards, iLineNo);
      printf("%s\n", sPrompt.azChar());
      break;
    }

    m_pCardCtl->vFormPrompt(sPrompt,
                            cPrefix, szXhand(), szXform(),
                            G.pBoard->i_Board(), nBoards, iLineNo);
    if (OA_REPRINT_BOARD != eCueUser(sPrompt))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushBoard(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(szBoardNm);

  G_vPushBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushBoard(Board* pBoard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vPushIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vSetBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/* push board                                                                 */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(i8 i_Board)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(i_Board);
  RASSERT(pBoard);

  if (pBoard) {
    G_vPushBoard(pBoard);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pSubQueryBoard = m_pBench->pGetBoard(szBoardNm);
  if (! pSubQueryBoard) {
    vTracer("DID NOT FIND Board: %s\n"_Stz,szBoardNm);
    return false;
  }

  vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

  G_vPushBoard(pSubQueryBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vPush(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushNewChild(Card* pParent, eCK eChild, Stz* szValue)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild, szValue);

  pParent->vPush(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushIter(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

//  Xsentinals* pTransformIterSave = new Xsentinals;
//
//  pTransformIterSave->vCopyFrom(this->aXsentinels());
//
//  m_pTransformIterList->vPush(pTransformIterSave);

  this->vSave();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (G.m_pChild && (! G.m_bDontDescend) ) {
    if (G.m_pChild->nChildCards()) {

      G.m_bRecursed = true;
      vPushRecurseIter();
      vChangeCard(G.m_pChild);
      G.m_pChild = pEachChild();

      return G.m_pChild;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachChild())
    return G.m_pChild;

  while (bPopRecurseIter()) {
    if (bEachChild())
      return G.m_pChild;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (Card* p_card = pRecurseEach())
    if (p_card->bIsCard(eCard))
      return p_card;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (pChild == G.m_pChild)
    vRemoveCurr(G.m_pChild->eCard());
  else
    G.m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  vRemoveChild(rpChild);

  delete rpChild;

  rpChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  delete G.m_pChild;
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck and delete it; clear M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  this->vRemoveDeleteCurr(eCrossCheck);

  G.p_MainCard = nullptr;
  G.e_MainCard = CK_NONE;

  G_vUpdateMember(eCrossCheck, nullptr, __func__);
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vRemoveMain(eCK eCrossCheck)
{
  _precondition_( _MAGIC_, _FOOTER_);

  this->vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCard(eCK eCard, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = G.pDeck->nChildCards();

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if (pMain->bIsCard(eCard)) {
      G_vUpdateMember(eCard, pMain, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCard(eCK eCard, char cWithValue, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eCard,cWithValue,m_pCard_parent))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eCard,szWithValue,m_pCard_parent))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left) before given card                                */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 i_Before = G.pDeck->iFindChild(pBefore);
  if (C_UNKNOWN == i_Before)
    return false;

  bool b = G_bResolveCard(eCard, szWithValue, i_Before);

  return b;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    G.p_MainCard = G.pDeck->pGetChild(i_Main);

    if (! G.p_MainCard->bIsCard(eOfMainKind))
      continue;

    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    G.e_MainCard = G.p_MainCard->eCard();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 i_Before = G.pDeck->iFindChild(pBefore);
  if (C_UNKNOWN == i_Before)
    return false;

  bool b = G_bResolveCardWithSub(eOfMainKind, eAndSubKind, i_Before);

  return b;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with grand-sub with given value before given i_Before   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveSubCard(eCK eGrandSub, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eGrandSub,szWithValue,m_pCard_parent))) {

      G_vUpdateMember(m_pCard_resolved->eCard(), m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with grand-sub with given value before given i_Before   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveSubCard(eCK eCard, eCK eGrandSub, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eGrandSub,szWithValue,m_pCard_parent))) {

      if ((m_pCard_resolved = pMain->pFindParent(m_pCard_resolved))) {
        G_vUpdateMember(m_pCard_resolved->eCard(), m_pCard_resolved, __func__);
        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with sub given value & has eGranSun before i_Before     */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveSubCard(eCK eSub, Stz* szWithValue, eCK eGrandSub, i8 i_Before)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ( !(m_pCard_resolved = pMain->pFindValueRecursive(eSub,szWithValue,m_pCard_parent)))
      continue;

    if ( !m_pCard_resolved->pFindSubCard(eGrandSub))
      continue;

    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with grand-sub with given value before given i_Before   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bResolveSubCard(eCK eCard, i8 i_Before) {
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindSubCard(eCard))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestart(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;
  G.m_bActive = true;
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void CardIter::G_vRestart(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  this->vRestart();

  G_vClearMembers();
  G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child == 0) {
    G.m_i_Child = C_UNKNOWN;
    G.m_pChild = nullptr;
    return;
  }

  G.m_i_Child--;

  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild =  G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRewindChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (G.m_i_Child == 0) {
    G.m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  G.m_i_Child--;

  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild =  G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bRewindMain(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pRewindChild())) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestore(void)
{
  CardIterMembers* pRestore = pPop();

  G.vCopyFrom(pRestore);

  delete pRestore;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSave(void)
{
  CardIterMembers* pSave = new CardIterMembers;

  pSave->vCopyFrom(&G);

  vPush(pSave);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::iSentinalsDepth(void)
{

  return this->nItems();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  G.m_i_Child = G.m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_pChild = nullptr;
  else
    G.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCardIter_src->G.m_pCard;
  G.m_i_Child = pCardIter_src->G.m_i_Child;
  G.m_pChild  = pCardIter_src->G.m_pChild;

  G.m_bActive = pCardIter_src->G.m_bActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  G.m_i_Child = iChild;
  G.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vSetBoard(Board* pBoard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();
  //G.pSubQuery = G.pBoard->pSubQuery();

  this->vChangeCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void CardIter::G_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_( _MAGIC_, _FOOTER_);

  pEachChild();

  if (bClearMembers)
    G_vClearMembers();

  if (bClearRegisters)
    G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* setup sub-query iteration                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vSubQueryIteration(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.i_SubQuery = C_UNKNOWN;
}


/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bSubsequentToMain(bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChild())) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
void CardIter::G_vSubsequentToMain(bool bUpdateMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  bool b = G_bSubsequentToMain(bUpdateMember);
  UNUSED(b);
}

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
bool CardIter::bIsTracing(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return m_pCardCtl->bTracing(szXhand(),szXform());
}

/*----------------------------------------------------------------------------*/
/* conditionally trace                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vTracer(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! bIsTracing())
    return;

  s256 sValue;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sValue.azChar(),xArgs,(char*)szFormat);
  va_end(xArgs);

  vTrace("  [%02lld] %s"_Stz,m_iTrace++,sValue.szStz());
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void CardIter::vTransform(const char* azFunc, Stz* szXdesc)
{
  _precondition_( _MAGIC_, _FOOTER_);

  vXform((Stz*)azFunc);
  G.pBoard->vXform((Stz*)azFunc);
  vXdesc(szXdesc);

  m_iXform++;
  G.pBoard->vXform(m_iXform);

  //m_pCardCtl->vUseXform();

  this->vRestart();

  if (m_pCardCtl->bTracing(szXhand(), szXform())) {
    vTrace("\n\nTracing %s::%s\n"_Stz, szXhand(), szXform());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::iTransformDepth(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return iSentinalsDepth();
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
void CardIter::G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(!pCard || pCard->bIsCard(eMember));

  switch (eMember) {

    case CK_ABSJOIN:          *((Card**)&G.p_absJoin)          = pCard; break;
    case CK_AGG:              *((Card**)&G.p_agg)              = pCard; break;
    case CK_AGGREG:           *((Card**)&G.p_aggreg)           = pCard; break;
    case CK_AS:               *((Card**)&G.p_as)               = pCard; break;
    case CK_BOARDNO:          *((Card**)&G.p_boardNo)          = pCard; break;
    case CK_BOOLCALC:         *((Card**)&G.p_boolCalc)         = pCard; break;
    case CK_BOUND:            *((Card**)&G.p_bound)            = pCard; break;
    case CK_BOUNDS:           *((Card**)&G.p_bounds)           = pCard; break;
    case CK_CALCOP:           *((Card**)&G.p_calcOp)           = pCard; break;
    case CK_CALL:             *((Card**)&G.p_call)             = pCard; break;
    case CK_CAST:             *((Card**)&G.p_cast)             = pCard; break;
    case CK_CLAUSE:           *((Card**)&G.p_clause)           = pCard; break;
    case CK_CMPPRED:          *((Card**)&G.p_cmpPred)          = pCard; break;
    case CK_COLNM:            *((Card**)&G.p_colNm)            = pCard; break;
    case CK_COMBINED:         *((Card**)&G.p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&G.p_comment)          = pCard; break;
    case CK_CONSTRAINT:       *((Card**)&G.p_constraint)       = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&G.p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&G.p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&G.p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&G.p_coPtnPredList)    = pCard; break;
    case CK_CORRELATION:      *((Card**)&G.p_correlation)      = pCard; break;
    case CK_CROSSJOIN:        *((Card**)&G.p_crossJoin)        = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&G.p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&G.p_crossPtnJoinList) = pCard; break;
    case CK_DIRECTION:        *((Card**)&G.p_direction)        = pCard; break;
    case CK_EXPR:             *((Card**)&G.p_expr)             = pCard; break;
    case CK_EXPRLIST:         *((Card**)&G.p_exprList)         = pCard; break;
    case CK_FILTER:           *((Card**)&G.p_filter)           = pCard; break;
    case CK_FLAT:             *((Card**)&G.p_flat)             = pCard; break;
    case CK_FOLD:             *((Card**)&G.p_fold)             = pCard; break;
    case CK_FOLDELIST:        *((Card**)&G.p_foldElist)        = pCard; break;
    case CK_FROM:             *((Card**)&G.p_from)             = pCard; break;
    case CK_GBYAGG:           *((Card**)&G.p_gbyAgg)           = pCard; break;
    case CK_GENESIS:          *((Card**)&G.p_genesis)          = pCard; break;
    case CK_GROUP:            *((Card**)&G.p_group)            = pCard; break;
    case CK_HASH:             *((Card**)&G.p_hash)             = pCard; break;
    case CK_HAVING:           *((Card**)&G.p_having)           = pCard; break;
    case CK_IDXCOL:           *((Card**)&G.p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&G.p_idxColList)       = pCard; break;
    case CK_IDXCOLNM:         *((Card**)&G.p_idxColNm)         = pCard; break;
    case CK_INDEX:            *((Card**)&G.p_index)            = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&G.p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&G.p_indexList)        = pCard; break;
    case CK_INDEXNM:          *((Card**)&G.p_indexNm)          = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&G.p_indexScan)        = pCard; break;
    case CK_JOINHEAD:         *((Card**)&G.p_joinHead)         = pCard; break;
    case CK_JOINNEXT:         *((Card**)&G.p_joinNext)         = pCard; break;
    case CK_JOINORDER:        *((Card**)&G.p_joinOrder)        = pCard; break;
    case CK_JOINORDERING:     *((Card**)&G.p_joinOrdering)     = pCard; break;
    case CK_JOINPERMALL:      *((Card**)&G.p_joinPermAll)      = pCard; break;
    case CK_JOINPERMSEQ:      *((Card**)&G.p_joinPermSeq)      = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&G.p_joinPredList)     = pCard; break;
    case CK_KEY:              *((Card**)&G.p_key)              = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&G.p_keyExists)        = pCard; break;
    case CK_KEYNOTEXISTS:     *((Card**)&G.p_keyNotExists)     = pCard; break;
    case CK_LEVEL:            *((Card**)&G.p_level)            = pCard; break;
    case CK_LIMIT:            *((Card**)&G.p_limit)            = pCard; break;
    case CK_LOAD:             *((Card**)&G.p_load)             = pCard; break;
    case CK_LOADJOINED:       *((Card**)&G.p_loadJoined)       = pCard; break;
    case CK_LOADNETWORK:      *((Card**)&G.p_loadNetwork)      = pCard; break;
    case CK_LOADPERMSEQ:      *((Card**)&G.p_loadPermSeq)      = pCard; break;
    case CK_LOADUNICAST:      *((Card**)&G.p_loadUnicast)      = pCard; break;
    case CK_MANNER:           *((Card**)&G.p_manner)           = pCard; break;
    case CK_NCORRCOLS:        *((Card**)&G.p_nCorrCols)        = pCard; break;
    case CK_NEXT:             *((Card**)&G.p_next)             = pCard; break;
    case CK_NOTE:             *((Card**)&G.p_note)             = pCard; break;
    case CK_NOTPRED:          *((Card**)&G.p_notPred)          = pCard; break;
    case CK_NPARMCOLS:        *((Card**)&G.p_nParmCols)        = pCard; break;
    case CK_ON:               *((Card**)&G.p_on)               = pCard; break;
    case CK_ORDAGG:           *((Card**)&G.p_ordAgg)           = pCard; break;
    case CK_ORDELMODN:        *((Card**)&G.p_ordElmOdn)        = pCard; break;
    case CK_ORDER:            *((Card**)&G.p_order)            = pCard; break;
    case CK_OUTERCOLNM:       *((Card**)&G.p_outerColNm)       = pCard; break;
    case CK_OUTERJOIN:        *((Card**)&G.p_outerJoin)        = pCard; break;
    case CK_PARTITION:        *((Card**)&G.p_partition)        = pCard; break;
    case CK_PERMUTATION:      *((Card**)&G.p_permutation)      = pCard; break;
    case CK_PMZWHERE:         *((Card**)&G.p_pmzWhere)         = pCard; break;
    case CK_PROJECT:          *((Card**)&G.p_project)          = pCard; break;
    case CK_PROJECTION:       *((Card**)&G.p_projection)       = pCard; break;
    case CK_PTNCOL:           *((Card**)&G.p_ptnCol)           = pCard; break;
    case CK_PTNCOLLIST:       *((Card**)&G.p_ptnColList)       = pCard; break;
    case CK_PTNCOLNM:         *((Card**)&G.p_ptnColNm)         = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&G.p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&G.p_ptndJoinSet)      = pCard; break;
    case CK_REASON:           *((Card**)&G.p_reason)           = pCard; break;
    case CK_RELNOP:           *((Card**)&G.p_relnOp)           = pCard; break;
    case CK_RPNCALC:          *((Card**)&G.p_rpnCalc)          = pCard; break;
    case CK_SCAN:             *((Card**)&G.p_scan)             = pCard; break;
    case CK_SECTION:          *((Card**)&G.p_section)          = pCard; break;
    case CK_SEND:             *((Card**)&G.p_send)             = pCard; break;
    case CK_SIZE:             *((Card**)&G.p_size)             = pCard; break;
    case CK_SORT:             *((Card**)&G.p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&G.p_stage)            = pCard; break;
    case CK_STMCOL:           *((Card**)&G.p_stmCol)           = pCard; break;
    case CK_STMCOLNM:         *((Card**)&G.p_stmColNm)         = pCard; break;
    case CK_STMCOLODN:        *((Card**)&G.p_stmColOdn)        = pCard; break;
    case CK_SUBQUERY:         *((Card**)&G.p_subQuery)         = pCard; break;
    case CK_SUBSET:           *((Card**)&G.p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&G.p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&G.p_tableList)        = pCard; break;
    case CK_TABLENM:          *((Card**)&G.p_tableNm)          = pCard; break;
    case CK_TABLEPTN:         *((Card**)&G.p_tablePtn)         = pCard; break;
    case CK_TBLCOLODN:        *((Card**)&G.p_tblColOdn)        = pCard; break;
    case CK_TERM:             *((Card**)&G.p_term)             = pCard; break;
    case CK_TOP:              *((Card**)&G.p_top)              = pCard; break;
    case CK_TOPOLOGY:         *((Card**)&G.p_topology)         = pCard; break;
    case CK_TWIST:            *((Card**)&G.p_twist)            = pCard; break;
    case CK_TYPE:             *((Card**)&G.p_type)             = pCard; break;
    case CK_UNIQUE:           *((Card**)&G.p_unique)           = pCard; break;
    case CK_WHERE:            *((Card**)&G.p_where)            = pCard; break;
    case CK_XBOUND:           *((Card**)&G.p_xBound)           = pCard; break;

    case CK_AV_AGGP:          *((Card**)&G.p_av_aggP)          = pCard; break;
    case CK_AV_AGGPN:         *((Card**)&G.p_av_aggPN)         = pCard; break;
    case CK_AV_AGGPNO:        *((Card**)&G.p_av_aggPNO)        = pCard; break;
    case CK_AV_AGGREGP:       *((Card**)&G.p_av_aggregP)       = pCard; break;
    case CK_AV_AGGREGPN:      *((Card**)&G.p_av_aggregPN)      = pCard; break;
    case CK_AV_AGGREGPNO:     *((Card**)&G.p_av_aggregPNO)     = pCard; break;
    case CK_AV_FILTER:        *((Card**)&G.p_av_filter)        = pCard; break;
    case CK_AV_FILTERPROJECT: *((Card**)&G.p_av_filterProject) = pCard; break;
    case CK_AV_GBYAGGP:       *((Card**)&G.p_av_gbyAggP)       = pCard; break;
    case CK_AV_GBYAGGPN:      *((Card**)&G.p_av_gbyAggPN)      = pCard; break;
    case CK_AV_GBYPNO:        *((Card**)&G.p_av_gbyPNO)        = pCard; break;
    case CK_AV_JOINSERIES:    *((Card**)&G.p_av_joinSeries)    = pCard; break;
    case CK_AV_LIMIT:         *((Card**)&G.p_av_limit)         = pCard; break;
    case CK_AV_PROJECT:       *((Card**)&G.p_av_project)       = pCard; break;
    case CK_AV_SCANJOINSERIES:*((Card**)&G.p_av_scanJoinSeries)= pCard; break;
    case CK_AV_SCANONLY:      *((Card**)&G.p_av_scanOnly)      = pCard; break;
    case CK_AV_SORT:          *((Card**)&G.p_av_sort)          = pCard; break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vUsingBoard(eBK eBoard, i8 i_Board)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.pBoard = m_pBench->pGetBoard(eBoard, i_Board);
  G.pDeck  = G.pBoard->pDeck();

  this->vChangeCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXdesc(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_sXdesc;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void CardIter::vXdesc(Stz* szDescription)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_sXdesc = szDescription;
  G.pBoard->vXdesc(szDescription);
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXform(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_sXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void CardIter::vXform(Stz* szTransformation)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_sXform = szTransformation;
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXhand(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_sXhand;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vXhand(const char* asXhand)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_sXhand = asXhand;

  G.pBoard->vXhand(asXhand);
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bPopRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  CardIterMembers* pXsentinals2 = pPeek(0);
  if (! pXsentinals2 || ! pXsentinals2->m_bRecursed)
    return false;

  vRestore();

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  RASSERT(G.m_bRecursed);

  vSave();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
