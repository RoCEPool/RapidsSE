/*============================================================================*/
/* XF_Optimise_Derive_streamKey.cpp -- Derive projection key                      */
/*============================================================================*/

#include "XF_Optimise_Derive_streamKey.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Derive_streamKey, CN_XF_OPTIMISE_DERIVE_STREAMKEY)
__aNewFunction(XF_Optimise_Derive_streamKey, CN_XF_OPTIMISE_DERIVE_STREAMKEY)
__vDeleteFunction(XF_Optimise_Derive_streamKey, CN_XF_OPTIMISE_DERIVE_STREAMKEY)
__aFooterFunction(XF_Optimise_Derive_streamKey)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Derive_streamKey::XF_Optimise_Derive_streamKey(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_DERIVE_STREAMKEY, OT_DERIVE_STREAMKEY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_DERIVE_STREAMKEY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Derive_streamKey::~XF_Optimise_Derive_streamKey(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Derive_streamKey::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Derive_streamKey* pXF_Derive_projection_key = new XF_Optimise_Derive_streamKey(pCI);

  bool b1 = pXF_Derive_projection_key->bOptimise();

  pXF_Derive_projection_key->vOptimised(b1);

  delete pXF_Derive_projection_key;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Derive_streamKey::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_DERIVE_STREAMKEY);

  RASSERT(nullptr == m_pCI->StreamOrder.m_pEmitted);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey);

  if (bFindStart_streamKey()) {
    vPropagate_streamKey();
    bOptRun = true;
  }

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Derive_streamKey::bFindStart_streamKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);

  bool bFoundKeyStart = false;

  m_pCI->vClearRestart();

  for (;;) {
    if (m_pCI->pPeekNextIf(CK_o_INDEXSCAN) ||
        m_pCI->pPeekNextIf(CK_o_GBYAGG) ||
        m_pCI->pPeekNextIf(CK_o_SORT) ) {
      bFoundKeyStart = true;
      break;
    }

    if (!m_pCI->bEachOp())
      break;
  }

  return bFoundKeyStart;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Derive_streamKey::vPropagate_streamKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DERIVE_STREAMKEY, _FOOTER_);

  while (m_pCI->bEachOp()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_o_GBYAGG:     m_pCI->G.p_gbyAgg->vDerive_streamKey(m_pCI);     break;
      case CK_o_INDEXJOIN:  m_pCI->G.p_indexJoin->vDerive_streamKey(m_pCI);  break;
      case CK_o_INDEXSCAN:  m_pCI->G.p_indexScan->vDerive_streamKey(m_pCI);  break;
      case CK_o_PROJECT:    m_pCI->G.p_project->vDerive_streamKey(m_pCI);    break;
      case CK_o_SORT:       m_pCI->G.p_sort->vDerive_streamKey(m_pCI);       break;

      default:
        break;
    }

   // m_pCI->PRINTBOARD_dbg();

    if (! m_pCI->StreamOrder.m_pStreamKey)
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
