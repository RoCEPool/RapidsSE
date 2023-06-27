/*============================================================================*/
/* XF_Derive_projection_key.cpp -- Derive projection key                      */
/*============================================================================*/

#include "XF_Derive_projection_key.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Derive_projection_key, CN_XF_DERIVE_PROJECTION_KEY)
__aNewFunction(XF_Derive_projection_key, CN_XF_DERIVE_PROJECTION_KEY)
__vDeleteFunction(XF_Derive_projection_key, CN_XF_DERIVE_PROJECTION_KEY)
__aFooterFunction(XF_Derive_projection_key)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Derive_projection_key::XF_Derive_projection_key(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DERIVE_PROJECTION_KEY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_PROJECTION_KEY, _FOOTER_);

  _checkInitialised_(CN_XF_DERIVE_PROJECTION_KEY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_PROJECTION_KEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Derive_projection_key::~XF_Derive_projection_key(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_PROJECTION_KEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Derive_projection_key::vXF_Derive_projection_key(CardIter* pCI)
{
  _static_

  XF_Derive_projection_key* pXF_Derive_projection_key = new XF_Derive_projection_key(pCI);

  pXF_Derive_projection_key->vXF_Derive_projection_key();

  delete pXF_Derive_projection_key;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Derive_projection_key::vXF_Derive_projection_key(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_PROJECTION_KEY, _FOOTER_);

  m_pCI->vTransform(__func__, "Derive projection keys"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  bool bFoundKeyStart = false;
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

  if (bFoundKeyStart) {

    while (m_pCI->G_bEachMain()) {

      switch (m_pCI->G.e_MainCard) {

        case CK_GBYAGG:
          m_pCI->G.p_gbyAgg->vDerive_Projection_Key(m_pCI);
          break;
        case CK_INDEXJOIN:
          m_pCI->G.p_indexJoin->vDerive_Projection_Key(m_pCI);
          break;
        case CK_INDEXSCAN:
          m_pCI->G.p_indexScan->vDerive_Projection_Key(m_pCI);
          break;
        case CK_PROJECT:
          m_pCI->G.p_project->vDerive_Projection_Key(m_pCI);
          break;
        case CK_SORT:
          m_pCI->G.p_sort->vDerive_Projection_Key(m_pCI);
          break;

        default:
          break;
      }

      m_pCI->vPrintBoard(__LINE__);

      if (! m_pCI->StreamOrder.m_pKey_running)
        break;
    }
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
