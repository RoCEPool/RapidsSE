/*============================================================================*/
/* Card_ColumnRef.cpp -- Column Reference                                     */
/*============================================================================*/

#include "Card_ColumnRef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_ColumnRef, CK_COLUMNREF)
__aNewFunction(Card_ColumnRef, CN_CARD)
__vDeleteFunction(Card_ColumnRef, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ColumnRef::Card_ColumnRef(Stz* szColumnRef)
: Card(CK_COLUMNREF,szColumnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ColumnRef::~Card_ColumnRef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_ColumnRef::bIsColumnRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0];
  char cSecond = szColumnRef->azChar()[1];
  char cThird  = szColumnRef->azChar()[2];
  char cFourth = szColumnRef->azChar()[3];

  if ('$' == cFirst && '$' == cSecond && '.' == cFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 Card_ColumnRef::iColumnRef_columnOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char* azColumnOrd = (szColumnRef->azChar() + 3);

  i8 iColumnOrd = StrLib::iGleanInt(azColumnOrd);

  return iColumnOrd;
}

/*----------------------------------------------------------------------------*/
/* get table syn of column ref                                                */
/*----------------------------------------------------------------------------*/
char Card_ColumnRef::cColumnRef_tableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Card_ColumnRef::cTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ColumnRef::vTableSyn(String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  psTableSyn->vSprint("%c"_Stz,cTableSyn());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
