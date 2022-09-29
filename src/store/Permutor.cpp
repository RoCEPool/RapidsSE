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
void Permutor::vXyz(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  Board* pBoard_permute = m_pBench->pGetBoard(BK_PERMUTE);
  if (!pBoard_permute)
    pBoard_permute = new Board(BK_PERMUTE, m_pBench);

  Board* pBoard_query = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI_query(pBoard_query);

  Card_TableList* p_tableList = new Card_TableList;

  while ( (m_pMainCard = xBI_query.pEachMain()) ) {
    Card_TablePtr* p_tablePtr = m_pMainCard->pSub_tablePtr();
    if (! p_tablePtr)
      continue;

    Card_Table* p_table = new Card_Table;
    p_table->vAppend(p_tablePtr->pDupCardAndSubCards());

    p_tableList->vAppend(p_table);
  }

  pBoard_permute->vAppendMainCard(p_tableList);

  if (m_bInteractive)
    pBoard_permute->vPrintBoard("Permutor::vXyz"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
