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

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Atom::Card_Atom(Stz* szAtom)
: Card(CN_CARD_ATOM,"Atom"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
      
  m_fpDeleteThis = &Card_Atom::vDeleteThis_;
  m_fpBoardPrint = &Card_Atom::vBoardPrint_;
  
  m_sAtom = szAtom;
  m_bPrintNm = false;
  
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
/* get atom                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card_Atom::szAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
  
  return m_sAtom;
}

/*----------------------------------------------------------------------------*/
/* set atom                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Atom::vAtom(Stz* szAtom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
  
  m_sAtom = szAtom;
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);

       if (m_sAtom.bSame("avg"_Stz))      return true;
  else if (m_sAtom.bSame("count"_Stz))    return true;
  else if (m_sAtom.bSame("countAll"_Stz)) return true;
  else if (m_sAtom.bSame("max"_Stz))      return true;
  else if (m_sAtom.bSame("min"_Stz))      return true;
//  else if (m_sAtom.bSame("stddev"_Stz))   return true;
  else if (m_sAtom.bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Atom::bIsOrdinalColumnAtom(void)
{
  _static_
  
  char acFirst  = m_sAtom.azChar()[0];
  char acSecond = m_sAtom.azChar()[1];

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
  
  char acFirst  = m_sAtom.azChar()[0];
  char acSecond = m_sAtom.azChar()[1];

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

  if (m_sAtom.bSame(szArg))
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

  if (m_sAtom.bSame(","_Stz))
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

  i8 i_Ord = StrLib::iGleanInt(m_sAtom.azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Atom::iOrdinalIndexValue(void)
{
  _static_

  i8 i_Ord = StrLib::iGleanInt(m_sAtom.azChar());
  
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

/*----------------------------------------------------------------------------*/
/* print atom piece                                                           */
/*----------------------------------------------------------------------------*/
void Card_Atom::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Atom* pCard_Atom = _pCard_Atom(pCard);

  s32 sAtom;

  sAtom.vSprint("%s"_Stz,pCard_Atom->m_sAtom.szStz());

  pRibbon->vSprint("%15s"_Stz,sAtom.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
