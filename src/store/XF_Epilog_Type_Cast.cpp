/*============================================================================*/
/* XF_Epilog_Type_Cast.cpp -- Hash Columns                                 */
/*============================================================================*/

#include "XF_Epilog_Type_Cast.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Type_Cast, CN_XF_EPILOG_TYPE_CAST)
__aNewFunction(XF_Epilog_Type_Cast, CN_XF_EPILOG_TYPE_CAST)
__vDeleteFunction(XF_Epilog_Type_Cast, CN_XF_EPILOG_TYPE_CAST)
__aFooterFunction(XF_Epilog_Type_Cast)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Type_Cast::XF_Epilog_Type_Cast(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_TYPE_CAST, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_TYPE_CAST, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_TYPE_CAST);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_TYPE_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Type_Cast::~XF_Epilog_Type_Cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_TYPE_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Type_Cast::vCast(CardIter* pCI)
{
  _static_

  XF_Epilog_Type_Cast* pXF_Type_Cast = new XF_Epilog_Type_Cast(pCI);

  pXF_Type_Cast->vCast();

  delete pXF_Type_Cast;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Type_Cast::vCast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_TYPE_CAST, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_TYPE_CAST);

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    switch (m_pCI->E.e_Op) {

      case CK_o_FROM:
        break;

      case CK_o_INDEXJOIN:
        break;

      case CK_o_INDEXSCAN:
        break;

      case CK_o_SCAN:
        break;

      case CK_o_SORT:
       break;

      case CK_c_STAGE:
      case CK_c_SEND:

      case CK_o_AGGREG:
      case CK_o_CALL:
      case CK_o_CAST:
      case CK_o_CORRELATION:
      case CK_o_CROSSJOIN:
      case CK_o_FILTER:
      case CK_o_GBYAGG:
      case CK_o_HASH:
      case CK_o_HAVING:
      case CK_o_INFLATE:
      case CK_o_KEYEXISTS:
      case CK_o_KEYNOTEXISTS:
      case CK_o_LIMIT:
      case CK_o_ORDAGG:
      case CK_o_PROJECT:
      case CK_o_TOP:
      case CK_o_WHERE:
        break;

      default:
        m_pCI->E.p_Op->vDump(__func__);
        NEVER_REACH
        break;
    } // switch

    if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
      m_pCI->E.p_key->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
      m_pCI->E.p_key->vRenumberAll_expr(SF_KEY_COL);
      m_pCI->E.p_key = nullptr;
    }

    if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {

      if (CK_o_FROM == m_pCI->E.e_Op ||
          CK_o_SCAN == m_pCI->E.e_Op ||
          CK_o_INDEXJOIN == m_pCI->E.e_Op ||
          CK_o_INDEXSCAN == m_pCI->E.e_Op) {

        m_pCI->vSnapIf_arriving();

        m_pCI->E.p_emit->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
        m_pCI->E.p_emit->vRenumberAll_expr();
      }
      else if (m_pCI->Arriving.m_pEmit) {
        if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
          m_pCI->E.p_key->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
        }

        if (m_pCI->m_pBench->eXform() == XF_EPILOG_TYPE_CAST &&
            CK_o_GBYAGG == m_pCI->E.e_Op) {
          //m_pCI->PRINTBOARD_dbg();
          vDebugPoint();
        }

        m_pCI->E.p_emit->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
        m_pCI->E.p_emit->vRenumberAll_expr();
        m_pCI->vSnapIf_arriving();
         m_pCI->E.p_key = nullptr;
      }
      else {
        m_pCI->vSnapIf_arriving();
      }

      if ((m_pCI->E.p_streamKey = m_pCI->E.p_Op->pSub_streamKey()))
        m_pCI->E.p_streamKey->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);

      if ((m_pCI->E.p_predicate = m_pCI->E.p_Op->pSub_predicate())) {
        //m_pCI->G.p_predicate->vDump(__func__);
        m_pCI->E.p_predicate->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
      }

      if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList()))
        m_pCI->E.p_indexList->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);

      if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint()))
        m_pCI->E.p_constraint->vApplyCast(m_pCI,/*??*/m_pCI->Arriving.m_pEmit);
    }
    else {

    }

  } // while

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
