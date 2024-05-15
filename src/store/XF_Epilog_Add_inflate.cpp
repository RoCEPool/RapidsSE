/*============================================================================*/
/* XF_Epilog_Add_inflate.cpp -- Add Hash (opt) and Cast                     */
/*============================================================================*/

#include "XF_Epilog_Add_inflate.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Add_inflate, CN_XF_EPILOG_ADD_INFLATE)
__aNewFunction(XF_Epilog_Add_inflate, CN_XF_EPILOG_ADD_INFLATE)
__vDeleteFunction(XF_Epilog_Add_inflate, CN_XF_EPILOG_ADD_INFLATE)
__aFooterFunction(XF_Epilog_Add_inflate)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_inflate::XF_Epilog_Add_inflate(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ADD_INFLATE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_INFLATE, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ADD_INFLATE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_INFLATE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_inflate::~XF_Epilog_Add_inflate(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_INFLATE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_inflate::vBifurcate(CardIter* pCI)
{
  _static_

  XF_Epilog_Add_inflate* pXF_stage_cast = new XF_Epilog_Add_inflate(pCI);

  pXF_stage_cast->vBifurcate_inflate();

  delete pXF_stage_cast;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_inflate::vBifurcate_inflate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_INFLATE, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_ADD_INFLATE);

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    if (CK_o_SCAN == m_pCI->E.e_Op) {
      m_pCI->E.p_inflate = new Card_Inflate;
      m_pCI->vInsertAfter(m_pCI->E.p_inflate, m_pCI->E.p_scan);

      if (m_pCI->E.p_scan->pSymbol()->Y_bHas(P_FusedProject))
        m_pCI->E.p_inflate->pSymbol()->Y_vSet(P_FusedProject);

      m_pCI->E.p_table = m_pCI->E.p_scan->pSub_table();

      if (m_pCI->E.p_table->pSymbol()->Y_bHas(P_AltFetch))
        m_pCI->E.p_inflate->pSymbol()->Y_vSet(P_AltFetch,m_pCI->E.p_table->pSymbol()->Y_szGet(P_AltFetch));

      m_pCI->vBurst(m_pCI->E.p_Op);

      if (m_pCI->E.p_table)
        m_pCI->E.p_inflate->vAppendChild(m_pCI->E.p_table->pDupCardRecursive());
      if (m_pCI->E.p_emit)
        m_pCI->E.p_inflate->vAppendChild(m_pCI->E.p_emit->pDupCardRecursive());
      if (m_pCI->E.p_injectable)
        m_pCI->E.p_inflate->vAppendChild(m_pCI->E.p_injectable->pDupCardRecursive());

      m_pCI->vSnapIf_arriving();

      continue;
    }

    m_pCI->vSnapIf_arriving();
  }

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    if (CK_o_STAGE != m_pCI->E.e_Op) {
      m_pCI->vSnapIf_arriving();
      continue;
    }

    eCK eNext = m_pCI->ePeekNext();
    if (CK_o_SCAN == eNext ||
        CK_o_INDEXSCAN == eNext) {
      m_pCI->vSnapIf_arriving();
      continue;
    }

    m_pCI->E.p_pdsRow = new Card_PdsRow;
    m_pCI->vInsertAfter(m_pCI->E.p_pdsRow, m_pCI->E.p_Op);

    m_pCI->vBurst(m_pCI->E.p_Op);

    if (m_pCI->Arriving.m_pEmitted_prior)
      m_pCI->E.p_pdsRow->vAppendChild(m_pCI->Arriving.m_pEmitted_prior->pDupCardRecursive());

    m_pCI->vSnapIf_arriving();
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
