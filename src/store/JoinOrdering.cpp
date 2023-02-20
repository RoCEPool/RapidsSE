/*============================================================================*/
/* JoinOrdering.cpp -- JoinOrdering                                           */
/*============================================================================*/

#include "JoinOrdering.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, JoinOrdering, CN_JOINORDERING)
__aNewFunction(JoinOrdering, CN_JOINORDERING)
__vDeleteFunction(JoinOrdering, CN_JOINORDERING)
__aFooterFunction(JoinOrdering)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JoinOrdering::JoinOrdering(i8 i_depth)
: Object(CN_JOINORDERING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = nullptr;

  m_pSubQuery    = nullptr;
  m_p_JoinOrdering       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JoinOrdering::JoinOrdering(i8 i_depth, Card_Deck* p_deck)
: Object(CN_JOINORDERING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = p_deck;

  m_pSubQuery    = nullptr;
  m_p_JoinOrdering       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JoinOrdering::~JoinOrdering(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* JoinOrdering::p_deck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  return m_p_deck;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::v_deck(Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_p_deck = p_deck;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 JoinOrdering::i_depth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  return m_i_depth;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering* JoinOrdering::p_JoinOrdering(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  return m_p_JoinOrdering;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::v_JoinOrdering(Card_JoinOrdering* p_JoinOrdering)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_p_JoinOrdering = p_JoinOrdering;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* JoinOrdering::pOuterQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  return m_pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vOuterQuery(SubQuery* pOuterQuery)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pOuterQuery = pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* JoinOrdering::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vSubQuery(SubQuery* pSubQuery)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pSubQuery = pSubQuery;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
