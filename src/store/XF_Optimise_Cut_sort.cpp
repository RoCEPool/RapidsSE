/*============================================================================*/
/* XF_Optimise_Cut_sort.cpp -- Cut Sort                                                */
/*============================================================================*/

#include "XF_Optimise_Cut_sort.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Cut_sort, CN_XF_OT_CUT_SORT)
__aNewFunction(XF_Optimise_Cut_sort, CN_XF_OT_CUT_SORT)
__vDeleteFunction(XF_Optimise_Cut_sort, CN_XF_OT_CUT_SORT)
__aFooterFunction(XF_Optimise_Cut_sort)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cut_sort::XF_Optimise_Cut_sort(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OT_CUT_SORT, OT_CUT_SORT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_CUT_SORT, _FOOTER_);

  _checkInitialised_(CN_XF_OT_CUT_SORT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_CUT_SORT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cut_sort::~XF_Optimise_Cut_sort(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_CUT_SORT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Cut_sort::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Cut_sort* pXF_Cut_sort = new XF_Optimise_Cut_sort(pCI);

  bool b1 = pXF_Cut_sort->bOptimise();

  pXF_Cut_sort->vOptimised(b1);

  delete pXF_Cut_sort;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Cut_sort::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_CUT_SORT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_CUT_SORT, "Cut sort"_Stz);

  m_pCI->vClearRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_SORT:
        m_pCI->G.p_sort->vCut_sort(m_pCI);
        m_pCI->pBoard()->pState()->vInsert(aOT[OT_CUT_SORT]);

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

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
