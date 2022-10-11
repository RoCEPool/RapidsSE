/*============================================================================*/
/* TransformerTool.cpp --                                                     */
/*============================================================================*/

#include "TransformerTool.hpp"

#include "Job.hpp"
#include "TagSet.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TransformerTool::TransformerTool(eCN eClass, Bench* pBench)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pBench           = pBench;
  m_pBoard           = m_pBench->pHead();
  m_pBoard_costEst   = nullptr;
  m_pBoard_partition = nullptr;
  m_pBoard_permute   = nullptr;
  m_pBoard_query     = nullptr;
  m_pCardCtl         = m_pBench->pCardCtl();
  m_bPrintTransformation   = true;

  m_pTable           = nullptr;
  m_pStreamDef       = nullptr;
  m_pIdxVec          = nullptr;
  m_pIndex           = nullptr;

  m_pPermGen         = nullptr;

  m_eMainCard      = CK_NONE;
  m_eNextMain      = CK_NONE;
  m_eStageTopo     = YG_NONE;

  m_pBoardIter     = new BoardIter(m_pBoard);

//  m_pChildCard     = nullptr;
//  m_i_Child        = C_UNKNOWN;
//  m_pDupCard       = nullptr;
//  m_pMainCard      = nullptr;
//  m_i_Main         = C_UNKNOWN;
//  m_pNextMain      = nullptr;
//  m_i_Next         = C_UNKNOWN;
//  m_pPoppedCard    = nullptr;
//  m_i_Popped       = C_UNKNOWN;
//
//  m_p_absJoin          = nullptr;
//  m_p_aggreg           = nullptr;
//  m_p_calcBool         = nullptr;
//  m_p_combined         = nullptr;
//  m_p_coPtnJoin        = nullptr;
//  m_p_coPtnJoinList    = nullptr;
//  m_p_coPtnPred        = nullptr;
//  m_p_coPtnPredList    = nullptr;
//  m_p_crossPtnJoin     = nullptr;
//  m_p_crossPtnJoinList = nullptr;
//  m_p_comment          = nullptr;
//  m_p_expr             = nullptr;
//  m_p_filter           = nullptr;
//  m_p_gbyElist         = nullptr;
//  m_p_idxColList       = nullptr;
//  m_p_indexJoin        = nullptr;
//  m_p_indexList        = nullptr;
//  m_p_indexPtr         = nullptr;
//  m_p_indexScan        = nullptr;
//  m_p_joinOrder        = nullptr;
//  m_p_joinOrderSeq     = nullptr;
//  m_p_joinPred         = nullptr;
//  m_p_joinPredList     = nullptr;
//  m_p_lBound           = nullptr;
//  m_p_permutation      = nullptr;
//  m_p_permutation_low  = nullptr;
//  m_p_predCmp          = nullptr;
//  m_p_project          = nullptr;
//  m_p_ptndJoin         = nullptr;
//  m_p_ptndJoinSet      = nullptr;
//  m_p_sbyElist         = nullptr;
//  m_p_scan             = nullptr;
//  m_p_scanPred         = nullptr;
//  m_p_sort             = nullptr;
//  m_p_stage            = nullptr;
//  m_p_stage_a          = nullptr;
//  m_p_stage_b          = nullptr;
//  m_p_stageTopo        = nullptr;
//  m_p_stageTopo_a      = nullptr;
//  m_p_stageTopo_b      = nullptr;
//  m_p_stream           = nullptr;
//  m_p_streamColList    = nullptr;
//  m_p_subSet           = nullptr;
//  m_p_table            = nullptr;
//  m_p_tableList        = nullptr;
//  m_p_tablePtn         = nullptr;
//  m_p_tablePtr         = nullptr;
//  m_p_tableSyn         = nullptr;
//  m_p_term             = nullptr;
//  m_p_term_0           = nullptr;
//  m_p_term_1           = nullptr;
//  m_p_uBound           = nullptr;

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TransformerTool::~TransformerTool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  delete m_pBoardIter;
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
eCK TransformerTool::eDecodeMain(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_eMainCard = pCard->eCard();
  
  switch (m_eMainCard) {

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

  return m_eMainCard;
}

/*----------------------------------------------------------------------------*/
/* set explain                                                                */
/*----------------------------------------------------------------------------*/
void TransformerTool::vExplain(Stz* szExplain)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_sExplain = szExplain;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vPrintBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (m_bPrintTransformation) {
    if (m_bInteractive)
      m_pBoard->vPrintBoard(m_sTranformation.szStz(), m_sExplain);
  }
  else {
    vTrace("SILENT : %s\n"_Stz,m_sTranformation.szStz());
  }

  m_sExplain = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* search for parent of given sub card                                        */
/*----------------------------------------------------------------------------*/
bool TransformerTool::bRecursivelyTheSame(Card* p_card_a, Card* p_card_b, i8 iDepth, bool bTestL0)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  bool bSame = Card::bCardsSameRecursive(p_card_a, p_card_b,iDepth,bTestL0);

  return bSame;
}

