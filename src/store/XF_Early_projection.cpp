/*============================================================================*/
/* XF_Early_projection.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Early_projection.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,XF_Early_projection,CN_XF_EARLY_PROJECTION)
__aNewFunction(XF_Early_projection,CN_XF_EARLY_PROJECTION)
__vDeleteFunction(XF_Early_projection,CN_XF_EARLY_PROJECTION)
__aFooterFunction(XF_Early_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Early_projection::XF_Early_projection(CardIter* pCI)
: XF_TRANSFORM(CN_XF_EARLY_PROJECTION,pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_EARLY_PROJECTION,_FOOTER_);

  _checkInitialised_(CN_XF_EARLY_PROJECTION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XF_EARLY_PROJECTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Early_projection::~XF_Early_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_EARLY_PROJECTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Early_projection::vXF_Early_projection(CardIter* pCI)
{
  _static_

  XF_Early_projection* pXF_Early_projection = new XF_Early_projection(pCI);

  pXF_Early_projection->vXF_Early_projection();

  delete pXF_Early_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Early_projection::vXF_Early_projection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_EARLY_PROJECTION,_FOOTER_);

//  if (! AM::JOB::bHasTag(C_EP_ENABLE))
//    return;
  if (AM::JOB::bHasTag(C_EP_DISABLE))
    return;

  m_pCI->vTransform(__func__, XF_EARLY_PROJECTION, "Early projections"_Stz);

  m_pCI->vClearRestart();

  m_pCI->vRenumberCards();

  m_pCI->vPrintBoard();

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  for (;;) {

    m_pCI->Early.m_pOP_root = nullptr;
    m_pCI->Early.m_pOP_leaf = nullptr;

    bool bEarlyRoot = false;
    while (m_pCI->G_bEachMain_reverse()) {
      if (Card_OP::bIsOp(m_pCI->G.p_MainCard) && m_pCI->G.p_OP->bIsEarlyRoot()) {
        bEarlyRoot = true;
        break;
      }
    }
    if (! bEarlyRoot)
      break;

    m_pCI->G.p_OP->vEarlyProject_root(m_pCI);

    if (m_pCI->Early.m_pNeeded_later) {
      DELETE_CARD(m_pCI->Early.m_pNeeded_later);
    }

    m_pCI->G_vMainResynch(m_pCI->Early.m_pOP_leaf);
  }

  m_pCI->vEndTransform();
  vDebugPoint();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
