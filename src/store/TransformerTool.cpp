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
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();

  m_pBoard           = m_pBench->pHead();
  m_pBoard_costEst   = nullptr;
  m_pBoard_partition = nullptr;
  m_pBoard_permute   = nullptr;
  m_pBoard_query     = nullptr;

  m_iTransformNo = 0;
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
  m_pM = new BoardIter(m_pBoard);

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

  delete m_pM;
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

  m_e_MainCard = pCard->eCard();
  
  switch (m_e_MainCard) {

    case CK_ABSJOIN:    m_p_absJoin     = _pCard_AbsJoin(pCard);   break;
    case CK_AGGREG:     m_p_aggreg      = _pCard_Aggreg(pCard);    break;
    case CK_COMBINED:   m_p_combined    = _pCard_Combined(pCard);   break;
    case CK_FILTER:     m_p_filter      = _pCard_Filter(pCard);    break;
    case CK_GBYELIST:   m_p_gbyElist    = _pCard_GbyElist(pCard);  break;
    case CK_INDEXJOIN:  m_p_indexJoin   = _pCard_IndexJoin(pCard); break;
    case CK_INDEXSCAN:  m_p_indexScan   = _pCard_IndexScan(pCard); break;
    case CK_PROJECT:    m_p_project     = _pCard_Project(pCard);   break;
    case CK_SBYELIST:   m_p_sbyElist    = _pCard_SbyElist(pCard);  break;
    case CK_SCAN:       m_p_scan        = _pCard_Scan(pCard);      break;
    case CK_SORT:       m_p_sort        = _pCard_Sort(pCard);      break;
    case CK_STAGE:      m_p_stage       = _pCard_Stage(pCard);     break;
    case CK_STAGETOPO:  m_p_stageTopo   = _pCard_StageTopo(pCard); break;

    default:
      NEVER_REACH
  }

  return m_e_MainCard;
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

  Card::vConsistencyCheck(m_pDeck, m_iTransformNo);
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
void TransformerTool::vPhase(Stz* szPhase)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_sPhase = szPhase;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vPrintBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (m_bPrintTransformation) {
    if (m_bInteractive)
      m_pBoard->vPrintBoard(this);
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
void TransformerTool::vTransformation(eBK eBoard,
                                      char* azTransformation,
                                      bool bPrintTransformation)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  //RASSERT(eBoard == m_pBoard->eBoard());

  vSetupBoard(eBoard);
  vSwapToBoard(eBoard);
  //m_pDeck = m_pBoard->pDeck();
  //m_pM->vRestart();

  m_sTransformation = azTransformation;
  m_bPrintTransformation = bPrintTransformation;
}

/*----------------------------------------------------------------------------*/
/* get the next transformation number                                         */
/*----------------------------------------------------------------------------*/
i8 TransformerTool::iTransformNoInc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  return ++ m_iTransformNo;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* swap to a different board                                                  */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSwapToBoard(eBK eBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard = m_pBench->pGetBoard(eBoard);
  m_pDeck  = m_pBoard->pDeck();

  m_pM->vChangeBoard(m_pBoard);
}

