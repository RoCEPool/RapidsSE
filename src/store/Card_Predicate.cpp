/*============================================================================*/
/* Card_Predicate.cpp -- Predicate                                            */
/*============================================================================*/

#include "Card_Predicate.hpp"

#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Predicate,CK_PREDICATE)
__aNewFunction(Card_Predicate,CN_CARD)
__vDeleteFunction(Card_Predicate,CN_CARD)

__bIsCard(STORE,Card_Predicate,CK_PREDICATE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Predicate::Card_Predicate(void)
: Card(CK_PREDICATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Predicate::~Card_Predicate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* to expr                                                                    */
/*----------------------------------------------------------------------------*/
void Card_Predicate::vToExpr(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  szExpr->vCopy(""_Stz);
  
  i8 nSubCards = nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_Term* pCard_Term = _pCard_Term(pGet(i_Sub));
    
    Stz* szTerm = pCard_Term->szGetProperty((i8)0);
    
    if (i_Sub)
      szExpr->vConcat(" "_Stz);

    szExpr->vConcat(szTerm);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
