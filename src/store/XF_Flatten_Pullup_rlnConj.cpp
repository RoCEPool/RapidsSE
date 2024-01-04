/*============================================================================*/
/* XF_Flatten_Pullup_rlnConj.cpp -- Predicate Pull Up                              */
/*============================================================================*/

#include "XF_Flatten_Pullup_rlnConj.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Pullup_rlnConj, CN_XF_FLATTEN_PULLUP_RLNCONJ)
__aNewFunction(XF_Flatten_Pullup_rlnConj, CN_XF_FLATTEN_PULLUP_RLNCONJ)
__vDeleteFunction(XF_Flatten_Pullup_rlnConj, CN_XF_FLATTEN_PULLUP_RLNCONJ)
__aFooterFunction(XF_Flatten_Pullup_rlnConj)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Pullup_rlnConj::XF_Flatten_Pullup_rlnConj(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_PULLUP_RLNCONJ, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PULLUP_RLNCONJ, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_PULLUP_RLNCONJ);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PULLUP_RLNCONJ, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Pullup_rlnConj::~XF_Flatten_Pullup_rlnConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PULLUP_RLNCONJ, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Pullup_rlnConj::vXF_Pullup_rlnConj(CardIter* pCI)
{
  _static_

  XF_Flatten_Pullup_rlnConj* pXF_Predicate_Pull_Up = new XF_Flatten_Pullup_rlnConj(pCI);

  pXF_Predicate_Pull_Up->vXF_Pullup_rlnConj();

  delete pXF_Predicate_Pull_Up;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Pullup_rlnConj::vXF_Pullup_rlnConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PULLUP_RLNCONJ, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_PULLUP_RLNCONJ, "use @c to move predicates"_Stz);

  // @ mark where predicates wrt. table
  Card_Where::vMarkWhereConjWrtTable(m_pCI);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__func__,__LINE__);

  // move @ marked where predicates to table filter
  Card_Where::vMoveWhereConjToTableFilter(m_pCI, MARK_AT);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__func__,__LINE__);

  // cull @ marked where predicates
  Card_Where::vCullMarked_rlnConj(m_pCI);

//  m_pCI->vRenumberCards();
//  m_pCI->vPrintBoard(__func__,__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
