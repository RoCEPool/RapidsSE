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
void Permutor::vCompute_coPtnList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board *pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Card_CoPtnList* p_coPtnList = new Card_CoPtnList;

  pBoard_permute->vAppendMainCard(p_coPtnList);



  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vCompute_coPtnList"_Stz);

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
