/*============================================================================*/
/* XF_Optimise_Early_projectionv2.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Optimise_Early_projectionv2.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aNewFunction(XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__vDeleteFunction(XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aFooterFunction(XF_Optimise_Early_projectionv2)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projectionv2::XF_Optimise_Early_projectionv2(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_EARLY_PROJECTION, OT_EARLY_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_EARLY_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projectionv2::~XF_Optimise_Early_projectionv2(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Early_projectionv2* pXF_Early_projection = new XF_Optimise_Early_projectionv2(pCI);

  bool b1;

  pXF_Early_projection->vEarly_needed_loop();
  b1 = pXF_Early_projection->bEarly_projection_loop();

  pXF_Early_projection->vOptimised(b1);

  delete pXF_Early_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* loop thru operators / early projection                                     */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vEarly_needed_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vBeginEach_tail();

  while (m_pCI->bEachOp_reverse())
    vEarly_needed_OP();

  m_pCI->vEndTransform(__LINE__);

  m_pCI->Early.m_pNeeded_later = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vEarly_needed_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

//  switch (m_pCI->E.e_Op) {
//
//    case CK_o_AGGREG:
//    case CK_o_CALL:
//    case CK_o_CAST:
//    case CK_o_CORRELATION:
//    case CK_o_CROSSJOIN:
//    case CK_o_FILTER:
//    case CK_o_FROM:
//    case CK_o_GBYAGG:
//    case CK_o_HASH:
//    case CK_o_HAVING:
//    case CK_o_INDEXSCAN:
//    case CK_o_INDEXJOIN:
//    case CK_o_INFLATE:
//    case CK_o_KEYEXISTS:
//    case CK_o_KEYNOTEXISTS:
//    case CK_o_LIMIT:
//    case CK_o_ORDAGG:
//    case CK_o_PROJECT:
//    case CK_o_SCAN:
//    case CK_o_SORT:
//    case CK_o_TOP:
//    case CK_o_WHERE:
//      break;
//
//    default:
//      break;
//  }

  if (! m_pCI->E.p_Op->bIsEarlyRoot())
    return;

  m_pCI->vBurst(m_pCI->E.p_Op);

  switch (m_pCI->E.e_Op) {
    case CK_o_AGGREG:
    case CK_o_GBYAGG:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_INDEXJOIN:
     break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* loop thru operators / early projection                                     */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projectionv2::bEarly_projection_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vBeginEach();

  while (m_pCI->bEachOp()) {
    bOptRun = bEarly_projection_OP();
  }

  m_pCI->vEndTransform(__LINE__);

  m_pCI->Early.m_pNeeded_later = nullptr;

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projectionv2::bEarly_projection_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  switch (m_pCI->E.e_Op) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      break;

    default:
      break;
  }

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
