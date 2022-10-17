/*============================================================================*/
/* TransformerTool.cpp --                                                     */
/*============================================================================*/

#include "TransformerTool.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TransformerTool::TransformerTool(eCN eClass, Bench* pBench)
: Object(eClass)
, m_pM(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();
  m_bTracing = false;
  m_iTrace   = 0;

  m_pBoard  = m_pBench->pHead();
  m_eBoard  = BK_NONE;
  m_i_SubBoard = C_UNKNOWN;

  m_bPrintTransformation = true;
  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  m_pPermGen = nullptr;

  m_pTable     = nullptr;
  m_pStreamDef = nullptr;
  m_pIdxVec    = nullptr;
  m_pIndex     = nullptr;

  m_pDeck = nullptr;

  m_pCard_child  = nullptr;
  m_pCard_dup    = nullptr;
  m_pCard_new    = nullptr;
  m_pCard_popped = nullptr;
  m_pCard_temp   = nullptr;

  // the M iterator - - - - - - - - - - - - - - - - - - - - - -
  //m_pM = new BoardIter(m_pBoard);

  M_vClearMembers();
  M_vClearRegisters();

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TransformerTool::~TransformerTool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //delete m_pM;
  delete m_pPermGen;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* TransformerTool::pBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return m_pBoard;
}

