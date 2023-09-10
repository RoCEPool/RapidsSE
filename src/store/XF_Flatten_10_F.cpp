/*============================================================================*/
/* XF_Flatten_10_F.cpp -- Flatten                                             */
/*============================================================================*/

#include "XF_Flatten_10_F.hpp"

#include "Fold.hpp"
#include "Smooth.hpp"

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
void XF_Flatten_10_F::vXF_Flatten_10_F(CardIter* pCI)
{
  _static_

  XF_Flatten_10_F* pXF_Flatten_10_F = new XF_Flatten_10_F(pCI);

  pXF_Flatten_10_F->vXF_Flatten_10_F();

  delete pXF_Flatten_10_F;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_10_F::vXF_Flatten_10_F(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // F
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_10_F))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of from

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward = pFold->pFold_from("From"_Stz,m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  pCarryForward->Table.m_bReproject = true;
  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vProjectionUpdate(m_pCI);
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  // todo: discuss with Alka
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_LEVEL)) {
        p_level_where->pSymbol()->Y_vSet(P_Level, m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
