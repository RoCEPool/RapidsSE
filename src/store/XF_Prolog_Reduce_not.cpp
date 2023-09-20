/*============================================================================*/
/* XF_Prolog_Reduce_not.cpp -- Reduce Not expression ops                     */
/*============================================================================*/

#include "XF_Prolog_Reduce_not.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aNewFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__vDeleteFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aFooterFunction(XF_Prolog_Reduce_not)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::XF_Prolog_Reduce_not(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_REDUCE_NOT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_REDUCE_NOT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::~XF_Prolog_Reduce_not(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_Reduce_not* pXF_not_Ops = new XF_Prolog_Reduce_not(pCI);

  pXF_not_Ops->vXF_not_Ops();

  delete pXF_not_Ops;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* not ops                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vXF_not_Ops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_NOT_OPS, "not ops"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC)) {

    Kid xKid(m_pCI->G.p_rpnCalc);
    while (xKid.bEach(CK_TERM)) {

      Card_Term* p_term = xKid.p_term();

      if (! xKid.bMore())
        break;

      if (p_term->pSymbol()->bSame("exists"_Stz)) {

        RASSERT(xKid.bEach(CK_TERM));
        Card_Term* p_term_next = xKid.p_term();
        if (p_term_next->pSymbol()->bSame("not"_Stz)) {
          xKid.vRewind();
          m_pCI->G.p_rpnCalc->vRemoveChild(p_term_next);
          p_term->SYM(SF_SQY_OP,"notExists"_Stz);
          continue;
        }
        continue;
      }

      if (xKid.p_term()->pSymbol()->bSame("in"_Stz)) {

        RASSERT(xKid.bEach(CK_TERM));
        Card_Term* p_term_next = xKid.p_term();
        if (p_term_next->pSymbol()->bSame("not"_Stz)) {
          xKid.vRewind();
          m_pCI->G.p_rpnCalc->vRemoveChild(p_term_next);
          p_term->SYM(SF_SQY_OP,"notIn"_Stz);
          DELETE_CARD(p_term_next);
          continue;
        }
        continue;
      }

    }

  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