/*----------------------------------------------------------------------------*/
/* set card print by kind                                                     */
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
/* record transformation                                                      */
/*----------------------------------------------------------------------------*/
void TransformerTool::vTransformation(eBK eBoard,
                                      char* azTranformation,
                                      bool bPrintTransformation)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  RASSERT(eBoard == m_pBoard->eBoard());

  _board_ = m_pBoard->pCard_Board();
  m_pBoardIter->vRestart();

  m_sTranformation = azTranformation;
  m_bPrintTransformation = bPrintTransformation;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetupCostEstBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pBoard_costEst = m_pBench->pGetBoard(BK_COSTEST);
  if (!m_pBoard_costEst)
    m_pBoard_costEst = new Board(BK_COSTEST, BT_ORIGINAL, m_pBench);
  m_pBoard = m_pBoard_costEst;

  m_pBoard_partition = m_pBench->pGetBoard(BK_PARTITION);
  m_pBoard_permute   = m_pBench->pGetBoard(BK_PERMUTE);
  m_pBoard_query     = m_pBench->pGetBoard(BK_QUERY);
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
  m_pBoard = m_pBoard_partition;

  m_pBoard_permute = m_pBench->pGetBoard(BK_PARTITION);
  m_pBoard_query   = m_pBench->pGetBoard(BK_QUERY);
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
  m_pBoard = m_pBoard_permute;

  m_pBoard_partition = m_pBench->pGetBoard(BK_PARTITION);
  m_pBoard_query     = m_pBench->pGetBoard(BK_QUERY);
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
  m_pBoard = m_pBoard_query;

  m_pBoard_partition = m_pBench->pGetBoard(BK_PARTITION);
  m_pBoard_query     = m_pBench->pGetBoard(BK_QUERY);
}


/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
bool TransformerTool::I_bEach_mainCard_(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pMainCard = m_pBoardIter->pEachMain();
  if (! m_pMainCard)
    return false;

  eCK eCard = m_pMainCard->eCard();

  vSetMember(eCard,m_pMainCard);

  return true;
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
bool TransformerTool::I_bEach_mainCard_(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  m_pMainCard = m_pBoardIter->pEachMain(eCard);
  if (! m_pMainCard)
    return false;

  vSetMember(eCard,m_pMainCard);

  return true;
}


/*----------------------------------------------------------------------------*/
/* set member card                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vSetMember(eCK eCard, Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  switch (eCard) {

    case CK_ABSJOIN:          *((Card**)&m_p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&m_p_aggreg)           = pCard; break;
    case CK_CALCBOOL:         *((Card**)&m_p_calcBool)         = pCard; break;
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
    case CK_CMPPRED:          *((Card**)&m_p_predCmp)          = pCard; break;
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

    case CK_SEND:
      break;

    default:
      NEVER_REACH  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
