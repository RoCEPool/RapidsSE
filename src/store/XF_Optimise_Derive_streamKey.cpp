/*============================================================================*/
/* XF_Optimise_Derive_streamKey.cpp -- Derive projection key                      */
/*============================================================================*/

#include "XF_Optimise_Derive_streamKey.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Derive_streamKey, CN_XF_DERIVE_STREAM_KEY)
__aNewFunction(XF_Optimise_Derive_streamKey, CN_XF_DERIVE_STREAM_KEY)
__vDeleteFunction(XF_Optimise_Derive_streamKey, CN_XF_DERIVE_STREAM_KEY)
__aFooterFunction(XF_Optimise_Derive_streamKey)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Derive_streamKey::XF_Optimise_Derive_streamKey(CardIter* pCI)
: XF_OPTIMISE(CN_XF_DERIVE_STREAM_KEY, OT_DERIVE_STREAM_KEY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);

  _checkInitialised_(CN_XF_DERIVE_STREAM_KEY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Derive_streamKey::~XF_Optimise_Derive_streamKey(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);

  m_pCI->vTransform(__func__, XF_DERIVE_PROJECTION_KEY, "Derive projection keys"_Stz);

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey_running);

  if (bFindStart_streamKey()) {
    vPropagate_streamKey();
    m_pCI->pBoard()->pState()->vInsert(aOT[OT_DERIVE_STREAM_KEY]);
  }


    m_pCI->vEndTransform();

  vValidate();

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Derive_streamKey::bFindStart_streamKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);

  bool bFoundKeyStart = false;

  m_pCI->vClearRestart();

  for (;;) {
    if (m_pCI->pNextChildIf(CK_INDEXSCAN) ||
        m_pCI->pNextChildIf(CK_GBYAGG)    ||
        m_pCI->pNextChildIf(CK_SORT) ) {
      bFoundKeyStart = true;
      break;
    }

    if (! m_pCI->G_bEachMain())
      break;
  }

  return bFoundKeyStart;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Derive_streamKey::vPropagate_streamKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_STREAM_KEY, _FOOTER_);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_GBYAGG:     m_pCI->G.p_gbyAgg->vDerive_streamKey(m_pCI);     break;
      case CK_INDEXJOIN:  m_pCI->G.p_indexJoin->vDerive_streamKey(m_pCI);  break;
      case CK_INDEXSCAN:  m_pCI->G.p_indexScan->vDerive_streamKey(m_pCI);  break;
      case CK_PROJECT:    m_pCI->G.p_project->vDerive_streamKey(m_pCI);    break;
      case CK_SORT:       m_pCI->G.p_sort->vDerive_streamKey(m_pCI);       break;

      default:
        break;
    }

    m_pCI->vPrintBoard(__LINE__);

    if (! m_pCI->StreamOrder.m_pStreamKey_running)
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
