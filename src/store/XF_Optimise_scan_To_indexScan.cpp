/*============================================================================*/
/* XF_Optimise_scan_To_indexScan.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Optimise_scan_To_indexScan.hpp"

#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_scan_To_indexScan, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN)
__aNewFunction(XF_Optimise_scan_To_indexScan, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN)
__vDeleteFunction(XF_Optimise_scan_To_indexScan, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN)
__aFooterFunction(XF_Optimise_scan_To_indexScan)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_scan_To_indexScan::XF_Optimise_scan_To_indexScan(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN, OT_SCAN_TO_INDEXSCAN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN, _FOOTER_);

  _checkInitialised_(CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_scan_To_indexScan::~XF_Optimise_scan_To_indexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_scan_To_indexScan::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_scan_To_indexScan* pXF_Morph_scan_To_indexScan = new XF_Optimise_scan_To_indexScan(pCI);

  bool b1 = pXF_Morph_scan_To_indexScan->bOptimise();

  pXF_Morph_scan_To_indexScan->vOptimised(b1);

  delete pXF_Morph_scan_To_indexScan;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_scan_To_indexScan::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_OPTIMISE_SCAN_TO_INDEXSCAN, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_SCAN_TO_INDEXSCAN, "morph scan to indexScan"_Stz);

  m_pCI->vClearRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {

    if (m_pCI->G.p_bounds->pSub_bound()->bIsLBound() ||
        m_pCI->G.p_bounds->pSub_bound()->bIsUBound()) {

      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);
      bOptRun = true;

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vEndTransform();
//vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
