/*============================================================================*/
/* XF_Optimise_Cut_sort.cpp -- Cut Sort                                                */
/*============================================================================*/

#include "XF_Optimise_Cut_sort.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Cut_sort, CN_XF_OPTIMISE_CUT_SORT)
__aNewFunction(XF_Optimise_Cut_sort, CN_XF_OPTIMISE_CUT_SORT)
__vDeleteFunction(XF_Optimise_Cut_sort, CN_XF_OPTIMISE_CUT_SORT)
__aFooterFunction(XF_Optimise_Cut_sort)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cut_sort::XF_Optimise_Cut_sort(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_CUT_SORT, OT_CUT_SORT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CUT_SORT, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_CUT_SORT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CUT_SORT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cut_sort::~XF_Optimise_Cut_sort(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CUT_SORT, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CUT_SORT, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_CUT_SORT, "Cut sort"_Stz);

  m_pCI->vClearRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pEmitted);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey);

  while (m_pCI->G_bEachMain_NONE()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_SORT:
        if (! m_pCI->StreamOrder.m_pStreamKey)
         break;

        m_pCI->G.p_key = m_pCI->G.p_sort->pSub_key();
        if (! m_pCI->StreamOrder.m_pStreamKey->bIsEquivalentTo(m_pCI->G.p_key))
          break;

        m_pCI->vRemoveDeleteCurr(CK_SORT);
//        if (m_pCI->G_bIsNextMain(CK_SORT)) {
//          m_pCI->G_bEachMain_NONE();
//          m_pCI->vRemoveDeleteCurr(CK_SORT);
//          if (m_pCI->G_bIsNextMain(CK_SORT)) {
//            m_pCI->G_bEachMain_NONE();
//            m_pCI->vRemoveDeleteCurr(CK_SORT);
//          }
//        }

        bOptRun = true;
        break;

      default:
        if (m_pCI->G.p_MainCard->pSub_emitted()) {
          m_pCI->StreamOrder.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
          m_pCI->StreamOrder.m_pStreamKey = m_pCI->StreamOrder.m_pEmitted->pSub_streamKey();
        }
        break;
    }
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
