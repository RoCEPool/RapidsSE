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
