/*============================================================================*/
/* Card_Term.cpp -- Term Card                                                 */
/*============================================================================*/

#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Term,CN_CARD_TERM)
__aNewFunction(Card_Term,CN_CARD_TERM)
__vDeleteFunction(Card_Term,CN_CARD_TERM)
__aFooterFunction(Card_Term)

__AssertFunction(STORE,Card_Term,CN_CARD_TERM)
__bIsClass(STORE,Card_Term,CN_CARD_TERM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(eCK eKind, Stz* szTerm)
: Card(CK_TERM,CN_CARD_TERM,"Term"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);
      
  m_fpDeleteThis = &Card_Term::vDeleteThis_;
  m_fpBoardPrint = &Card_Term::vBoardPrint_;
  
  m_sTerm = szTerm;
  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD_TERM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term::~Card_Term(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get atom                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card_Term::szTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);
  
  return m_sTerm;
}

/*----------------------------------------------------------------------------*/
/* set atom                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Term::vTerm(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);
  
  m_sTerm = szTerm;
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);

       if (m_sTerm.bSame("avg"_Stz))      return true;
  else if (m_sTerm.bSame("count"_Stz))    return true;
  else if (m_sTerm.bSame("countAll"_Stz)) return true;
  else if (m_sTerm.bSame("max"_Stz))      return true;
  else if (m_sTerm.bSame("min"_Stz))      return true;
//  else if (m_sTerm.bSame("stddev"_Stz))   return true;
  else if (m_sTerm.bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsOrdinalColumnTerm(void)
{
  _static_
  
  char acFirst  = m_sTerm.azChar()[0];
  char acSecond = m_sTerm.azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom an index ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsOrdinalIndexTerm(void)
{
  _static_
  
  char acFirst  = m_sTerm.azChar()[0];
  char acSecond = m_sTerm.azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom same as argument?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsSame(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);

  if (m_sTerm.bSame(szArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an separator?                                                 */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TERM,_FOOTER_);

  if (m_sTerm.bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iOrdinalColumnValue(void)
{
  _static_

  i8 i_Ord = StrLib::iGleanInt(m_sTerm.azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iOrdinalIndexValue(void)
{
  _static_

  i8 i_Ord = StrLib::iGleanInt(m_sTerm.azChar());
  
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
void Card_Term::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Term*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print atom piece                                                           */
/*----------------------------------------------------------------------------*/
void Card_Term::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Term* pCard_Term = _pCard_Term(pCard);
  
  pRibbon->vSput(FR_ATOM,pCard_Term->m_sTerm.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