/*----------------------------------------------------------------------------*/
/* set up board for transformations                                           */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupBoard(eBK eBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eBoard) {

    case BK_COSTEST:   vSetupCostEstBoard();   break;
    case BK_PARTITION: vSetupPartitionBoard(); break;
    case BK_PERMUTE:   vSetupPermuteBoard();   break;
    case BK_QUERY:     vSetupQueryBoard();     break;

    default:
      NEVER_REACH
  }

  m_pBoard = m_pBench->pGetBoard(eBoard);
  m_pDeck  = m_pBoard->pDeck();

  m_pBoard_costEst   = m_pBench->pGetBoard(BK_COSTEST);
  m_pBoard_partition = m_pBench->pGetBoard(BK_PARTITION);
  m_pBoard_permute   = m_pBench->pGetBoard(BK_PERMUTE);
  m_pBoard_query     = m_pBench->pGetBoard(BK_QUERY);

  m_pM->vChangeBoard(m_pBoard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupCostEstBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard_costEst = m_pBench->pGetBoard(BK_COSTEST);
  if (!m_pBoard_costEst)
    m_pBoard_costEst = new Board(BK_COSTEST, BT_ORIGINAL, m_pBench);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupPartitionBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard_partition = m_pBench->pGetBoard(BK_PARTITION);
  if (!m_pBoard_partition)
    m_pBoard_partition = new Board(BK_PARTITION, BT_ORIGINAL, m_pBench);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupPermuteBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!m_pBoard_permute)
    m_pBoard_permute = new Board(BK_PERMUTE, BT_ORIGINAL, m_pBench);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupQueryBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard_query = m_pBench->pGetBoard(BK_QUERY);
  if (!m_pBoard_query)
    m_pBoard_query = new Board(BK_QUERY, BT_ORIGINAL, m_pBench);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* delete member of given kind; clear-if registers; update M member to null   */
