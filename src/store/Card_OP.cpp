/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OP::Card_OP(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OP::~Card_OP(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add missing level                                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vAddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Level* p_level = pSub_level();
  if (p_level)
    return;

  p_level = new Card_Level(AL_PARTITION);
  vPush(p_level);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_rpnCalc_To_boolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_boolCalc = pCI->G.p_rpnCalc->pToBoolCalc();

  vReplaceSubCard(pCI->G.p_rpnCalc,pCI->G.p_boolCalc);

  pCI->G_vDeleteMember(pCI->G.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (   CK_AGGREG  == pCI->G.e_MainCard
         || CK_GBYAGG  == pCI->G.e_MainCard
         || CK_PROJECT == pCI->G.e_MainCard
         || CK_WHERE   == pCI->G.e_MainCard))
    return;

  if (CK_AGGREG  == pCI->G.e_MainCard ||
      CK_GBYAGG  == pCI->G.e_MainCard ||
      CK_PROJECT == pCI->G.e_MainCard) {
    pCI->G.p_level = pCI->G.p_MainCard->pSub_level();
    if (pCI->G.p_level && AL_PARTITION != pCI->G.p_level->Y_iGet(P_Level))
      return;
  }

  vConvert_stmColOdn_terms_To_tblColOdn(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  auto bLambdaFn = [pCI](Card* pCard)
//  {
//    Card_Term* p_term = _pCard_Term(pCard);
//    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
//      return C_CONTINUE_RECURSION;
//
//    Stz* szStmColOdn = p_term->szValue();
//    if (! pCI->G_bResolveSubCard(CK_EXPR,szStmColOdn,CK_TBLCOLODN,pCI->G.i_MainCard))
//      return C_CONTINUE_RECURSION;
//
//    //RASSERT(pCI->m_pCard_parent->bIsCard(CK_STMCOL));
//
//    pCI->G.p_tblColOdn = pCI->m_pCard_resolved->pSub_tblColOdn();
//    p_term->vSetValueMark1(pCI->G.p_tblColOdn->szValue(), MARK_CHANGED);
//
//    return C_CONTINUE_RECURSION;
//  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_tblColOdn_To_stmColOdn(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    vTrace("CONVERT %s\n"_Stz,p_term->szValue());

    if (! Card::bIsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    //Stz* szStmColOdn = Card_StmColOdn::szResolve_tblColOdn_Into_stmColOdn(pCI, p_term,pCI->G.i_MainCard+1);

    Stz* szStmColOdn;
    s32 sColOfThisTable;
    Stz* szTblColOdn = p_term->szValue();
    if (! pCI->G_bResolveSubCard(CK_TBLCOLODN,szTblColOdn,pCI->G.i_MainCard)) {

      if (! (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) || pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)))
        return C_CONTINUE_RECURSION;

      pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
      i8 nExpr_prior = pCI->G.p_projection->nChildCards();
      i8 i_Col = p_term->iGetOdnNo();
      sColOfThisTable.vSprint("$%lld"_Stz,nExpr_prior+i_Col);

      szStmColOdn = sColOfThisTable.szStz();
    }
    else {
      //RASSERT(pCI->m_pCard_parent->bIsCard(CK_STMCOL));

      szStmColOdn = pCI->m_pCard_parent->szValue();
    }

    p_term->vSetValueMark1(szStmColOdn, MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bApplyRecursive(CK_TERM, bLambdaFn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
