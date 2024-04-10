/*============================================================================*/
/* XF_Optimise_gbyAgg_To_ordAgg.cpp -- Promote gbyAgg to ordAgg                */
/*============================================================================*/

#include "XF_Optimise_gbyAgg_To_ordAgg.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG)
__aNewFunction(XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG)
__vDeleteFunction(XF_Optimise_gbyAgg_To_ordAgg, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG)
__aFooterFunction(XF_Optimise_gbyAgg_To_ordAgg)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_gbyAgg_To_ordAgg::XF_Optimise_gbyAgg_To_ordAgg(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG, OT_GBYAGG_TO_ORDAGG, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_gbyAgg_To_ordAgg::~XF_Optimise_gbyAgg_To_ordAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_gbyAgg_To_ordAgg::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_gbyAgg_To_ordAgg* pXF_Promote_gbyAgg_To_ordAgg = new XF_Optimise_gbyAgg_To_ordAgg(pCI);

  bool b1 = pXF_Promote_gbyAgg_To_ordAgg->bOptimise();

  pXF_Promote_gbyAgg_To_ordAgg->vOptimised(b1);

  delete pXF_Promote_gbyAgg_To_ordAgg;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_gbyAgg_To_ordAgg::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_GBYAGG_TO_ORDAGG);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pEmitted);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pStreamKey);

  while (m_pCI->bEachOp()) {

    switch (m_pCI->E.e_Op) {

      case CK_o_GBYAGG:
        if (m_pCI->E.p_gbyAgg->bMorph_gbyAgg_To_ordAgg(m_pCI))
          bOptRun = true;

        break;

      default:
        if (m_pCI->E.p_Op->pSub_emit()) {
          m_pCI->StreamOrder.m_pEmitted = m_pCI->E.p_Op->pSub_emit();
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
