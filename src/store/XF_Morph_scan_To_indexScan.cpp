/*============================================================================*/
/* XF_Morph_scan_To_indexScan.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Morph_scan_To_indexScan.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Morph_scan_To_indexScan, CN_XF_MORPH_SCAN_TO_INDEXSCAN)
__aNewFunction(XF_Morph_scan_To_indexScan, CN_XF_MORPH_SCAN_TO_INDEXSCAN)
__vDeleteFunction(XF_Morph_scan_To_indexScan, CN_XF_MORPH_SCAN_TO_INDEXSCAN)
__aFooterFunction(XF_Morph_scan_To_indexScan)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Morph_scan_To_indexScan::XF_Morph_scan_To_indexScan(CardIter* pCI)
: XF_TRANSFORM(CN_XF_MORPH_SCAN_TO_INDEXSCAN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_SCAN_TO_INDEXSCAN, _FOOTER_);

  _checkInitialised_(CN_XF_MORPH_SCAN_TO_INDEXSCAN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_SCAN_TO_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Morph_scan_To_indexScan::~XF_Morph_scan_To_indexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_SCAN_TO_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Morph_scan_To_indexScan::vXF_Morph_scan_To_indexScan(CardIter* pCI)
{
  _static_

  XF_Morph_scan_To_indexScan* pXF_Morph_scan_To_indexScan = new XF_Morph_scan_To_indexScan(pCI);

  pXF_Morph_scan_To_indexScan->vXF_Morph_scan_To_indexScan();

  delete pXF_Morph_scan_To_indexScan;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Morph_scan_To_indexScan::vXF_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_SCAN_TO_INDEXSCAN, _FOOTER_);

  m_pCI->vTransform(__func__, "morph scan to indexScan"_Stz);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {

    if (m_pCI->G.p_bounds->pSub_bound()->bIsLBound() ||
        m_pCI->G.p_bounds->pSub_bound()->bIsUBound()) {


      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vEndTransform();


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
