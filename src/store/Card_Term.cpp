/*============================================================================*/
/* Card_Term.cpp -- Term                                                      */
/*============================================================================*/

#include "Card_Term.hpp"

#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Term,CK_TERM)
__aNewFunction(Card_Term,CN_CARD)
__vDeleteFunction(Card_Term,CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Stz* szTerm)
: Card(CK_TERM,szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Symbol* pSymbol)
: Card(CK_TERM,pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term::~Card_Term(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBumpKeyOdn(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iKeyOdn = pSymbol()->iExtractOrdinal();

  this->pSymbol()->vSetKeyOrdinal(iKeyOdn + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_stmOdn(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmColOdn = pSymbol()->iExtractOrdinal();

  this->pSymbol()->vSetStreamOrdinal(iStmColOdn + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/* convert term (if stream-column) to table-column                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vConvertIf_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  ASSERT (   pCI->G.p_MainCard->bIsCard(CK_AGGREG)
          || pCI->G.p_MainCard->bIsCard(CK_FILTER)
          || pCI->G.p_MainCard->bIsCard(CK_GBYAGG)
          || pCI->G.p_MainCard->bIsCard(CK_PROJECT)
          || pCI->G.p_MainCard->bIsCard(CK_WHERE));

  if (CK_AGGREG  == pCI->G.e_MainCard ||
      CK_FILTER  == pCI->G.e_MainCard ||
      CK_GBYAGG  == pCI->G.e_MainCard ||
      CK_PROJECT == pCI->G.e_MainCard) {
    pCI->G.p_level = pCI->G.p_MainCard->pSub_level();
    ASSERT(pCI->G.p_level && AL_PARTITION == pCI->G.p_level->pSymbol()->Y_iGet(P_Level));
  }

  if (!pSymbol()->bIsStreamOrdinal())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Card_Expr* p_expr_prior = pCI->pResolvePrior_expr(this))
    if (Stz* szPrjCol = p_expr_prior->pGetTerm(0)->szSymbol())
      vSymbolMark1(szPrjCol, MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Card_Term::eEvalBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eEvalRelOp();

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eEvalRelnOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eEvalRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = pSymbol()->eEvalRelnOp();

  return eRelOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card_Term::eEvalTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szValue = this->szSymbol();

       if (bIsSymbol("and"_Stz))        return TE_AND;
  else if (bIsSymbol("or"_Stz))         return TE_OR;
  else if (bIsSymbol("not"_Stz))        return TE_NOT;
  else if (bIsSymbol("}"_Stz))          return TE_EXECUTE;
  else if (bIsSymbol("exists"_Stz))     return TE_EXISTS;
  else if (bIsSymbol("in"_Stz))         return TE_IN;
  else if (bIsSymbol("notExists"_Stz))  return TE_NOTEXISTS;
  else if (bIsSymbol("notIn"_Stz))      return TE_NOTIN;

  else if (pSymbol()->bIsRelnOp())     return TE_RELNOP;
  else if (pSymbol()->bIsBoolFn())     return TE_BOOLFN;

  else return TE_OTHER;
}


/*----------------------------------------------------------------------------*/
/* get ordinal number                                                         */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iGetOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(this->szSymbol()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $c.n)                                            */
/*----------------------------------------------------------------------------*/
char Card_Term::cGetTblSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->szSymbol();

  char cSyn = szTerm->azChar()[1];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_Term(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(Symbol* pDiffSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_clone = pClone();

  p_term_clone->vSymbol(pDiffSymbol->szSymbol());

  return p_term_clone;
}

/*----------------------------------------------------------------------------*/
/* is this term a column-value ordinal? $n.c                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bIsColumnOrdinal())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a key ordinal? $Kn                                            */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsKeyOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIsKeyOrdinal();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a parameter ordinal? %n                                       */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsPrmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIsParameterOrdinal();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a stream-value ordinal? $n                                    */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIsStreamOrdinal();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a value ordinal? $n or $c.n                                   */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsValOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsStmOdn())
    return true;
  else
  if (bIsColOdn())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->Y_bHas(P_Column)) {
    vPrintCard_bySettings(pRibbon, iDepth);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s256 sTerm = szSymbol();
  s32 sType_short = pSymbol()->Y_szGet(P_Type);

  pRibbon->vSprintRight("%s %s"_Stz,sTerm.szStz(),sType_short.szStz());

  //vPrintCard_bySettings(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* is                                                                         */
/*----------------------------------------------------------------------------*/
bool Card_Term::bSameAs(Card_Term* p_term_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = bIsSymbol(p_term_arg->szSymbol());

  return b;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
