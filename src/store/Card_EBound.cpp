/*============================================================================*/
/* Card_EBound.cpp -- Equi-Bound                                              */
/*============================================================================*/

#include "Card_EBound.hpp"
#include "CardIter.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_EBound,CK_EBOUND)
__aNewFunction(Card_EBound,CN_CARD)
__vDeleteFunction(Card_EBound,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_EBound::Card_EBound(void)
: Card_BOUND(CK_EBOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_EBound::~Card_EBound(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get $In of ebound                                                          */
/*----------------------------------------------------------------------------*/
Card_Term* Card_EBound::pGet_idxColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild(CK_TERM)))
    if (Card::bIsIndexColOrdinal(p_term->szValue()))
      return p_term;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $In of ebound                                                          */
/*----------------------------------------------------------------------------*/
Stz* Card_EBound::szGet_idxColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild(CK_TERM)))
    if (Card::bIsIndexColOrdinal(p_term->szValue()))
      return p_term->szValue();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_EBound::pGet_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild(CK_TERM)))
    if (Card::bIsTableColOrdinal(p_term->szValue()))
      return p_term;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card_EBound::szGet_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild(CK_TERM)))
    if (Card::bIsTableColOrdinal(p_term->szValue()))
      return p_term->szValue();

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
