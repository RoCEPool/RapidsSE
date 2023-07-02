/*============================================================================*/
/* XF_as_Review.cpp -- As Review                                              */
/*============================================================================*/

#include "XF_as_Review.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_as_Review, CN_XF_AS_REVIEW)
__aNewFunction(XF_as_Review, CN_XF_AS_REVIEW)
__vDeleteFunction(XF_as_Review, CN_XF_AS_REVIEW)
__aFooterFunction(XF_as_Review)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_as_Review::XF_as_Review(CardIter* pCI)
: XF_TRANSFORM(CN_XF_AS_REVIEW, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);

  _checkInitialised_(CN_XF_AS_REVIEW);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_as_Review::~XF_as_Review(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_as_Review::vXF_as_Review(CardIter* pCI)
{
  _static_

  XF_as_Review* pXF_as_Review = new XF_as_Review(pCI);

  pXF_as_Review->vXF_as_Review();

  delete pXF_as_Review;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_as_Review::vXF_as_Review(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);

  m_pCI->vTransform(__func__, "as review"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_GBYAGG:
        m_pCI->G.p_gbyAgg->vAs_Review(m_pCI);
        break;

      case CK_INDEXJOIN:  break;
      case CK_INDEXSCAN:  break;
      case CK_PROJECT:    break;
      case CK_SORT:       break;

      case CK_TABLE:
        m_pCI->StreamOrder.m_pProjection_running = m_pCI->G.p_table->pSub_projection();
        m_pCI->StreamOrder.m_pKey_running = nullptr;
        break;

      default:
        break;
    }

    m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
