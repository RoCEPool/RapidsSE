/*============================================================================*/
/* XF_Utility_Verify_dic.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Utility_Verify_dic.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aNewFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__vDeleteFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aFooterFunction(XF_Utility_Verify_dic)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::XF_Utility_Verify_dic(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_VALIDATE_BOARD, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_VALIDATE_BOARD);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::~XF_Utility_Verify_dic(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify(CardIter* pCI)
{
  _static_

  XF_Utility_Verify_dic* pXF_Validate = new XF_Utility_Verify_dic(pCI);

  try {
    pXF_Validate->vVerify_dic_loop();
  }
  catch (Error xError) {
    delete pXF_Validate;
    throw xError;
  }
  delete pXF_Validate;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify_dic_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  m_pCI->vTransform(__func__, XF_UTILITY_VERIFY_DIC, "verify dic"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMain_NONE()) {

    vVerify_dic_OP();

    if (m_pCI->G.p_MainCard->pSub_emit())
      m_pCI->Flux.m_pArriving = m_pCI->G.p_MainCard->pSub_emit();
  }

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify_dic_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  RASSERT(m_pCI->G.p_MainCard->bIsOp());
  switch (m_pCI->G.e_MainCard) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_c_COMBINED:
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
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_o_FILTER) == 1);
  }

  //

  if ((m_pCI->G.p_injectable = m_pCI->G.p_MainCard->pSub_injectable())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_s_INJECTABLE) == 1);
  }

  if ((m_pCI->G.p_key = m_pCI->G.p_MainCard->pSub_key())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_s_KEY) == 1);
  }

  if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_s_CONSTRAINT) == 1);
  }

  if ((m_pCI->G.p_hshColList = m_pCI->G.p_MainCard->pSub_hshColList())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_s_HSHCOLLIST) == 1);
  }

  //

  if ((m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_t_INDEXLIST) == 1);
  }

  if ((m_pCI->G.p_emit = m_pCI->G.p_MainCard->pSub_emit())) {
    RASSERT(m_pCI->G.p_MainCard->nSubCards(CK_t_EMIT) == 1);
  }

  vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_injectable = m_pCI->G.p_MainCard->pSub_injectable())) {
    for (Kid xInject(m_pCI->G.p_injectable); xInject.bEach(CK_EXPR); /**/) {
      m_pCI->G.p_expr = xInject.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      RASSERT(m_pCI->G.p_expr->pSymbol()->bHasFacets(SF_EXP_COL,
                                                     SF_NMD_COL,
                                                     SF_TBL_COL,
                                                     SF_NONE));
      RASSERT(m_pCI->G.p_expr->bIsScalarCol());
      m_pCI->G.p_rpnCalc = m_pCI->G.p_expr->pSub_rpnCalc();
      for (Kid xRpnCalc(m_pCI->G.p_rpnCalc); xRpnCalc.bEach(CK_TERM); /**/) {
        m_pCI->G.p_term = xRpnCalc.p_term();
        RASSERT(m_pCI->G.p_term->pSymbol()->bSingleFaceted(SF_TBL_COL));
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_key = m_pCI->G.p_MainCard->pSub_key())) {
    for (Kid xKey(m_pCI->G.p_key); xKey.bEach(CK_EXPR); /**/) {
      m_pCI->G.p_expr->vDump(__func__);
      vDebugPoint();
    }
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
