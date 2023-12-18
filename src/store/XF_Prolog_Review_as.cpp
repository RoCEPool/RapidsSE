/*============================================================================*/
/* XF_Prolog_Review_as.cpp -- As Review                                              */
/*============================================================================*/

#include "XF_Prolog_Review_as.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Review_as, CN_XF_PROLOG_REVIEW_AS)
__aNewFunction(XF_Prolog_Review_as, CN_XF_PROLOG_REVIEW_AS)
__vDeleteFunction(XF_Prolog_Review_as, CN_XF_PROLOG_REVIEW_AS)
__aFooterFunction(XF_Prolog_Review_as)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Review_as::XF_Prolog_Review_as(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_REVIEW_AS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REVIEW_AS, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_REVIEW_AS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REVIEW_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Review_as::~XF_Prolog_Review_as(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REVIEW_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Review_as::vReview(CardIter* pCI)
{
  _static_

  XF_Prolog_Review_as* pXF_as_Review = new XF_Prolog_Review_as(pCI);

  pXF_as_Review->vReview_as();

  delete pXF_as_Review;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Review_as::vReview_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REVIEW_AS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_REVIEW_AS, "as review"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMain_NONE()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_o_GBYAGG:
        //m_pCI->G.p_gbyAgg->vAs_Review(m_pCI);
        m_pCI->G.p_key = m_pCI->G.p_gbyAgg->pSub_key();
        m_pCI->G.p_emit = m_pCI->G.p_gbyAgg->pSub_emit();
        //m_pCI->G.p_emit->vAs_Review(m_pCI);
        break;

      case CK_o_INDEXJOIN:  break;
      case CK_o_INDEXSCAN:  break;
      case CK_o_PROJECT:    break;
      case CK_o_SORT:       break;
      case CK_s_TABLE:      break;

      default:
        break;
    }

   // m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
