/*============================================================================*/
/* TransformerTool.cpp --                                                              */
/*============================================================================*/

#include "TransformerTool.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TransformerTool::TransformerTool(eCN eClass, Bench* pBench)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pBench = pBench;
  m_pBoard = m_pBench->pHead();

  m_pMainCardIter = nullptr;

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TransformerTool::~TransformerTool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  delete m_pMainCardIter;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void TransformerTool::vBeginIteration(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  RASSERT(! m_pMainCardIter);

  m_pMainCardIter = m_pBoard->pNewMainCardIter();
}

/*----------------------------------------------------------------------------*/
/* each main card                                                             */
/*----------------------------------------------------------------------------*/
Card* TransformerTool::pEachMain(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  while (m_pMainCardIter->bHasNext()) {
    Card* pMainCard = m_pMainCardIter->pNext();

    return pMainCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* each main card which is of given eCard                                     */
/*----------------------------------------------------------------------------*/
Card* TransformerTool::pEachMain(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  while (m_pMainCardIter->bHasNext()) {
    Card* pMainCard = m_pMainCardIter->pNext();

    if (eCard == pMainCard->eCard())
      return pMainCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void TransformerTool::vEndIteration(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  RASSERT(m_pMainCardIter);

  delete m_pMainCardIter;
  m_pMainCardIter = nullptr;
}

/*----------------------------------------------------------------------------*/
/* search for parent of given sub card                                        */
/*----------------------------------------------------------------------------*/
Card* TransformerTool::pFindMainOf(Card* pGivenCard)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card *pMainCard = nullptr;

  CardIter *pMainCardIter = m_pBoard->pNewMainCardIter();

  while (pMainCardIter->bHasNext()) {
    pMainCard = pMainCardIter->pNext();

    if (pMainCard->bHasSubCard(pGivenCard))
      break;
  }

  delete pMainCardIter;

  return pMainCard;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
