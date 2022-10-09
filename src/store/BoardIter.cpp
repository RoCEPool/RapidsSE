/*============================================================================*/
/* BoardIter.cpp --                                                           */
/*============================================================================*/

#include "Board.hpp"

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
BoardIter::BoardIter(Board* pBoard)
: CardIter(CN_BOARDITER,pBoard->pCard_Board())
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);

  m_pBoard    = pBoard;

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
void BoardIter::vApplyMain(eCK eCard, std::function<void(Card* pCard)> vfApply)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  while (Card* pCard = pEachMain(eCard))
    vfApply(pCard);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Main;

  m_pCurrChild = pEachMain(i_Main);

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(i8& ri_Main)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pBoard->nMainCards()) {
    m_pCurrChild = nullptr;
    ri_Main = C_UNKNOWN;
    return nullptr;
  }

  m_pCurrChild = m_pBoard->pGetMain(m_i_Child);
  ri_Main = m_i_Child;

  return m_pCurrChild;
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

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  for (;;) {

    if (m_i_Child >= m_pBoard->nMainCards()) {
      m_pCurrChild = nullptr;
      ri_Main = C_UNKNOWN;
      return nullptr;
    }

    m_pCurrChild = m_pBoard->pGetMain(m_i_Child);

    eCK eCardMain = m_pCurrChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = m_i_Child;
      return m_pCurrChild;
    }
    m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pBoard->nMainCards()) {
    m_pCurrChild = nullptr;
    ri_Main = C_UNKNOWN;
    reCard = CK_NONE;
    return nullptr;
  }

  m_pCurrChild = m_pBoard->pGetMain(m_i_Child);

  reCard = m_pCurrChild->eCard();
  ri_Main = m_i_Child;

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pNextMain(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Next = m_i_Child + 1;

  if (i_Next >= m_pBoard->nMainCards())
    return nullptr;

  Card* pNextMain = m_pBoard->pGetMain(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pNextMainIf(eCK eCard)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card* pNextMain = this->pNextMain();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pPrevMain(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Prev = m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pPrevMain = m_pBoard->pGetMain(i_Prev);

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Sort* BoardIter::pEach_sort(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Sort* p_sort = _pCard_Sort(pEachMain(CK_SORT));

  m_pCurrChild = p_sort;

  return p_sort;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Aggreg* BoardIter::pPrev_aggreg(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  Card_Aggreg* p_aggreg =
    _pCard_Aggreg(m_pBoard->pFindPrevMain(m_pCurrChild,CK_AGGREG,m_i_Child));


  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool BoardIter::bMore(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Child + 1 < m_pBoard->nMainCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSkip(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child++;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCard(Card* pCard)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  if (pCard == m_pCurrChild) {
    vRemoveCurr(m_pCurrChild->eCard());
    return;
  }

  m_pBoard->vRemoveMainCard(pCard);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrChild->eCard());
  m_pBoard->vRemoveMainCard(m_pCurrChild);
  m_i_Child--;
  if (m_i_Child < m_pBoard->nMainCards())
    m_pCurrChild = m_pBoard->pGetMain(m_i_Child);
  else
    m_pCurrChild = nullptr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrChild->eCard());
  m_pBoard->vRemoveMainCard(m_pCurrChild);
  delete m_pCurrChild;
  m_i_Child--;
  if (m_i_Child < m_pBoard->nMainCards())
    m_pCurrChild = m_pBoard->pGetMain(m_i_Child);
  else
    m_pCurrChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRestart(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child    = C_UNKNOWN;
  m_pCurrChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSet(Card* pCurrMain)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = m_pBoard->iFindMain(pCurrMain);

  if (C_UNKNOWN == m_i_Child)
    m_pCurrChild = nullptr;
  else
    m_pCurrChild = pCurrMain;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSet(i8 i_Main, Card* pCurrMain)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = i_Main;
  m_pCurrChild = pCurrMain;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
