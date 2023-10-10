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

  m_pCI->vTransform(__func__, XF_EPILOG_ADD_INFLATE, "add inflate"_Stz);

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain_NONE()) {

    if (CK_SCAN == m_pCI->G.e_MainCard) {
      m_pCI->G.p_inflate = new Card_Inflate;
      m_pCI->G_vInsertAfter(m_pCI->G.p_inflate,m_pCI->G.p_scan);

      if (m_pCI->G.p_scan->pSymbol()->Y_bHas(P_FusedProject))
        m_pCI->G.p_inflate->pSymbol()->Y_vSet(P_FusedProject);

      m_pCI->vBurst(m_pCI->G.p_MainCard);

      if (m_pCI->G.p_table)
        m_pCI->G.p_inflate->vAppendChild(m_pCI->G.p_table->pDupCardRecursive());
      if (m_pCI->G.p_stream)
        m_pCI->G.p_inflate->vAppendChild(m_pCI->G.p_stream->pDupCardRecursive());
      if (m_pCI->G.p_injected)
        m_pCI->G.p_inflate->vAppendChild(m_pCI->G.p_injected->pDupCardRecursive());
      if (m_pCI->G.p_synthesis)
        m_pCI->G.p_inflate->vAppendChild(m_pCI->G.p_synthesis->pDupCardRecursive());

    }
  }
  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
