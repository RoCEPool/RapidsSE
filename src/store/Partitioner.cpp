/*============================================================================*/
/* Partitioner.cpp -- partitioner tool                                        */
/*============================================================================*/

#include "Partitioner.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Partitioner, CN_PARTITIONER)
__aNewFunction(Partitioner, CN_PARTITIONER)
__vDeleteFunction(Partitioner, CN_PARTITIONER)
__aFooterFunction(Partitioner)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partitioner::Partitioner(Bench* pBench)
: TransformerTool(CN_PARTITIONER, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  _checkInitialised_(CN_PARTITIONER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partitioner::~Partitioner(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vExtract_tableList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // tableList

  BoardIter xBI_query(m_pBoard_query);

  m_p_tableList = new Card_TableList;
  m_pBoard_partition->vAppendMainCard(m_p_tableList);

  while ( (m_pMainCard = xBI_query.pEachMain()) ) {
    m_p_table = m_pMainCard->pSub_table();
    if (! m_p_table)
      continue;

    m_p_tableList->vAppend(m_p_table->pDupCardAndSubCards());
  }


  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vExtract_tableList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vExtract_joinPredList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // joinPredList

  m_p_joinPredList = new Card_JoinPredList;
  m_pBoard_partition->vAppendMainCard(m_p_joinPredList);

  BoardIter xBI_query(m_pBoard_query);
  while ((m_pMainCard = xBI_query.pEachMain(CK_FILTER))) {
    m_p_calcBool = m_pMainCard->pSub_calcBool();
    if (!m_p_calcBool)
      continue;

    m_p_joinPredList->vExtract_joinPredList(m_p_calcBool);
  }


  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vExtract_joinPredList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vCompute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // coPptnPredList

  m_p_coPtnPredList = new Card_CoPtnPredList;
  m_pBoard_partition->vAppendMainCard(m_p_coPtnPredList);

  m_p_tableList =
      _pCard_TableList(m_pBoard_partition->pFindMain(CK_TABLELIST));
  m_p_joinPredList =
      _pCard_JoinPredList(m_pBoard_partition->pFindMain(CK_JOINPREDLIST));

  i8 nJoinPreds = m_p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    m_p_predCmp =
        _pCard_PredCmp(m_p_joinPredList->pGetChild(i_joinPred));

    if (! m_p_tableList->bPredCmpIsCoPtn(m_p_predCmp))
      continue;

    m_p_coPtnPred = new Card_CoPtnPred;
    m_p_coPtnPred->vAppend(m_p_predCmp->pDupCardAndSubCards());

    m_p_coPtnPredList->vAppend(m_p_coPtnPred);
  }


  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vCompute_coPtnPredList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vCompute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // coPtnJoinList

  m_p_coPtnJoinList = new Card_CoPtnJoinList;
  m_pBoard_partition->vAppendMainCard(m_p_coPtnJoinList);

  m_p_coPtnPredList =
      _pCard_CoPtnPredList(m_pBoard_partition->pFindMain(CK_COPTNPREDLIST));


  i8 nChildren = m_p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_p_coPtnPred =
        _pCard_CoPtnPred(m_p_coPtnPredList->pGetChild(i_Child));

    m_p_subSet = m_p_coPtnPred->pDupSubCards();

    if (! (m_p_coPtnJoin = m_p_coPtnJoinList->pFind_coPtn_for(m_p_coPtnPred)) ) {
      m_p_coPtnJoin = new Card_CoPtnJoin;
      m_p_coPtnJoinList->vAppend(m_p_coPtnJoin);
    }

    m_p_coPtnJoin->vTransferSubCards(m_p_subSet);
    DELETE_CARD(m_p_subSet);
  }



  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vCompute_coPtnJoinList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vCompute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // crossPtnJoinList

  m_p_joinPredList =
      _pCard_JoinPredList(m_pBoard_partition->pFindMain(CK_JOINPREDLIST));

  m_p_crossPtnJoinList = new Card_CrossPtnJoinList;
  m_pBoard_partition->vAppendMainCard(m_p_crossPtnJoinList);

  i8 nChildren = m_p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_p_predCmp =
        _pCard_PredCmp(m_p_joinPredList->pGetChild(i_Child));

    if (m_p_coPtnJoinList->bHasEqPredCmp(m_p_predCmp))
      continue;

    m_p_crossPtnJoin = new Card_CrossPtnJoin;
    m_p_crossPtnJoin->vAppend(m_p_predCmp->pDupCardAndSubCards());
    m_p_crossPtnJoinList->vAppend(m_p_crossPtnJoin);
  }

//  m_p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  m_p_crossPtnJoinList->vAppend(m_p_comment);


  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vCompute_crossPtnJoinList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Partitioner::vCompute_ptndJointSet(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARTITIONER, _FOOTER_);

  vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

  m_p_joinPtnList = new Card_PtndJoinSet;
  m_pBoard_partition->vAppendMainCard(m_p_joinPtnList);

  m_p_coPtnJoinList =
      _pCard_CoPtnJoinList(m_pBoard_partition->pFindMain(CK_COPTNJOINLIST));
  m_p_subSet = m_p_coPtnJoinList->pDupSubCards();
  m_p_joinPtnList->vTransferSubCards(m_p_subSet);
  DELETE_CARD(m_p_subSet);

  m_p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(m_pBoard_partition->pFindMain(CK_CROSSPTNJOINLIST));
  m_p_subSet = m_p_crossPtnJoinList->pDupSubCards();
  m_p_joinPtnList->vTransferSubCards(m_p_subSet);
  DELETE_CARD(m_p_subSet);

  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard("Partitioner::vCompute_ptndJointSet"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