/*----------------------------------------------------------------------------*/
/* decode card                                                                */
/*----------------------------------------------------------------------------*/
eCK TransformerTool::eDecodeMain(Card* pCard)                                   // TODO: to be deleted
{
  _precondition_(_ADDRESS_, _MAGIC_);

  M.m_e_MainCard = pCard->eCard();
  
  switch (M.m_e_MainCard) {

    case CK_ABSJOIN:    M.m_p_absJoin     = _pCard_AbsJoin(pCard);   break;
    case CK_AGGREG:     M.m_p_aggreg      = _pCard_Aggreg(pCard);    break;
    case CK_COMBINED:   M.m_p_combined    = _pCard_Combined(pCard);   break;
    case CK_FILTER:     M.m_p_filter      = _pCard_Filter(pCard);    break;
    case CK_GBYELIST:   M.m_p_gbyElist    = _pCard_GbyElist(pCard);  break;
    case CK_INDEXJOIN:  M.m_p_indexJoin   = _pCard_IndexJoin(pCard); break;
    case CK_INDEXSCAN:  M.m_p_indexScan   = _pCard_IndexScan(pCard); break;
    case CK_PROJECT:    M.m_p_project     = _pCard_Project(pCard);   break;
    case CK_SBYELIST:   M.m_p_sbyElist    = _pCard_SbyElist(pCard);  break;
    case CK_SCAN:       M.m_p_scan        = _pCard_Scan(pCard);      break;
    case CK_SORT:       M.m_p_sort        = _pCard_Sort(pCard);      break;
    case CK_STAGE:      M.m_p_stage       = _pCard_Stage(pCard);     break;
    case CK_STAGETOPO:  M.m_p_stageTopo   = _pCard_StageTopo(pCard); break;

    default:
      NEVER_REACH
  }

  return M.m_e_MainCard;
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* TransformerTool::szDescription(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return m_sDescription;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void TransformerTool::vDescription(Stz* szDescription)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_sDescription = szDescription;
}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void TransformerTool::vEndTransformation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_bTracing = false;

  // UNDO: Card::vConsistencyCheck(m_pDeck, m_iTransformNo);
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* TransformerTool::szPhase(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return m_sPhase;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void TransformerTool::vPhase(const char* asPhase)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_sPhase = asPhase;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vPrintBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (m_bPrintTransformation) {
    if (m_bInteractive)
      m_pBoard->vPrintBoard(szPhase(),
                            szTransformation(),
                            szDescription());
  }
  else {
    vTrace("SILENT : %s\n"_Stz, m_sTransformation.szStz());
  }

  m_sDescription = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* set control of card printing by kind                                       */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetPrintByKind(eCK eCard, bool bPrintByKind)
{
  m_pCardCtl->vPrintByKind(eCard,bPrintByKind);
}

/*----------------------------------------------------------------------------*/
/* set card print properties                                                  */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetPrintProperties(eCK eCard, bool bPrintProperties)
{
  m_pCardCtl->vPrintProperties(eCard,bPrintProperties);
}

/*----------------------------------------------------------------------------*/
/* set card print subCards                                                     */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetPrintSubCards(eCK eCard, bool bPrintSubCards)
{
  m_pCardCtl->vPrintSubCards(eCard,bPrintSubCards);
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* TransformerTool::szTransformation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return m_sTransformation;
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void TransformerTool::vTransformation(const char* azTransformation,
                                      bool bPrintTransformation)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_sTransformation = azTransformation;
  m_bPrintTransformation = bPrintTransformation;

  m_pBoard->vTransformNoInc();

  m_pM.vRestart();
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void TransformerTool::vUsingBoard(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_eBoard = eBoard;
  m_i_SubBoard = i_SubBoard;

  m_pBoard = m_pBench->pGetBoard(m_eBoard,m_i_SubBoard);
  m_pDeck  = m_pBoard->pDeck();

  m_pM.vChangeBoard(m_pBoard);
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
bool TransformerTool::bIsTracing(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return m_bTracing;
}

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
void TransformerTool::vTracing(bool bTracing)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_bTracing = bTracing;

  if (m_bTracing) {
    vTrace("\n\nTracing %s\n"_Stz,m_sTransformation.szStz());
    m_iTrace = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* conditionally trace                                                        */
/*----------------------------------------------------------------------------*/
void TransformerTool::vTracer(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_);

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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vAppendChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = F_pFetchMember(eParent);
  Card* pChild  = F_pFetchMember(eChild);

  pParent->vAppend(pChild);

  if (! m_bTracing)
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = F_pFetchMember(eParent);

  pParent->vAppend(pChild);

  if (! m_bTracing)
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vDuplicateMember(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pMember  = F_pFetchMember(eMember);

  m_pCard_dup = pMember->pDupCardAndSubCards();

  if (! m_bTracing)
    return;

  vTrace("  %s %s from %s\n"_Stz,
         __func__,m_pCard_dup->szBearing(),pMember->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vRemoveChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_, _MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vRemoveDeleteChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_, _MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vRemoveDeleteMain(eCK eMain)
{
  _precondition_(_ADDRESS_, _MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::C_vPromoteChildren(eCK eParent)
{
  _precondition_(_ADDRESS_, _MAGIC_);

}


/*----------------------------------------------------------------------------*/
/* delete member of given kind; clear-if registers; update M member to null   */
/*----------------------------------------------------------------------------*/
void TransformerTool::D_vDeleteMember(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pMember = F_pFetchMember(eMember);

  M_vClearRegistersIf(pMember);
  U_vUpdateMember(eMember, nullptr,__func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete given member; clear-if registers; update M member to null           */
/*----------------------------------------------------------------------------*/
void TransformerTool::D_vDeleteMember(Card* pMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  eCK eMember = pMember->eCard();

  M_vClearRegistersIf(pMember);
  U_vUpdateMember(eMember, nullptr,__func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMain(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_MainCard = m_pM.pEachMain())) {
    M.m_e_MainCard = M.m_p_MainCard->eCard();
    U_vUpdateMember(M.m_e_MainCard, M.m_p_MainCard,__func__);
    return true;
  }

  M.m_e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMain(eCK eOfMainKind)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_MainCard = m_pM.pEachMain(eOfMainKind))) {
    M.m_e_MainCard = M.m_p_MainCard->eCard();
    U_vUpdateMember(M.m_e_MainCard, M.m_p_MainCard,__func__);
    return true;
  }

  M.m_e_MainCard = CK_NONE;
  U_vUpdateMember(eOfMainKind, nullptr,__func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  while ((M.m_p_MainCard = m_pM.pEachMain(eOfMainKind))) {
    if (! (M.m_p_SubOfMain = M.m_p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    M.m_e_MainCard = M.m_p_MainCard->eCard();
    U_vUpdateMember(M.m_e_MainCard, M.m_p_MainCard,__func__);
    M.m_e_SubOfMain = M.m_p_SubOfMain->eCard();
    U_vUpdateMember(M.m_e_SubOfMain, M.m_p_SubOfMain,__func__);
    return true;
  }

  M.m_p_MainCard  = nullptr;
  M.m_e_MainCard  = CK_NONE;
  U_vUpdateMember(eOfMainKind, nullptr,__func__);
  M.m_p_SubOfMain = nullptr;
  M.m_e_SubOfMain = CK_NONE;
  U_vUpdateMember(eAndSubKind, nullptr,__func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMainWith2Sub(eCK eOfMainKind,
                                          eCK eAndSubKind1,
                                          eCK eAndSubKind2)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  while ((M.m_p_MainCard = m_pM.pEachMain(eOfMainKind))) {
    if (! (M.m_p_SubOfMain = M.m_p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (M.m_p_Sub2OfMain = M.m_p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    M.m_e_MainCard = M.m_p_MainCard->eCard();
    U_vUpdateMember(M.m_e_MainCard, M.m_p_MainCard,__func__);
    M.m_e_SubOfMain = M.m_p_SubOfMain->eCard();
    U_vUpdateMember(M.m_e_SubOfMain, M.m_p_SubOfMain,__func__);
    M.m_e_Sub2OfMain = M.m_p_Sub2OfMain->eCard();
    U_vUpdateMember(M.m_e_Sub2OfMain, M.m_p_Sub2OfMain,__func__);
    return true;
  }

  M.m_p_MainCard  = nullptr;
  M.m_e_MainCard  = CK_NONE;
  U_vUpdateMember(eOfMainKind, nullptr,__func__);
  M.m_p_SubOfMain = nullptr;
  M.m_e_SubOfMain = CK_NONE;
  U_vUpdateMember(eAndSubKind1, nullptr,__func__);
  M.m_p_Sub2OfMain = nullptr;
  M.m_e_Sub2OfMain = CK_NONE;
  U_vUpdateMember(eAndSubKind2, nullptr,__func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM.vForward();

  if (bClearMembers)
    M_vClearMembers();

  if (bClearRegisters)
    M_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* are there more main cards?                                                 */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bMoreMain(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (m_pM.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vRemoveMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM.vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* remove ain card from deck and delete it; clear M member to null            */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM.vRemoveDeleteCurr(eCrossCheck);

  M.m_p_MainCard = nullptr;
  M.m_e_MainCard = CK_NONE;

  U_vUpdateMember(eCrossCheck, nullptr,__func__);
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_SubsequentToMain = m_pM.pNextMain())) {
    M.m_e_SubsequentToMain = M.m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_SubsequentToMain, M.m_p_SubsequentToMain,__func__);
    return;
  }

  M.m_e_SubsequentToMain = CK_NONE;
}


/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* TransformerTool::F_pFetchMember(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eMember) {

    case CK_ABSJOIN:          return M.m_p_absJoin;
    case CK_AGGREG:           return M.m_p_aggreg;
    case CK_BOOLCALC:         return M.m_p_boolCalc;
    case CK_CALC:             return M.m_p_calc;
    case CK_CMPPRED:          return M.m_p_cmpPred;
    case CK_COMBINED:         return M.m_p_combined;
    case CK_COMMENT:          return M.m_p_comment;
    case CK_COPTNJOIN:        return M.m_p_coPtnJoin;
    case CK_COPTNJOINLIST:    return M.m_p_coPtnJoinList;
    case CK_COPTNPRED:        return M.m_p_coPtnPred;
    case CK_COPTNPREDLIST:    return M.m_p_coPtnPredList;
    case CK_CROSSPTNJOIN:     return M.m_p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return M.m_p_crossPtnJoinList;
    case CK_EXPR:             return M.m_p_expr;
    case CK_FILTER:           return M.m_p_filter;
    case CK_GBYELIST:         return M.m_p_gbyElist;
    case CK_IDXCOLLIST:       return M.m_p_idxColList;
    case CK_INDEXJOIN:        return M.m_p_indexJoin;
    case CK_INDEXLIST:        return M.m_p_indexList;
    case CK_INDEXPTR:         return M.m_p_indexPtr;
    case CK_INDEXSCAN:        return M.m_p_indexScan;
    case CK_JOINORDER:        return M.m_p_joinOrder;
    case CK_JOINORDERSEQ:     return M.m_p_joinOrderSeq;
    case CK_JOINPRED:         return M.m_p_joinPred;
    case CK_JOINPREDLIST:     return M.m_p_joinPredList;
    case CK_KEYEXISTS:        return M.m_p_keyExists;
    case CK_LBOUND:           return M.m_p_lBound;
    case CK_PARAM:            return M.m_p_param;
    case CK_PARAMLIST:        return M.m_p_paramList;
    case CK_PERMUTATION:      return M.m_p_permutation;
    case CK_PROJECT:          return M.m_p_project;
    case CK_PTNDJOIN:         return M.m_p_ptndJoin;
    case CK_PTNDJOINSET:      return M.m_p_ptndJoinSet;
    case CK_SBYELIST:         return M.m_p_sbyElist;
    case CK_SCAN:             return M.m_p_scan;
    case CK_SCANPRED:         return M.m_p_scanPred;
    case CK_SEND:             return M.m_p_send;
    case CK_SORT:             return M.m_p_sort;
    case CK_STAGE:            return M.m_p_stage;
    case CK_STAGETOPO:        return M.m_p_stageTopo;
    case CK_STREAM:           return M.m_p_stream;
    case CK_STREAMCOLLIST:    return M.m_p_streamColList;
    case CK_SUBSET:           return M.m_p_subSet;
    case CK_TABLE:            return M.m_p_table;
    case CK_TABLELIST:        return M.m_p_tableList;
    case CK_TABLEPTN:         return M.m_p_tablePtn;
    case CK_TABLEPTR:         return M.m_p_tablePtr;
    case CK_TERM:             return M.m_p_term;
    case CK_UBOUND:           return M.m_p_uBound;
    case CK_WHERE:            return M.m_p_where;

   default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main card?                      */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasPriorToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_PriorToMain = m_pM.pPrevMain())) {
    M.m_e_PriorToMain = M.m_p_PriorToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_PriorToMain, M.m_p_PriorToMain,__func__);
    return true;
  }

  M.m_e_PriorToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main which is of given kind?    */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_PriorToMain = m_pM.pPrevMainIf(eOfPriorKind))) {
    M.m_e_PriorToMain = M.m_p_PriorToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_PriorToMain, M.m_p_PriorToMain,__func__);
    return true;
  }

  M.m_e_PriorToMain = CK_NONE;
  if (bUpdateMember)
    U_vUpdateMember(eOfPriorKind, nullptr,__func__);

  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_SubsequentToMain = m_pM.pNextMain())) {
    M.m_e_SubsequentToMain = M.m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_SubsequentToMain, M.m_p_SubsequentToMain,__func__);
    return true;
  }

  M.m_e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main which is of given kind?     */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_SubsequentToMain = m_pM.pNextMainIf(eOfSubsequentKind))) {
    M.m_e_SubsequentToMain = M.m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_SubsequentToMain, M.m_p_SubsequentToMain,__func__);
    return true;
  }

  M.m_e_SubsequentToMain = CK_NONE;
  if (bUpdateMember)
    U_vUpdateMember(eOfSubsequentKind, nullptr,__func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearMembers(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  M.m_p_absJoin          = nullptr;
  M.m_p_aggreg           = nullptr;
  M.m_p_boolCalc         = nullptr;
  M.m_p_calc             = nullptr;
  M.m_p_cmpPred          = nullptr;
  M.m_p_combined         = nullptr;
  M.m_p_comment          = nullptr;
  M.m_p_coPtnJoin        = nullptr;
  M.m_p_coPtnJoinList    = nullptr;
  M.m_p_coPtnPred        = nullptr;
  M.m_p_coPtnPredList    = nullptr;
  M.m_p_crossPtnJoin     = nullptr;
  M.m_p_crossPtnJoinList = nullptr;
  M.m_p_expr             = nullptr;
  M.m_p_filter           = nullptr;
  M.m_p_gbyElist         = nullptr;
  M.m_p_idxCol           = nullptr;
  M.m_p_idxColList       = nullptr;
  M.m_p_indexJoin        = nullptr;
  M.m_p_indexList        = nullptr;
  M.m_p_indexPtr         = nullptr;
  M.m_p_indexScan        = nullptr;
  M.m_p_joinOrder        = nullptr;
  M.m_p_joinOrderSeq     = nullptr;
  M.m_p_joinPred         = nullptr;
  M.m_p_joinPredList     = nullptr;
  M.m_p_keyExists        = nullptr;
  M.m_p_lBound           = nullptr;
  M.m_p_param            = nullptr;
  M.m_p_paramList        = nullptr;
  M.m_p_permutation      = nullptr;
  M.m_p_permutation_low  = nullptr;
  M.m_p_project          = nullptr;
  M.m_p_ptndJoin         = nullptr;
  M.m_p_ptndJoinSet      = nullptr;
  M.m_p_sbyElist         = nullptr;
  M.m_p_scan             = nullptr;
  M.m_p_scanPred         = nullptr;
  M.m_p_send             = nullptr;
  M.m_p_sort             = nullptr;
  M.m_p_stage            = nullptr;
  M.m_p_stageTopo        = nullptr;
  M.m_p_stream           = nullptr;
  M.m_p_streamCol        = nullptr;
  M.m_p_streamColList    = nullptr;
  M.m_p_subSet           = nullptr;
  M.m_p_table            = nullptr;
  M.m_p_tableList        = nullptr;
  M.m_p_tablePtn         = nullptr;
  M.m_p_tablePtr         = nullptr;
  M.m_p_term             = nullptr;
  M.m_p_uBound           = nullptr;
  M.m_p_where            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the M members and registers                                          */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearMembersRegisters(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  M_vClearMembers();
  M_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearRegisters(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  M.m_p_PriorToMain      = nullptr;
  M.m_e_PriorToMain      = CK_NONE;
  M.m_p_MainCard         = nullptr;
  M.m_e_MainCard         = CK_NONE;
  M.m_p_SubOfMain        = nullptr;
  M.m_e_SubOfMain        = CK_NONE;
  M.m_p_Sub2OfMain       = nullptr;
  M.m_e_Sub2OfMain       = CK_NONE;
  M.m_p_SubsequentToMain = nullptr;
  M.m_e_SubsequentToMain = CK_NONE;

  M.m_p_SubMember        = nullptr;
  M.m_e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given kind                     */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearRegistersIf(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (eCard == M.m_e_PriorToMain ||
       (M.m_p_PriorToMain && eCard == M.m_p_PriorToMain->eCard())) {
    M.m_p_PriorToMain = nullptr;
    M.m_e_PriorToMain = CK_NONE;
  }
  if (eCard == M.m_e_MainCard ||
       (M.m_p_MainCard && eCard == M.m_p_MainCard->eCard())) {
    M.m_p_MainCard = nullptr;
    M.m_e_MainCard = CK_NONE;
  }
  if (eCard == M.m_e_SubOfMain ||
       (M.m_p_SubOfMain && eCard == M.m_p_SubOfMain->eCard())) {
    M.m_p_SubOfMain = nullptr;
    M.m_e_SubOfMain = CK_NONE;
  }
  if (eCard == M.m_e_Sub2OfMain ||
      (M.m_p_Sub2OfMain && eCard == M.m_p_Sub2OfMain->eCard())) {
    M.m_p_Sub2OfMain = nullptr;
    M.m_e_Sub2OfMain = CK_NONE;
  }
  if (eCard == M.m_e_SubsequentToMain ||
       (M.m_p_SubsequentToMain && eCard == M.m_p_SubsequentToMain->eCard())) {
    M.m_p_SubsequentToMain = nullptr;
    M.m_e_SubsequentToMain = CK_NONE;
  }
  if (eCard == M.m_e_SubMember ||
       (M.m_p_SubMember && eCard == M.m_p_SubMember->eCard())) {
    M.m_p_SubMember = nullptr;
    M.m_e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearRegistersIf(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (pCard == M.m_p_PriorToMain) {
    M.m_p_PriorToMain = nullptr;
    M.m_e_PriorToMain = CK_NONE;
  }
  if (pCard == M.m_p_MainCard) {
    M.m_p_MainCard = nullptr;
    M.m_e_MainCard = CK_NONE;
  }
  if (pCard == M.m_p_SubOfMain) {
    M.m_p_SubOfMain = nullptr;
    M.m_e_SubOfMain = CK_NONE;
  }
  if (pCard == M.m_p_SubsequentToMain) {
    M.m_p_SubsequentToMain = nullptr;
    M.m_e_SubsequentToMain = CK_NONE;
  }
  if (pCard == M.m_p_SubMember) {
    M.m_p_SubMember = nullptr;
    M.m_e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vRestart(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM.vRestart();

  M_vClearMembers();
  M_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void TransformerTool::N_vNewMember(eCK eMember, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pCard_new = new Card(eMember);

  if (bUpdateMember)
    U_vUpdateMember(eMember, m_pCard_new,__func__);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void TransformerTool::N_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  N_vNewMember(eMember,bUpdateMember);

  m_pCard_new->vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
void TransformerTool::U_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eMember) {

    case CK_ABSJOIN:          *((Card**)&M.m_p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&M.m_p_aggreg)           = pCard; break;
    case CK_BOOLCALC:         *((Card**)&M.m_p_boolCalc)         = pCard; break;
    case CK_CALC:             *((Card**)&M.m_p_calc)             = pCard; break;
    case CK_CMPPRED:          *((Card**)&M.m_p_cmpPred)          = pCard; break;
    case CK_COMBINED:         *((Card**)&M.m_p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&M.m_p_comment)          = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&M.m_p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&M.m_p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&M.m_p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&M.m_p_coPtnPredList)    = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&M.m_p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&M.m_p_crossPtnJoinList) = pCard; break;
    case CK_EXPR:             *((Card**)&M.m_p_expr)             = pCard; break;
    case CK_FILTER:           *((Card**)&M.m_p_filter)           = pCard; break;
    case CK_GBYELIST:         *((Card**)&M.m_p_gbyElist)         = pCard; break;
    case CK_IDXCOL:           *((Card**)&M.m_p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&M.m_p_idxColList)       = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&M.m_p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&M.m_p_indexList)        = pCard; break;
    case CK_INDEXPTR:         *((Card**)&M.m_p_indexPtr)         = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&M.m_p_indexScan)        = pCard; break;
    case CK_JOINORDER:        *((Card**)&M.m_p_joinOrder)        = pCard; break;
    case CK_JOINORDERSEQ:     *((Card**)&M.m_p_joinOrderSeq)     = pCard; break;
    case CK_JOINPRED:         *((Card**)&M.m_p_joinPred)         = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&M.m_p_joinPredList)     = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&M.m_p_keyExists)        = pCard; break;
    case CK_LBOUND:           *((Card**)&M.m_p_lBound)           = pCard; break;
    case CK_PARAM:            *((Card**)&M.m_p_param)            = pCard; break;
    case CK_PARAMLIST:        *((Card**)&M.m_p_paramList)        = pCard; break;
    case CK_PERMUTATION:      *((Card**)&M.m_p_permutation)      = pCard; break;
    case CK_PROJECT:          *((Card**)&M.m_p_project)          = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&M.m_p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&M.m_p_ptndJoinSet)      = pCard; break;
    case CK_SBYELIST:         *((Card**)&M.m_p_sbyElist)         = pCard; break;
    case CK_SCAN:             *((Card**)&M.m_p_scan)             = pCard; break;
    case CK_SCANPRED:         *((Card**)&M.m_p_scanPred)         = pCard; break;
    case CK_SEND:             *((Card**)&M.m_p_send)             = pCard; break;
    case CK_SORT:             *((Card**)&M.m_p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&M.m_p_stage)            = pCard; break;
    case CK_STAGETOPO:        *((Card**)&M.m_p_stageTopo)        = pCard; break;
    case CK_STREAM:           *((Card**)&M.m_p_stream)           = pCard; break;
    case CK_STREAMCOLLIST:    *((Card**)&M.m_p_streamColList)    = pCard; break;
    case CK_SUBSET:           *((Card**)&M.m_p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&M.m_p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&M.m_p_tableList)        = pCard; break;
    case CK_TABLEPTN:         *((Card**)&M.m_p_tablePtn)         = pCard; break;
    case CK_TABLEPTR:         *((Card**)&M.m_p_tablePtr)         = pCard; break;
    case CK_TERM:             *((Card**)&M.m_p_term)             = pCard; break;
    case CK_UBOUND:           *((Card**)&M.m_p_uBound)           = pCard; break;
    case CK_WHERE:            *((Card**)&M.m_p_where)            = pCard; break;

    default:
      NEVER_REACH
  }

  if (! m_bTracing)
    return;

  vTrace("  %s %s\n"_Stz,asFuncNm,szT(eMember));
}

/*----------------------------------------------------------------------------*/
/* search for sub of member                                                   */
/*----------------------------------------------------------------------------*/
bool TransformerTool::S_bSearchForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pMember = F_pFetchMember(eMember);

  bool b = S_bSearchForSubMember(pMember, eSubCard, bUpdateMember);

  return b;
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
bool TransformerTool::S_bSearchForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((M.m_p_SubMember = pMember->pFindSubCard(eSubCard))) {
    M.m_e_SubMember = M.m_p_SubMember->eCard();
    if (bUpdateMember)
      U_vUpdateMember(M.m_e_SubMember, M.m_p_SubMember,__func__);
    return true;
  }

  if (bUpdateMember)
    U_vUpdateMember(eSubCard, nullptr,__func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* TransformerTool::szT(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pMember = F_pFetchMember(eMember);
  Stz* szMemberNm = this->szMemberNm(eMember);
  if (pMember)
    m_sT.vSprint("%s=%s %s"_Stz,
                    szMemberNm,pMember->szValue(), pMember->szBearing());
  else
    m_sT.vSprint("%s=null"_Stz,szMemberNm);

  return m_sT;
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* TransformerTool::szMemberNm(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eMember) {

    case CK_ABSJOIN:          return "M.m_p_absJoin"_Stz;
    case CK_AGGREG:           return "M.m_p_aggreg"_Stz;
    case CK_BOOLCALC:         return "M.m_p_boolCalc"_Stz;
    case CK_CALC:             return "M.m_p_calc"_Stz;
    case CK_CMPPRED:          return "M.m_p_cmpPred"_Stz;
    case CK_COMBINED:         return "M.m_p_combined"_Stz;
    case CK_COMMENT:          return "M.m_p_comment"_Stz;
    case CK_COPTNJOIN:        return "M.m_p_coPtnJoin"_Stz;
    case CK_COPTNJOINLIST:    return "M.m_p_coPtnJoinList"_Stz;
    case CK_COPTNPRED:        return "M.m_p_coPtnPred"_Stz;
    case CK_COPTNPREDLIST:    return "M.m_p_coPtnPredList"_Stz;
    case CK_CROSSPTNJOIN:     return "M.m_p_crossPtnJoin"_Stz;
    case CK_CROSSPTNJOINLIST: return "M.m_p_crossPtnJoinList"_Stz;
    case CK_EXPR:             return "M.m_p_expr"_Stz;
    case CK_FILTER:           return "M.m_p_filter"_Stz;
    case CK_GBYELIST:         return "M.m_p_gbyElist"_Stz;
    case CK_IDXCOLLIST:       return "M.m_p_idxColList"_Stz;
    case CK_INDEXJOIN:        return "M.m_p_indexJoin"_Stz;
    case CK_INDEXLIST:        return "M.m_p_indexList"_Stz;
    case CK_INDEXPTR:         return "M.m_p_indexPtr"_Stz;
    case CK_INDEXSCAN:        return "M.m_p_indexScan"_Stz;
    case CK_JOINORDER:        return "M.m_p_joinOrder"_Stz;
    case CK_JOINORDERSEQ:     return "M.m_p_joinOrderSeq"_Stz;
    case CK_JOINPRED:         return "M.m_p_joinPred"_Stz;
    case CK_JOINPREDLIST:     return "M.m_p_joinPredList"_Stz;
    case CK_KEYEXISTS:        return "M.m_p_keyExists"_Stz;
    case CK_LBOUND:           return "M.m_p_lBound"_Stz;
    case CK_PARAM:            return "M.m_p_param"_Stz;
    case CK_PARAMLIST:        return "M.m_p_paramList"_Stz;
    case CK_PERMUTATION:      return "M.m_p_permutation"_Stz;
    case CK_PROJECT:          return "M.m_p_project"_Stz;
    case CK_PTNDJOIN:         return "M.m_p_ptndJoin"_Stz;
    case CK_PTNDJOINSET:      return "M.m_p_ptndJoinSet"_Stz;
    case CK_SBYELIST:         return "M.m_p_sbyElist"_Stz;
    case CK_SCAN:             return "M.m_p_scan"_Stz;
    case CK_SCANPRED:         return "M.m_p_scanPred"_Stz;
    case CK_SEND:             return "M.m_p_send"_Stz;
    case CK_SORT:             return "M.m_p_sort"_Stz;
    case CK_STAGE:            return "M.m_p_stage"_Stz;
    case CK_STAGETOPO:        return "M.m_p_stageTopo"_Stz;
    case CK_STREAM:           return "M.m_p_stream"_Stz;
    case CK_STREAMCOLLIST:    return "M.m_p_streamColList"_Stz;
    case CK_SUBSET:           return "M.m_p_subSet"_Stz;
    case CK_TABLE:            return "M.m_p_table"_Stz;
    case CK_TABLELIST:        return "M.m_p_tableList"_Stz;
    case CK_TABLEPTN:         return "M.m_p_tablePtn"_Stz;
    case CK_TABLEPTR:         return "M.m_p_tablePtr"_Stz;
    case CK_TERM:             return "M.m_p_term"_Stz;
    case CK_UBOUND:           return "M.m_p_uBound"_Stz;
    case CK_WHERE:            return "M.m_p_where"_Stz;

    default:
      NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
