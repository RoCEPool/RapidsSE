/*============================================================================*/
/* XF_Cut_sort.cpp -- Cut Sort                                                */
/*============================================================================*/

#include "XF_Cut_sort.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Cut_sort, CN_XF_CUT_SORT)
__aNewFunction(XF_Cut_sort, CN_XF_CUT_SORT)
__vDeleteFunction(XF_Cut_sort, CN_XF_CUT_SORT)
__aFooterFunction(XF_Cut_sort)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Cut_sort::XF_Cut_sort(CardIter* pCI)
: XF_TRANSFORM(CN_XF_CUT_SORT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CUT_SORT, _FOOTER_);

  _checkInitialised_(CN_XF_CUT_SORT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_CUT_SORT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Cut_sort::~XF_Cut_sort(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CUT_SORT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Cut_sort::vXF_Cut_sort(CardIter* pCI)
{
  _static_

  XF_Cut_sort* pXF_Cut_sort = new XF_Cut_sort(pCI);

  pXF_Cut_sort->vXF_Cut_sort();

  delete pXF_Cut_sort;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Cut_sort::vXF_Cut_sort(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CUT_SORT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_CUT_SORT, "Cut sort"_Stz);

  m_pCI->vClearRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_SORT:
        m_pCI->G.p_sort->vCut_sort(m_pCI);
        break;

      default:
        if (m_pCI->G.p_MainCard->pSub_projection()) {
          m_pCI->StreamOrder.m_pProjection_running = m_pCI->G.p_MainCard->pSub_projection();
          m_pCI->StreamOrder.m_pStreamKey_running = m_pCI->StreamOrder.m_pProjection_running->pSub_streamKey();
        }
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
