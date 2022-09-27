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

  i8 i_Main;

  m_pCurrMain = pEachMain(i_Main);

  return m_pCurrMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(i8& ri_Main)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Main)
    m_i_Main = 0;
  else
    m_i_Main++;

  if (m_i_Main >= m_pBoard->nMainCards()) {
    m_pCurrMain = nullptr;
    ri_Main = C_UNKNOWN;
    return nullptr;
  }

  m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);
  ri_Main = m_i_Main;

  return m_pCurrMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(eCK eCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Main;

  Card* pMain = pEachMain(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Main)
    m_i_Main = 0;
  else
    m_i_Main++;

  for (;;) {

    if (m_i_Main >= m_pBoard->nMainCards()) {
      m_pCurrMain = nullptr;
      ri_Main = C_UNKNOWN;
      return nullptr;
    }

    m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);

    eCK eCardMain = m_pCurrMain->eCard();
    if (eCardMain == eCard) {
      ri_Main = m_i_Main;
      return m_pCurrMain;
    }
    m_i_Main++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pNextMain(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Next = m_i_Main + 1;

  if (i_Next >= m_pBoard->nMainCards())
    return nullptr;

  Card* pNextMain = m_pBoard->pGetMainCard(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pPrevMain(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Prev = m_i_Main - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pPrevMain = m_pBoard->pGetMainCard(i_Prev);

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Sort* BoardIter::pEach_sort(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Sort* p_sort = _pCard_Sort(pEachMain(CK_SORT));

  m_pCurrMain = p_sort;

  return p_sort;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Stage* BoardIter::pFirst_stage(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Stage* p_stage =
      _pCard_Stage(m_pBoard->pFindMain(CK_STAGE,m_i_Main));

  m_pCurrMain = p_stage;

  return p_stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Stage* BoardIter::pNext_stage(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Stage* p_stage =
      _pCard_Stage(m_pBoard->pFindNextMain(m_pCurrMain, CK_STAGE, m_i_Main));

  m_pCurrMain = p_stage;

  return p_stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Aggreg* BoardIter::pPrev_aggreg(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Aggreg* p_aggreg =
    _pCard_Aggreg(m_pBoard->pFindPrevMain(m_pCurrMain,CK_AGGREG,m_i_Main));

  m_pCurrMain = p_aggreg;

  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool BoardIter::bMore(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Main + 1 < m_pBoard->nMainCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSkip(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Main++;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrMain->eCard());
  m_pBoard->vRemoveMainCard(m_pCurrMain);
  m_i_Main--;
  if (m_i_Main < m_pBoard->nMainCards())
    m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);
  else
    m_pCurrMain = nullptr;
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
  m_i_Main--;
  if (m_i_Main < m_pBoard->nMainCards())
    m_pCurrMain = m_pBoard->pGetMainCard(m_i_Main);
  else
    m_pCurrMain = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
