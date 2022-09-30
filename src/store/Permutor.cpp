/*============================================================================*/
/* Permutor.cpp -- scan tool                                                    */
/*============================================================================*/

#include "Permutor.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Permutor, CN_PERMUTOR)
__aNewFunction(Permutor, CN_PERMUTOR)
__vDeleteFunction(Permutor, CN_PERMUTOR)
__aFooterFunction(Permutor)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Permutor::Permutor(Bench* pBench)
: TransformerTool(CN_PERMUTOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  _checkInitialised_(CN_PERMUTOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Permutor::~Permutor(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vCompute_coPtnCrossPtn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board *pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Card_CoPtnList* p_coPtnList = new Card_CoPtnList;

  pBoard_permute->vAppendMainCard(p_coPtnList);

  Card_CrossPtnList* p_crossPtnList = new Card_CrossPtnList;

  pBoard_permute->vAppendMainCard(p_crossPtnList);

  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vCompute_coPtnCrossPtn"_Stz);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vCompute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board *pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Card_CoPtnPredList* p_coPtnPredList = new Card_CoPtnPredList;

  pBoard_permute->vAppendMainCard(p_coPtnPredList);

  Card_TableList* p_tableList =
      _pCard_TableList(pBoard_permute->pFindMain(CK_TABLELIST));
  Card_JoinPredList* p_joinPredList =
      _pCard_JoinPredList(pBoard_permute->pFindMain(CK_JOINPREDLIST));

  i8 nJoinPreds = p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    Card_PredCmp* p_predCmp =
        _pCard_PredCmp(p_joinPredList->pGetChild(i_joinPred));

//    Card_ColumnRef* p_columnRef_0 =
//        _pCard_ColumnRef(p_predCmp->pGetChild(0));
//    Card_ColumnRef* p_columnRef_1 =
//        _pCard_ColumnRef(p_predCmp->pGetChild(1));

    if (! p_tableList->bPredCmpIsCoPtn(p_predCmp))
      continue;

    Card_CoPtnPred* p_coPtnPred = new Card_CoPtnPred;
//    p_coPtnPred->vAppend(p_columnRef_0->pDupCardAndSubCards());
//    p_coPtnPred->vAppend(p_columnRef_1->pDupCardAndSubCards());
    p_coPtnPred->vAppend(p_predCmp->pDupCardAndSubCards());

    p_coPtnPredList->vAppend(p_coPtnPred);
  }

  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vCompute_coPtnPredList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vExtract_tableList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board* pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Board* pBoard_query = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI_query(pBoard_query);

  Card_TableList* p_tableList_new = new Card_TableList;

  while ( (m_pMainCard = xBI_query.pEachMain()) ) {
    Card_Table* p_table = m_pMainCard->pSub_table();
    if (! p_table)
      continue;

    p_tableList_new->vAppend(p_table->pDupCardAndSubCards());
  }

  pBoard_permute->vAppendMainCard(p_tableList_new);

  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vExtract_tableList"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vExtract_joinPredList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board *pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Board *pBoard_query = m_pBench->pGetBoard(BK_QUERY);


  Card_JoinPredList *p_joinPredList = new Card_JoinPredList;

  BoardIter xBI_query(pBoard_query);
  while ((m_pMainCard = xBI_query.pEachMain(CK_FILTER))) {
    Card_CalcBool *p_calcBool = m_pMainCard->pSub_calcBool();
    if (!p_calcBool)
      continue;

    p_joinPredList->vExtract_joinPredList(p_calcBool);
  }

  pBoard_permute->vAppendMainCard(p_joinPredList);

  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vExtract_joinPredList"_Stz);

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
