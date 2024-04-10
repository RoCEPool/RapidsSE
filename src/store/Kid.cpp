/*============================================================================*/
/* Kid.cpp --  kid iterator                                                   */
/*============================================================================*/

#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kid::Kid(void)
{
  _nonProper_

  m_pParent   = nullptr;
  m_bBegun    = false;
  m_bForwards = true;
  m_bEnded    = false;

  vChildReset();

  _checkInitialised_(CN_KID);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kid::Kid(Card* pParent)
{
  _nonProper_

  m_pParent = pParent;

  vBegin();

  _checkInitialised_(CN_KID);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kid::Kid(Card* pParent, Card *pChild)
{
  _nonProper_

  m_pParent = pParent;

  vBegin();
  vReposition(pChild);

  _checkInitialised_(CN_KID);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Kid::~Kid(void) noexcept(false)
{
  _nonProper_
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vBackstep(void)
{
  _nonProper_

  if (m_bForwards) {
    m_i_Child--;
  }

  vChildUpdate();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vBegin(void)
{
  _nonProper_

  vChildReset();
  m_bBegun    = true;
  m_bForwards = true;
  m_bEnded    = false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vBegin_reverse(void)
{
  _nonProper_

  vChildReset();
  m_i_Child = m_pParent->nChildCards();

  m_bBegun    = true;
  m_bForwards = false;
  m_bEnded    = false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bBegun(void)
{
  _nonProper_

  return m_bBegun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCK Kid::eChild(void)
{
  _nonProper_

  if (m_pChild)
    return m_pChild->eCard();

  return CK_NONE;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Kid::i_Child(void)
{
  _nonProper_

  return m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pChild(void)
{
  _nonProper_

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vDelete(void)
{
  _nonProper_

  m_pParent->vDeleteChild(m_pChild);
  m_pChild = nullptr;

  vBackstep();
}

/*----------------------------------------------------------------------------*/
/* next child                                                                 */
/*----------------------------------------------------------------------------*/
bool Kid::bEach(void)
{
  _nonProper_

  RASSERT(m_bBegun);
  RASSERT(m_bForwards);

  RASSERT(CK_RPNCALC != m_pParent->eCard());

  m_i_Child++;
  vChildUpdate();

  if (bWithinRange()) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* next child                                                                 */
/*----------------------------------------------------------------------------*/
bool Kid::bEach_opaque(void)
{
  _nonProper_

  RASSERT(m_bBegun);
  RASSERT(m_bForwards);

  m_i_Child++;
  vChildUpdate();

  if (bWithinRange()) {
    return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/* next child of given card                                                   */
/*----------------------------------------------------------------------------*/
bool Kid::bEach(eCK eChild)
{
  _nonProper_

  while (bEach_opaque()) {
    if (this->eChild() == eChild)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* next child of given card                                                   */
/*----------------------------------------------------------------------------*/
bool Kid::bEach_opaque(eCK eChild)
{
  _nonProper_

  while (bEach_opaque()) {
    if (this->eChild() == eChild)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* next child of given card and sub1                                          */
/*----------------------------------------------------------------------------*/
bool Kid::bEach(eCK eChild, eCK eSub1)
{
  _nonProper_

  RASSERT(CK_RPNCALC != m_pParent->eCard());

  while (bEach()) {
    if (this->eChild() == eChild) {
      if (! (m_pSub1 = m_pChild->pFindSubCard(eSub1)) )
        continue;

      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* next child of given card and sub1 & sub2                                   */
/*----------------------------------------------------------------------------*/
bool Kid::bEach(eCK eChild, eCK eSub1, eCK eSub2)
{
  _nonProper_

  RASSERT(CK_RPNCALC != m_pParent->eCard());

  while (bEach()) {
    if (this->eChild() == eChild) {
      if (! (m_pSub1 = m_pChild->pFindSubCard(eSub1)) )
        continue;
      if (! (m_pSub2 = m_pChild->pFindSubCard(eSub2)) )
        continue;

      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* next child of given card and sub1 & sub2 & sub3                            */
/*----------------------------------------------------------------------------*/
bool Kid::bEach(eCK eChild, eCK eSub1, eCK eSub2, eCK eSub3)
{
  _nonProper_

  RASSERT(CK_RPNCALC != m_pParent->eCard());

  while (bEach()) {
    if (this->eChild() == eChild) {
      if (! (m_pSub1 = m_pChild->pFindSubCard(eSub1)) )
        continue;
      if (! (m_pSub2 = m_pChild->pFindSubCard(eSub2)) )
        continue;
      if (! (m_pSub3 = m_pChild->pFindSubCard(eSub3)) )
        continue;

      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bEach_reverse(void)
{
  _nonProper_

  RASSERT(m_bBegun);
  RASSERT(! m_bForwards);

  m_i_Child--;
  vChildUpdate();

  if (bWithinRange()) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bEach_reverse(eCK eChild)
{
  _nonProper_

  while (bEach_reverse())
    if (this->eChild() == eChild)
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bEnded(void)
{
  _nonProper_

  return m_bEnded;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vEnded(void)
{
  _nonProper_

  vChildReset();

  m_bEnded = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bForwards(void)
{
  _nonProper_

  return m_bForwards;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vForwards(bool bForwards)
{
  _nonProper_

  m_bForwards = bForwards;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bLess(void)
{
  _nonProper_

  if (m_i_Child > 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bMore(void)
{
  _nonProper_

  if (m_i_Child < m_pParent->nChildCards() - 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pNext(void)
{
  _nonProper_

  if (! bMore())
    return nullptr;

  Card* pNext = m_pParent->pGetChild(m_i_Child + 1);

  return pNext;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pParent(void)
{
  _nonProper_

  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vParent(Card* pParent)
{
  _nonProper_

  m_pParent = pParent;
  vChildReset();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bParentIs(eCK eParent)
{
  _nonProper_

  if (eParent == m_pParent->eCard())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pPrev(void)
{
  _nonProper_

  if (! bLess())
    return nullptr;

  Card* pPrev = m_pParent->pGetChild(m_i_Child - 1);

  return pPrev;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vRemove(void)
{
  _nonProper_

  m_pParent->vRemoveChild(m_pChild);
  m_pChild = nullptr;

  vBackstep();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vReposition(i8 i_Child)
{
  _nonProper_

  m_i_Child = i_Child;
  vChildUpdate();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Kid::vReposition(Card* pChild)
{
  _nonProper_

  vChildReset();

  if (m_bForwards) {
    while (bEach())
      if (pChild == m_pChild)
        break;
  }
  else {
    vBegin_reverse();
    while (bEach_reverse())
      if (pChild == m_pChild)
        break;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pSub1(void)
{
  _nonProper_

  return m_pSub1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pSub2(void)
{
  _nonProper_

  return m_pSub2;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Kid::pSub3(void)
{
  _nonProper_

  return m_pSub3;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Kid::bWithinRange(void)
{
  _nonProper_

  if (m_i_Child >= 0 && m_i_Child < m_pParent->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_AndConj* Kid::p_andConj(void)
{
  _nonProper_

  return _pCard_AndConj(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Bound* Kid::p_bound(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_bound();

  return _pCard_Bound(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Correlation* Kid::p_correlation(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_correlation();

  return _pCard_Correlation(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Kid::p_expr(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_expr();

  return _pCard_Expr(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Fold* Kid::p_fold(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_fold();

  return _pCard_Fold(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_HshCol* Kid::p_hshCol(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_hshCol();

  return _pCard_HshCol(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Kid::p_idxCol(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_idxCol();

  return _pCard_IdxCol(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Index* Kid::p_index(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_index();

  return _pCard_Index(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_JoinOrder* Kid::p_joinOrder(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_joinOrder();

  return _pCard_JoinOrder(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Map* Kid::p_map(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_map();

  return _pCard_Map(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Kid::p_rlnConj(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_rlnConj();

  return _pCard_RlnConj(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Stage* Kid::p_stage(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_stage();

  return _pCard_Stage(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamHead* Kid::p_streamHead(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_streamHead();

  return _pCard_StreamHead(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamJoin* Kid::p_streamJoin(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_streamJoin();

  return _pCard_StreamJoin(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamKey* Kid::p_streamKey(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_streamKey();

  return _pCard_StreamKey(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Kid::p_streamPer(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_streamPer();

  return _pCard_StreamPer(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Kid::p_table(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_table();

  return _pCard_Table(pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Kid::p_term(bool bSub)
{
  _nonProper_

  if (bSub)
    return pChild()->pSub_term();

  return _pCard_Term(pChild());
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* reset child                                                                */
/*----------------------------------------------------------------------------*/
void Kid::vChildReset(void)
{
  _nonProper_

  m_i_Child = C_UNKNOWN;
  m_pChild  = nullptr;

  m_pSub1 = nullptr;
  m_pSub2 = nullptr;
  m_pSub3 = nullptr;

  m_bEnded = false;
}

/*----------------------------------------------------------------------------*/
/* update child                                                               */
/*----------------------------------------------------------------------------*/
void Kid::vChildUpdate(void)
{
  _nonProper_

  i8 nChildren = m_pParent->nChildCards();

  if (m_i_Child >= 0 && m_i_Child < nChildren) {
    m_pChild = m_pParent->pGetChild(m_i_Child);
  }
  else {
    m_pChild = nullptr;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
