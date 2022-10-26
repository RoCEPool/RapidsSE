/*============================================================================*/
/* Xlator.cpp --                                                              */
/*============================================================================*/

#include "Xlator.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xlator::Xlator(eCN eClassNo, Bench* pBench)
: TransformerTool(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xlator::~Xlator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Xlator::X_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_AndPred* p_andPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_TstPred* p_tstPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  //N_vNewMember(CK_BOOLCALC);
  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term = _pCard_Term(p_rpnCalc->pPop());

    eTE eTopTerm = T_eTerm_TermEval(p_term);
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == p_boolCalc->nChildCards());
        p_andPred = new Card_AndPred;
        p_andPred->vTransferChildren(p_boolCalc);
        RASSERT(0 == p_boolCalc->nChildCards());
        p_boolCalc->vAppend(p_andPred);
        RASSERT(1 == p_boolCalc->nChildCards());
        p_andPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_OR:
        RASSERT(2 == p_boolCalc->nChildCards());
        p_orPred = new Card_OrPred;
        p_orPred->vTransferChildren(p_boolCalc);
        RASSERT(0 == p_boolCalc->nChildCards());
        p_boolCalc->vAppend(p_orPred);
        RASSERT(1 == p_boolCalc->nChildCards());
        p_orPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vValue(p_term->sz());
        p_cmpPred->vTransferChildren(pCard_SubExpr);
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_EXISTS:
        p_tstPred = new Card_TstPred;
        p_tstPred->vValue(p_term->sz());
        p_tstPred->vTransferChildren(pCard_SubExpr);
        p_boolCalc->vAppend(p_tstPred);
        p_tstPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(p_term);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  //M.p_boolCalc->vDump();

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == p_boolCalc->nChildCards());
    p_cmpPred->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == p_boolCalc->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;

  return p_boolCalc;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
