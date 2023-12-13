/*============================================================================*/
/* XF_Prolog_Analyse_emit.cpp -- Analyse Emit                                 */
/*============================================================================*/

#include "XF_Prolog_Analyse_emit.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__aNewFunction(XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__vDeleteFunction(XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__aFooterFunction(XF_Prolog_Analyse_emit)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emit::XF_Prolog_Analyse_emit(CardIter* m_pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_EMIT, m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_EMIT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emit::~XF_Prolog_Analyse_emit(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emit::vAnalyse(CardIter* m_pCI)
{
  _static_

  XF_Prolog_Analyse_emit* pXF_Analyse_emitted = new XF_Prolog_Analyse_emit(m_pCI);

  pXF_Analyse_emitted->vXF_Analyse();

  delete pXF_Analyse_emitted;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* first analysis of emit                                                     */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emit::vXF_Analyse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_EMIT, "emit analysis"_Stz);

  Card_OP::vRecreate_emit(m_pCI);
  /*
  m_pCI->vBeginEach();
  m_pCI->Flux.m_pArriving = m_pCI->G.p_emit = nullptr;

  while (m_pCI->G_bEachMain_NONE()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_c_COMBINED:
      case CK_c_SEND:
      case CK_c_STAGE:
      case CK_o_CALL:
      case CK_o_CORRELATION:
        RASSERT(! m_pCI->G.p_OP->pSub_emit());
        break;

      case CK_o_FROM:
      case CK_o_SCAN:
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

      case CK_o_INDEXJOIN:
      case CK_o_INDEXSCAN:
        RASSERT(! m_pCI->G.p_OP->pSub_emit())
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_AGGREG:
      case CK_o_PROJECT:
        RASSERT(m_pCI->G.p_OP->pSub_emit());
        RASSERT(! m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_emit = m_pCI->G.p_OP->pSub_emit();
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_WHERE:
      case CK_o_HAVING:
        RASSERT(! m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

      case CK_o_HASH:
      case CK_o_LIMIT:
        RASSERT(m_pCI->Flux.m_pArriving);
        RASSERT(! m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_GBYAGG:
      case CK_o_ORDAGG:
        RASSERT(m_pCI->G.p_OP->pSub_emit());
        RASSERT(m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_emit = m_pCI->G.p_OP->pSub_emit();
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_KEYEXISTS:
      case CK_o_KEYNOTEXISTS:
        RASSERT(m_pCI->Flux.m_pArriving);
        RASSERT(! m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

      case CK_o_TOP:
        RASSERT(m_pCI->Flux.m_pArriving);
        if (! m_pCI->G.p_OP->pSub_key())
          m_pCI->G.p_OP->vDump(__func__);
        RASSERT(m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_SORT:
        RASSERT(m_pCI->Flux.m_pArriving);
        RASSERT(m_pCI->G.p_OP->pSub_key());
        m_pCI->G.p_key = m_pCI->G.p_OP->pSub_key();
        m_pCI->G.p_OP->vCreate_emit(m_pCI);
        m_pCI->G.p_emit->vConcatFacets_From_key(m_pCI->G.p_key);
        break;

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case CK_o_CAST:
      case CK_o_CROSSJOIN:
      case CK_o_FILTER:
      case CK_o_INFLATE:
        m_pCI->G.p_OP->vDump(__func__);
        NEVER_REACH
        break;

      default:
        m_pCI->G.p_OP->vDump(__func__);
        NEVER_REACH
        break;

    } // switch(m_pCI->G.e_MainCard)

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    m_pCI->Flux.m_pArriving = m_pCI->G.p_emit;

  } // while (m_pCI->G_bEachMain_NONE())
*/
  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
