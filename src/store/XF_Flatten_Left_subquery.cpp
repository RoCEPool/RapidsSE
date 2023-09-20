/*============================================================================*/
/* XF_Flatten_Left_subQuery.cpp -- Left SubQuery                              */
/*============================================================================*/

#include "XF_Flatten_Left_subQuery.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Left_subQuery, CN_XF_FLATTEN_LEFT_SUBQUERY)
__aNewFunction(XF_Flatten_Left_subQuery, CN_XF_FLATTEN_LEFT_SUBQUERY)
__vDeleteFunction(XF_Flatten_Left_subQuery, CN_XF_FLATTEN_LEFT_SUBQUERY)
__aFooterFunction(XF_Flatten_Left_subQuery )


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Left_subQuery::XF_Flatten_Left_subQuery(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_LEFT_SUBQUERY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_LEFT_SUBQUERY, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_LEFT_SUBQUERY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_LEFT_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Left_subQuery::~XF_Flatten_Left_subQuery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_LEFT_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* remove left-most subquery and rotate next sub-query to be first card       */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Left_subQuery::vRotate_Next(CardIter* pCI)
{
  _static_

  XF_Flatten_Left_subQuery* pXF_Delete_flat = new XF_Flatten_Left_subQuery(pCI);

  pXF_Delete_flat->vRotate_Next();

  delete pXF_Delete_flat;
}

/*----------------------------------------------------------------------------*/
/* remove left-most subquery and rotate next sub-query to be first card       */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Left_subQuery::vRemove_LeftMost(CardIter* pCI)
{
  _static_

  XF_Flatten_Left_subQuery* pXF_Delete_flat = new XF_Flatten_Left_subQuery(pCI);

  pXF_Delete_flat->vRemove_LeftMost();

  delete pXF_Delete_flat;
}



/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove left-most subquery and rotate next sub-query to be first card       */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Left_subQuery::vRemove_LeftMost(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_LEFT_SUBQUERY, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_LEFT_SUBQUERY, "left subQuery"_Stz);

  Kid xDeck(m_pCI->G.pDeck);
  if (xDeck.bEach()) {
    if (CK_SUBQUERY == xDeck.eChild()) {
      xDeck.vDelete();
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* remove left-most subquery and rotate next sub-query to be first card       */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Left_subQuery::vRotate_Next(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_LEFT_SUBQUERY, _FOOTER_);

  //m_pCI->vTransform(__func__, XF_FLATTEN_LEFT_SUBQUERY, "left subQuery"_Stz);

  Kid xDeck(m_pCI->G.pDeck);
  if (xDeck.bEach(CK_SUBQUERY)) {
    m_pCI->G.pDeck->vRotateFirst(xDeck.pChild());
  }

  //m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
