/*============================================================================*/
/* Card_t.hpp -- Card (template)                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  template <class CARD>
  class Card_t : public Card
  {
  private:
    
  public:
    Card_t(eCK eCard);
    Card_t(eCK eCard, Symbol* pSymbol, bool bCopyProperties=true);
    Card_t(eCK eCard, Stz* szSymbol, eSF eSymbol);

    ~Card_t(void) noexcept(false);
  //CARD* operator [](Symbol* pSymbol);
    CARD* SYM(Symbol xSymbol);
    CARD* SYM(Symbol* pSymbol);
    CARD* SYMPPY(Symbol* pSymbol);

    CARD* SYM(eSF eSymbol);
    CARD* SYM(eSF eSymbol, char cChr);
    CARD* SYM(eSF eSymbol, char cChr, i8 iInt);
    CARD* SYM(eSF eSymbol, i8 iInt);
    CARD* SYM(eSF eSymbol, Stz* szStz);
  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_t<CARD>::Card_t(eCK eCard)
    : Card(eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}
/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_t<CARD>::Card_t(eCK eCard, Symbol* pSymbol, bool bCopyProperties)
    : Card(eCard, pSymbol, bCopyProperties)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_t<CARD>::Card_t(eCK eCard, Stz* szSymbol, eSF eSymbol)
    : Card(eCard, szSymbol, eSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_t<CARD>::~Card_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator : []                       ]                                      */
/*----------------------------------------------------------------------------*/
//template <class CARD>
//CARD* Card_t<CARD>::operator [](Symbol* pSymbol)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  vSymbol(pSymbol);
//
//  return this;
//}

/*----------------------------------------------------------------------------*/
/* SYM                                                                        */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(Symbol xSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vSymbol(&xSymbol);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vSymbol(pSymbol);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYMPPY(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vSymbol(pSymbol);
  m_xSymbol.vUpdate(pSymbol,true);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(eSF eSymbol, char cChr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,cChr);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(eSF eSymbol, char cChr, i8 iInt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,cChr,iInt);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(eSF eSymbol, i8 iInt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,iInt);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_t<CARD>::SYM(eSF eSymbol, Stz* szStz)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,szStz);

  return (CARD*)this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
