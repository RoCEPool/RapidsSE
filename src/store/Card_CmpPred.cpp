/*============================================================================*/
/* Card_CmpPred.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_ColumnRef.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CmpPred, CK_CMPPRED)
__aNewFunction(Card_CmpPred,CN_CARD)
__vDeleteFunction(Card_CmpPred,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CmpPred::Card_CmpPred(void)
: Card_PRED(CK_CMPPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CmpPred::Card_CmpPred(Stz* szCmp)
    : Card_PRED(CK_CMPPRED, szCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred::~Card_CmpPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bCoPtnWith(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

  bool bAreCoPtn = false;

  if (   cTableSyn_arg_0 == cTableSyn_this_0
         || cTableSyn_arg_0 == cTableSyn_this_1
         || cTableSyn_arg_1 == cTableSyn_this_0
         || cTableSyn_arg_1 == cTableSyn_this_1
      )
    bAreCoPtn = true;

  vTrace("(%c,%c) - (%c,%c) are%s co-partitioned\n"_Stz,
         cTableSyn_this_0,cTableSyn_this_1,
         cTableSyn_arg_0,cTableSyn_arg_1,
         bAreCoPtn ? "" : " NOT");

  return bAreCoPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bIsEq(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

  if (cTableSyn_this_0 == cTableSyn_arg_0 &&
      cTableSyn_this_1 == cTableSyn_arg_1)
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bOverlapsWith(Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

  if (   cTableSyn_this_0 == cTableSyn_arg_0
      || cTableSyn_this_0 == cTableSyn_arg_1
      || cTableSyn_this_1 == cTableSyn_arg_0
      || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bUsesTable(char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
