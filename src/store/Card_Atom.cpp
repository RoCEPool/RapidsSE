/*============================================================================*/
/* Card_Atom.cpp -- Atom Card                                                 */
/*============================================================================*/

#include "Card_Atom.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Atom,CN_CARD_ATOM)
__aNewFunction(Card_Atom,CN_CARD_ATOM)
__vDeleteFunction(Card_Atom,CN_CARD_ATOM)
__aFooterFunction(Card_Atom)

__AssertFunction(STORE,Card_Atom,CN_CARD_ATOM)
__bIsClass(STORE,Card_Atom,CN_CARD_ATOM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Atom::Card_Atom(eCK eKind, Stz* szAtom)
: Card(CK_ATOM,CN_CARD_ATOM,"Atom"_Stz,"atom"_Stz,szAtom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
      
  m_fpDeleteThis = &Card_Atom::vDeleteThis_;
  
  _checkInitialised_(CN_CARD_ATOM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Atom::~Card_Atom(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

       if (szAtom->bSame("avg"_Stz))      return true;
  else if (szAtom->bSame("count"_Stz))    return true;
  else if (szAtom->bSame("countAll"_Stz)) return true;
  else if (szAtom->bSame("max"_Stz))      return true;
  else if (szAtom->bSame("min"_Stz))      return true;
//  else if (szAtom->bSame("stddev"_Stz))   return true;
  else if (szAtom->bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsOrdinalColumnAtom(void)
{
  _static_
  
  Stz* szAtom = szGetProperty("atom"_Stz);

  char acFirst  = szAtom->azChar()[0];
  char acSecond = szAtom->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom an index ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsOrdinalIndexAtom(void)
{
  _static_
  
  Stz* szAtom = szGetProperty("atom"_Stz);

  char acFirst  = szAtom->azChar()[0];
  char acSecond = szAtom->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom same as argument?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsSame(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(szArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an separator?                                                 */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card_Atom::iOrdinalColumnValue(void)
{
  _static_

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Atom::iOrdinalIndexValue(void)
{
  _static_

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print hashcast                                                             */
/*----------------------------------------------------------------------------*/
void Card_Atom::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Atom*)pCard;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
