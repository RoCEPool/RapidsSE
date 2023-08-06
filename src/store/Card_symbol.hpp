/*============================================================================*/
/* Card_symbol.hpp -- Card (template)                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  template <class CARD>
  class Card_symbol : public Card
  {
  private:
    
  public:
    Card_symbol(eCK eCard);
    Card_symbol(eCK eCard, Symbol* pSymbol, bool bCopyProperties=true);
    Card_symbol(eCK eCard, Stz* szSymbol, eSF eSymbol);

    ~Card_symbol(void) noexcept(false);
  //CARD* operator [](Symbol* pSymbol);
    CARD* SYM(Symbol xSymbol);
    CARD* SYM(Symbol* pSymbol);
    CARD* SYMPPY(Symbol* pSymbol);

    CARD* SYM(eSF eSymbol);
    CARD* SYM(eSF eSymbol, char cChr);
    CARD* SYM(eSF eSymbol, char cChr, i8 iInt);
    CARD* SYM(eSF eSymbol, i8 iInt);
    CARD* SYM(eSF eSymbol, Stz* szStz);
    CARD* SYM(eSF eSymbol, i8 iInt, Symbol *pSymbol);
  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_symbol<CARD>::Card_symbol(eCK eCard)
    : Card(eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}
/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_symbol<CARD>::Card_symbol(eCK eCard, Symbol* pSymbol, bool bCopyProperties)
    : Card(eCard, pSymbol, bCopyProperties)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_symbol<CARD>::Card_symbol(eCK eCard, Stz* szSymbol, eSF eSymbol)
    : Card(eCard, szSymbol, eSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class CARD>
Card_symbol<CARD>::~Card_symbol(void) noexcept(false)
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
//CARD* Card_symbol<CARD>::operator [](Symbol* pSymbol)
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
CARD* Card_symbol<CARD>::SYM(Symbol xSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vSymbol(&xSymbol);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vSymbol(pSymbol);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYMPPY(Symbol* pSymbol)
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
CARD* Card_symbol<CARD>::SYM(eSF eSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(eSF eSymbol, char cChr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,cChr);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(eSF eSymbol, char cChr, i8 iInt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,cChr,iInt);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(eSF eSymbol, i8 iInt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,iInt);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(eSF eSymbol, Stz* szStz)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,szStz);

  return (CARD*)this;
}

/*----------------------------------------------------------------------------*/
/*  SYM                                                                       */
/*----------------------------------------------------------------------------*/
template <class CARD>
CARD* Card_symbol<CARD>::SYM(eSF eSymbol, i8 iInt, Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.vSym(eSymbol,iInt,pSymbol);

  return (CARD*)this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
