/*============================================================================*/
/* XF_OT_Replace_scan_By_indexScan.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_OT_Replace_scan_By_indexScan.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_OT_Replace_scan_By_indexScan, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN)
__aNewFunction(XF_OT_Replace_scan_By_indexScan, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN)
__vDeleteFunction(XF_OT_Replace_scan_By_indexScan, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN)
__aFooterFunction(XF_OT_Replace_scan_By_indexScan)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_OT_Replace_scan_By_indexScan::XF_OT_Replace_scan_By_indexScan(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN, _FOOTER_);

  _checkInitialised_(CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_OT_Replace_scan_By_indexScan::~XF_OT_Replace_scan_By_indexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_OT_Replace_scan_By_indexScan::vXF_Morph_scan_To_indexScan(CardIter* pCI)
{
  _static_

  XF_OT_Replace_scan_By_indexScan* pXF_Morph_scan_To_indexScan = new XF_OT_Replace_scan_By_indexScan(pCI);

  pXF_Morph_scan_To_indexScan->vXF_Morph_scan_To_indexScan();

  delete pXF_Morph_scan_To_indexScan;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_OT_Replace_scan_By_indexScan::vXF_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN, _FOOTER_);

  m_pCI->vTransform(__func__, XF_MORPH_SCAN_TO_INDEXSCAN, "morph scan to indexScan"_Stz);

  m_pCI->vClearRestart();
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
