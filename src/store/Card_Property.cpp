/*============================================================================*/
/* Card_Property.cpp -- Property                                              */
/*============================================================================*/

#include "Card_Property.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Property,CN_CARD_PROPERTY)
__aNewFunction(Card_Property,CN_CARD_PROPERTY)
__vDeleteFunction(Card_Property,CN_CARD_PROPERTY)
__aFooterFunction(Card_Property)

__AssertFunction(STORE,Card_Property,CN_CARD_PROPERTY)
__bIsClass(STORE,Card_Property,CN_CARD_PROPERTY)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Property::Card_Property(eCK eKind, Stz* szAtom)
: Card(eKind,CN_CARD_PROPERTY,"Atom"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);
      
  m_fpDeleteThis = &Card_Property::vDeleteThis_;
  m_fpBoardPrint = &Card::vCardPrint_;
  
  vAddProperty("atom"_Stz,szAtom);
  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD_PROPERTY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Property::~Card_Property(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card_Property::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);

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
bool Card_Property::bIsOrdinalColumnAtom(void)
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
bool Card_Property::bIsOrdinalIndexAtom(void)
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
bool Card_Property::bIsSame(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(szArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an separator?                                                 */
/*----------------------------------------------------------------------------*/
bool Card_Property::bIsSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROPERTY,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card_Property::iOrdinalColumnValue(void)
{
  _static_

  Stz* szAtom = szGetProperty("atom"_Stz);

  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Property::iOrdinalIndexValue(void)
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
void Card_Property::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Property*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