/*----------------------------------------------------------------------------*/
void TransformerTool::D_vDeleteMember(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pMember = F_pFetchMember(eMember);

  M_vClearRegistersIf(pMember);
  U_vUpdateMember(eMember, nullptr);

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
  U_vUpdateMember(eMember, nullptr);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMain(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((m_p_MainCard = m_pM->pEachMain())) {
    m_e_MainCard = m_p_MainCard->eCard();
    U_vUpdateMember(m_e_MainCard, m_p_MainCard);
    return true;
  }

  m_e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMain(eCK eOfMainKind)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (m_p_MainCard = m_pM->pEachMain(eOfMainKind)) {
    m_e_MainCard = m_p_MainCard->eCard();
    U_vUpdateMember(m_e_MainCard, m_p_MainCard);
    return true;
  }

  m_e_MainCard = CK_NONE;
  U_vUpdateMember(eOfMainKind, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool TransformerTool::E_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  while (m_p_MainCard = m_pM->pEachMain(eOfMainKind)) {
    if (! (m_p_SubOfMain = m_p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    m_e_MainCard = m_p_MainCard->eCard();
    U_vUpdateMember(m_e_MainCard, m_p_MainCard);
    m_e_SubOfMain = m_p_SubOfMain->eCard();
    U_vUpdateMember(m_e_SubOfMain, m_p_SubOfMain);
    return true;
  }

  m_p_MainCard  = nullptr;
  m_e_MainCard  = CK_NONE;
  U_vUpdateMember(eOfMainKind, nullptr);
  m_p_SubOfMain = nullptr;
  m_e_SubOfMain = CK_NONE;
  U_vUpdateMember(eAndSubKind, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM->vForward();

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

  if (m_pM->bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vRemoveMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM->vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* remove ain card from deck and delete it; clear M member to null            */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM->vRemoveDeleteCurr(eCrossCheck);

  m_p_MainCard = nullptr;
  m_e_MainCard = CK_NONE;

  U_vUpdateMember(eCrossCheck, nullptr);
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
void TransformerTool::E_vSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((m_p_SubsequentToMain = m_pM->pNextMain())) {
    m_e_SubsequentToMain = m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_SubsequentToMain, m_p_SubsequentToMain);
    return;
  }

  m_e_SubsequentToMain = CK_NONE;
}


/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* TransformerTool::F_pFetchMember(eCK eMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eMember) {

    case CK_ABSJOIN:          return m_p_absJoin;
    case CK_AGGREG:           return m_p_aggreg;
    case CK_CALC:             return m_p_calc;
    case CK_BOOLCALC:         return m_p_calcBool;
    case CK_COMBINED:         return m_p_combined;
    case CK_COMMENT:          return m_p_comment;
    case CK_COPTNJOIN:        return m_p_coPtnJoin;
    case CK_COPTNJOINLIST:    return m_p_coPtnJoinList;
    case CK_COPTNPRED:        return m_p_coPtnPred;
    case CK_COPTNPREDLIST:    return m_p_coPtnPredList;
    case CK_CROSSPTNJOIN:     return m_p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return m_p_crossPtnJoinList;
    case CK_EXPR:             return m_p_expr;
    case CK_FILTER:           return m_p_filter;
    case CK_GBYELIST:         return m_p_gbyElist;
    case CK_IDXCOLLIST:       return m_p_idxColList;
    case CK_INDEXJOIN:        return m_p_indexJoin;
    case CK_INDEXLIST:        return m_p_indexList;
    case CK_INDEXPTR:         return m_p_indexPtr;
    case CK_INDEXSCAN:        return m_p_indexScan;
    case CK_JOINORDER:        return m_p_joinOrder;
    case CK_JOINORDERSEQ:     return m_p_joinOrderSeq;
    case CK_JOINPRED:         return m_p_joinPred;
    case CK_JOINPREDLIST:     return m_p_joinPredList;
    case CK_LBOUND:           return m_p_lBound;
    case CK_PERMUTATION:      return m_p_permutation;
    case CK_CMPPRED:          return m_p_cmpPred;
    case CK_PROJECT:          return m_p_project;
    case CK_PTNDJOIN:         return m_p_ptndJoin;
    case CK_PTNDJOINSET:      return m_p_ptndJoinSet;
    case CK_SBYELIST:         return m_p_sbyElist;
    case CK_SCAN:             return m_p_scan;
    case CK_SCANPRED:         return m_p_scanPred;
    case CK_SORT:             return m_p_sort;
    case CK_STAGE:            return m_p_stage;
    case CK_STAGETOPO:        return m_p_stageTopo;
    case CK_STREAM:           return m_p_stream;
    case CK_STREAMCOLLIST:    return m_p_streamColList;
    case CK_SUBSET:           return m_p_subSet;
    case CK_TABLE:            return m_p_table;
    case CK_TABLELIST:        return m_p_tableList;
    case CK_TABLEPTN:         return m_p_tablePtn;
    case CK_TABLEPTR:         return m_p_tablePtr;
    case CK_TERM:             return m_p_term;
    case CK_UBOUND:           return m_p_uBound;
    case CK_WHERE:            return m_p_where;

    case CK_SEND:
      NEVER_REACH
      break;

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

  if ((m_p_PriorToMain = m_pM->pPrevMain())) {
    m_e_PriorToMain = m_p_PriorToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_PriorToMain, m_p_PriorToMain);
    return true;
  }

  m_e_PriorToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main which is of given kind?    */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((m_p_PriorToMain = m_pM->pPrevMainIf(eOfPriorKind))) {
    m_e_PriorToMain = m_p_PriorToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_PriorToMain, m_p_PriorToMain);
    return true;
  }

  m_e_PriorToMain = CK_NONE;
  if (bUpdateMember)
    U_vUpdateMember(eOfPriorKind, nullptr);

  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((m_p_SubsequentToMain = m_pM->pNextMain())) {
    m_e_SubsequentToMain = m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_SubsequentToMain, m_p_SubsequentToMain);
    return true;
  }

  m_e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main which is of given kind?     */
/*----------------------------------------------------------------------------*/
bool TransformerTool::H_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if ((m_p_SubsequentToMain = m_pM->pNextMainIf(eOfSubsequentKind))) {
    m_e_SubsequentToMain = m_p_SubsequentToMain->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_SubsequentToMain, m_p_SubsequentToMain);
    return true;
  }

  m_e_SubsequentToMain = CK_NONE;
  if (bUpdateMember)
    U_vUpdateMember(eOfSubsequentKind, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearMembers(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_p_absJoin          = nullptr;
  m_p_aggreg           = nullptr;
  m_p_calc             = nullptr;
  m_p_calcBool         = nullptr;
  m_p_cmpPred          = nullptr;
  m_p_combined         = nullptr;
  m_p_comment          = nullptr;
  m_p_coPtnJoin        = nullptr;
  m_p_coPtnJoinList    = nullptr;
  m_p_coPtnPred        = nullptr;
  m_p_coPtnPredList    = nullptr;
  m_p_crossPtnJoin     = nullptr;
  m_p_crossPtnJoinList = nullptr;
  m_p_expr             = nullptr;
  m_p_filter           = nullptr;
  m_p_gbyElist         = nullptr;
  m_p_idxColList       = nullptr;
  m_p_indexJoin        = nullptr;
  m_p_indexList        = nullptr;
  m_p_indexPtr         = nullptr;
  m_p_indexScan        = nullptr;
  m_p_joinOrder        = nullptr;
  m_p_joinOrderSeq     = nullptr;
  m_p_joinPred         = nullptr;
  m_p_joinPredList     = nullptr;
  m_p_lBound           = nullptr;
  m_p_permutation      = nullptr;
  m_p_permutation_low  = nullptr;
  m_p_project          = nullptr;
  m_p_ptndJoin         = nullptr;
  m_p_ptndJoinSet      = nullptr;
  m_p_sbyElist         = nullptr;
  m_p_scan             = nullptr;
  m_p_scanPred         = nullptr;
  m_p_sort             = nullptr;
  m_p_stage            = nullptr;
  m_p_stageTopo        = nullptr;
  m_p_stream           = nullptr;
  m_p_streamColList    = nullptr;
  m_p_subSet           = nullptr;
  m_p_table            = nullptr;
  m_p_tableList        = nullptr;
  m_p_tablePtn         = nullptr;
  m_p_tablePtr         = nullptr;
  m_p_term             = nullptr;
  m_p_uBound           = nullptr;
  m_p_where            = nullptr;

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

  m_p_PriorToMain      = nullptr;
  m_e_PriorToMain      = CK_NONE;
  m_p_MainCard         = nullptr;
  m_e_MainCard         = CK_NONE;
  m_p_SubOfMain        = nullptr;
  m_e_SubOfMain        = CK_NONE;
  m_p_SubsequentToMain = nullptr;
  m_e_SubsequentToMain = CK_NONE;

  m_p_SubMember        = nullptr;
  m_e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given kind                     */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearRegistersIf(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (eCard == m_e_PriorToMain ||
       (m_p_PriorToMain && eCard == m_p_PriorToMain->eCard())) {
    m_p_PriorToMain = nullptr;
    m_e_PriorToMain = CK_NONE;
  }
  if (eCard == m_e_MainCard ||
       (m_p_MainCard && eCard == m_p_MainCard->eCard())) {
    m_p_MainCard = nullptr;
    m_e_MainCard = CK_NONE;
  }
  if (eCard == m_e_SubOfMain ||
       (m_p_SubOfMain && eCard == m_p_SubOfMain->eCard())) {
    m_p_SubOfMain = nullptr;
    m_e_SubOfMain = CK_NONE;
  }
  if (eCard == m_e_SubsequentToMain ||
       (m_p_SubsequentToMain && eCard == m_p_SubsequentToMain->eCard())) {
    m_p_SubsequentToMain = nullptr;
    m_e_SubsequentToMain = CK_NONE;
  }
  if (eCard == m_e_SubMember ||
       (m_p_SubMember && eCard == m_p_SubMember->eCard())) {
    m_p_SubMember = nullptr;
    m_e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vClearRegistersIf(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (pCard == m_p_PriorToMain) {
    m_p_PriorToMain = nullptr;
    m_e_PriorToMain = CK_NONE;
  }
  if (pCard == m_p_MainCard) {
    m_p_MainCard = nullptr;
    m_e_MainCard = CK_NONE;
  }
  if (pCard == m_p_SubOfMain) {
    m_p_SubOfMain = nullptr;
    m_e_SubOfMain = CK_NONE;
  }
  if (pCard == m_p_SubsequentToMain) {
    m_p_SubsequentToMain = nullptr;
    m_e_SubsequentToMain = CK_NONE;
  }
  if (pCard == m_p_SubMember) {
    m_p_SubMember = nullptr;
    m_e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void TransformerTool::M_vRestart(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pM->vRestart();

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
    U_vUpdateMember(eMember, m_pCard_new);
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
void TransformerTool::U_vUpdateMember(eCK eMember, Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eMember) {

    case CK_ABSJOIN:          *((Card**)&m_p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&m_p_aggreg)           = pCard; break;
    case CK_CALC:             *((Card**)&m_p_calc)             = pCard; break;
    case CK_BOOLCALC:         *((Card**)&m_p_calcBool)         = pCard; break;
    case CK_COMBINED:         *((Card**)&m_p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&m_p_comment)          = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&m_p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&m_p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&m_p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&m_p_coPtnPredList)    = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&m_p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&m_p_crossPtnJoinList) = pCard; break;
    case CK_EXPR:             *((Card**)&m_p_expr)             = pCard; break;
    case CK_FILTER:           *((Card**)&m_p_filter)           = pCard; break;
    case CK_GBYELIST:         *((Card**)&m_p_gbyElist)         = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&m_p_idxColList)       = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&m_p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&m_p_indexList)        = pCard; break;
    case CK_INDEXPTR:         *((Card**)&m_p_indexPtr)         = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&m_p_indexScan)        = pCard; break;
    case CK_JOINORDER:        *((Card**)&m_p_joinOrder)        = pCard; break;
    case CK_JOINORDERSEQ:     *((Card**)&m_p_joinOrderSeq)     = pCard; break;
    case CK_JOINPRED:         *((Card**)&m_p_joinPred)         = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&m_p_joinPredList)     = pCard; break;
    case CK_LBOUND:           *((Card**)&m_p_lBound)           = pCard; break;
    case CK_PERMUTATION:      *((Card**)&m_p_permutation)      = pCard; break;
    case CK_CMPPRED:          *((Card**)&m_p_cmpPred)          = pCard; break;
    case CK_PROJECT:          *((Card**)&m_p_project)          = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&m_p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&m_p_ptndJoinSet)      = pCard; break;
    case CK_SBYELIST:         *((Card**)&m_p_sbyElist)         = pCard; break;
    case CK_SCAN:             *((Card**)&m_p_scan)             = pCard; break;
    case CK_SCANPRED:         *((Card**)&m_p_scanPred)         = pCard; break;
    case CK_SORT:             *((Card**)&m_p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&m_p_stage)            = pCard; break;
    case CK_STAGETOPO:        *((Card**)&m_p_stageTopo)        = pCard; break;
    case CK_STREAM:           *((Card**)&m_p_stream)           = pCard; break;
    case CK_STREAMCOLLIST:    *((Card**)&m_p_streamColList)    = pCard; break;
    case CK_SUBSET:           *((Card**)&m_p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&m_p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&m_p_tableList)        = pCard; break;
    case CK_TABLEPTN:         *((Card**)&m_p_tablePtn)         = pCard; break;
    case CK_TABLEPTR:         *((Card**)&m_p_tablePtr)         = pCard; break;
    case CK_TERM:             *((Card**)&m_p_term)             = pCard; break;
    case CK_UBOUND:           *((Card**)&m_p_uBound)           = pCard; break;
    case CK_WHERE:            *((Card**)&m_p_where)            = pCard; break;

    case CK_SEND:
      break;

    default:
      NEVER_REACH
  }
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

  if ((m_p_SubMember = pMember->pFindSubCard(eSubCard))) {
    m_e_SubMember = m_p_SubMember->eCard();
    if (bUpdateMember)
      U_vUpdateMember(m_e_SubMember, m_p_SubMember);
    return true;
  }

  if (bUpdateMember)
    U_vUpdateMember(eSubCard, nullptr);
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
