/*============================================================================*/
/* XF_Flatten_10_F.cpp -- Flatten                                             */
/*============================================================================*/

#include "XF_Flatten_10_F.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "XF_Prolog_Analyse_emit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_10_F, CN_XF_FLATTEN_10_F)
__aNewFunction(XF_Flatten_10_F, CN_XF_FLATTEN_10_F)
__vDeleteFunction(XF_Flatten_10_F, CN_XF_FLATTEN_10_F)
__aFooterFunction(XF_Flatten_10_F)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_10_F::XF_Flatten_10_F(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_10_F, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_10_F);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_10_F::~XF_Flatten_10_F(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_10_F::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_10_F* pXF_Flatten_10_F = new XF_Flatten_10_F(pCI);

  pXF_Flatten_10_F->vFlatten();

  delete pXF_Flatten_10_F;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_10_F::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // F
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_10_F);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of from

  XF_Fold* pFold = new XF_Fold(0, m_pCI->G.pDeck);
  CarryForward* pCarryForward = pFold->pFold_from("From"_Stz,m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  pCarryForward->Table.m_bReproject = true;
  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vMarksClearRecursive();

  // todo: discuss with Alka
  m_pCI->vClearRestart();
  if (m_pCI->bEachMain(CK_c_FOLD)) {
    if (m_pCI->bEachMain(CK_o_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->bEachMainWithSub(CK_o_PROJECT, CK_s_LEVEL)) {
        p_level_where->pSymbol()->Y_vSet(P_Level, m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform(__LINE__);


  delete pSmooth;
  delete pCarryForward;
  delete pFold;

  XF_Prolog_Analyse_emit::vAnalyse(m_pCI);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
