/*============================================================================*/
/* XF_Epilog_Type_Cast.cpp -- Hash Columns                                 */
/*============================================================================*/

#include "XF_Epilog_Type_Cast.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Type_Cast, CN_XF_EPILOG_HASH_COLUMNS)
__aNewFunction(XF_Epilog_Type_Cast, CN_XF_EPILOG_HASH_COLUMNS)
__vDeleteFunction(XF_Epilog_Type_Cast, CN_XF_EPILOG_HASH_COLUMNS)
__aFooterFunction(XF_Epilog_Type_Cast)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Type_Cast::XF_Epilog_Type_Cast(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_HASH_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_HASH_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Type_Cast::~XF_Epilog_Type_Cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_HASH_COLUMNS, "column hashing"_Stz);

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain_NONE()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_o_FROM:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_OP->pSub_constraint()))
          m_pCI->G.p_constraint->vEnsureTypeId_expCol(m_pCI);
        break;

      case CK_o_INDEXJOIN:
//        if ((m_pCI->G.p_indexList = m_pCI->G.p_OP->pSub_indexList())) {
//          if (m_pCI->Flux.m_pArriving) {
//            m_pCI->G.p_indexList->vTypes_For_expCol(m_pCI);
//          }
//        }

        break;

      case CK_o_INDEXSCAN:
//        if ((m_pCI->G.p_indexList = m_pCI->G.p_OP->pSub_indexList())) {
//          if (m_pCI->Flux.m_pArriving) {
//            m_pCI->G.p_indexList->vTypes_For_expCol(m_pCI);
//          }
//        }
        break;

      case CK_o_SCAN:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_OP->pSub_constraint()))
          m_pCI->G.p_constraint->vEnsureTypeId_expCol(m_pCI);
        break;

      case CK_c_COMBINED:
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
      case CK_o_SORT:
      case CK_o_TOP:
      case CK_o_WHERE:
        break;

      default:
        m_pCI->G.p_OP->vDump(__func__);
        NEVER_REACH
        break;
    } // switch
  } // while

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
