/*============================================================================*/
/* XF_Optimise_gbyAgg_To_ordAgg.cpp -- Promote gbyAgg to ordAgg                */
/*============================================================================*/

#include "XF_Optimise_gbyAgg_To_ordAgg.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG)
__aNewFunction(XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG)
__vDeleteFunction(XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG)
__aFooterFunction(XF_Optimise_gbyAgg_To_ordAgg)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_gbyAgg_To_ordAgg::XF_Optimise_gbyAgg_To_ordAgg(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG, _FOOTER_);

  _checkInitialised_(CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_gbyAgg_To_ordAgg::~XF_Optimise_gbyAgg_To_ordAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_gbyAgg_To_ordAgg::vXF_Promote_gbyAgg_To_ordAgg(CardIter* pCI)
{
  _static_

  XF_Optimise_gbyAgg_To_ordAgg* pXF_Promote_gbyAgg_To_ordAgg = new XF_Optimise_gbyAgg_To_ordAgg(pCI);

  pXF_Promote_gbyAgg_To_ordAgg->vXF_Promote_gbyAgg_To_ordAgg();

  delete pXF_Promote_gbyAgg_To_ordAgg;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_gbyAgg_To_ordAgg::vXF_Promote_gbyAgg_To_ordAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROMOTE_GBYAGG_TO_ORDAGG, "Promote gbyAgg to ordAgg"_Stz);

  m_pCI->vClearRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_GBYAGG:
        if (m_pCI->G.p_gbyAgg->bReplace_gbyAgg_By_ordAgg(m_pCI))
          m_pCI->pBoard()->pState()->vInsert(aOT[OT_REPLACE_GBYAGG_TO_ORDAGG]);

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
