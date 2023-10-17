/*============================================================================*/
/* XF_Flatten_Predicate_Pull_Up.cpp -- Predicate Pull Up                              */
/*============================================================================*/

#include "XF_Flatten_Predicate_Pull_Up.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Predicate_Pull_Up, CN_XF_FLATTEN_PREDICATE_PULLUP)
__aNewFunction(XF_Flatten_Predicate_Pull_Up, CN_XF_FLATTEN_PREDICATE_PULLUP)
__vDeleteFunction(XF_Flatten_Predicate_Pull_Up, CN_XF_FLATTEN_PREDICATE_PULLUP)
__aFooterFunction(XF_Flatten_Predicate_Pull_Up)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Pull_Up::XF_Flatten_Predicate_Pull_Up(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_PREDICATE_PULLUP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_PULLUP, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_PREDICATE_PULLUP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_PULLUP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Pull_Up::~XF_Flatten_Predicate_Pull_Up(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_PULLUP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Predicate_Pull_Up::vXF_Predicate_Pull_Up(CardIter* pCI)
{
  _static_

  XF_Flatten_Predicate_Pull_Up* pXF_Predicate_Pull_Up = new XF_Flatten_Predicate_Pull_Up(pCI);

  pXF_Predicate_Pull_Up->vXF_Predicate_Pull_Up();

  delete pXF_Predicate_Pull_Up;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Predicate_Pull_Up::vXF_Predicate_Pull_Up(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_PULLUP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_PREDICATE_PULL_UP, "use @c to move predicates"_Stz);

  // @ mark where predicates wrt. table
  Card_Where::vMarkWherePredWrtTable(m_pCI);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__LINE__);

  // move @ marked where predicates to table filter
  Card_Where::vMoveWherePredsToTableFilter(m_pCI);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__LINE__);

  // cull @ marked where predicates
  Card_Where::vCullMarkedCmpConj(m_pCI);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
