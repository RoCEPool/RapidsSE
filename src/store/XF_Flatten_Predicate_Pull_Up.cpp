/*============================================================================*/
/* XF_Flatten_Predicate_Pull_Up.cpp -- Predicate Pull Up                              */
/*============================================================================*/

#include "XF_Flatten_Predicate_Pull_Up.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Predicate_Pull_Up, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Flatten_Predicate_Pull_Up, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Flatten_Predicate_Pull_Up, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Flatten_Predicate_Pull_Up)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Pull_Up::XF_Flatten_Predicate_Pull_Up(CardIter* pCI)
: XF_TRANSFORM(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Pull_Up::~XF_Flatten_Predicate_Pull_Up(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_PREDICATE_PULL_UP, "use @c to move predicates"_Stz);

  // @ mark where predicates wrt. table
  Card_Where::vMarkWherePredWrtTable(m_pCI);

  // move @ marked where predicates to table filter
  Card_Where::vMoveWherePredsToTableFilter(m_pCI);

  // cull @ marked where predicates
  Card_Where::vCullMarkedCmpPred(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
