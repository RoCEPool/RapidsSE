/*============================================================================*/
/* BoardIter.cpp --                                                           */
/*============================================================================*/

#include "BoardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BoardIter,CN_BOARDITER)
__aNewFunction(BoardIter,CN_BOARDITER)
__vDeleteFunction(BoardIter,CN_BOARDITER)
__aFooterFunction(BoardIter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BoardIter::BoardIter(TransformerTool* pTransformerTool)
: Object(CN_BOARDITER)
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);

  m_pTransformerTool = pTransformerTool;
  m_pBoard    = m_pTransformerTool->pBoard();
  m_i_Main    = C_UNKNOWN;
  m_pCurrMain = nullptr;

  _checkInitialised_(CN_BOARDITER);
  
  _postcondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BoardIter::~BoardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Main)
    m_i_Main = 0;
  else
    m_i_Main++;

  if (m_i_Main > m_pBoard->nMainCards()) {
    m_pCurrMain = nullptr;
    return nullptr;
  }

  m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);

  return m_pCurrMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(eCK eCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Main)
    m_i_Main = 0;
  else
    m_i_Main++;

  for (;;) {

    if (m_i_Main > m_pBoard->nMainCards()) {
      m_pCurrMain = nullptr;
      return nullptr;
    }

    m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);

    eCK eCardMain = m_pCurrMain->eCard();
    if (eCardMain == eCard)
      return m_pCurrMain;

    m_i_Main++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Sort* BoardIter::pEach_sort(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Sort* p_sort = _pCard_Sort(pEachMain(CK_SORT));

  return p_sort;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Aggreg* BoardIter::pPrev_aggreg(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Aggreg* p_aggreg =
    _pCard_Aggreg(m_pBoard->pFindPrevMain(m_pCurrMain,CK_AGGREG));

  return p_aggreg;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrMain->eCard());
  m_pBoard->vRemoveMainCard(m_pCurrMain);
  m_pCurrMain = nullptr;
  m_i_Main--;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrMain->eCard());
  m_pBoard->vRemoveMainCard(m_pCurrMain);
  delete m_pCurrMain;
  m_pCurrMain = nullptr;
  m_i_Main--;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
