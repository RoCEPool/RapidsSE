/*============================================================================*/
/* Card_StreamKey.cpp -- Stream Key                                           */
/*============================================================================*/

#include "Card_StreamKey.hpp"
#include "Card_Table.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"
#include "Card_TableNm.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamKey,CK_STREAMKEY)
__aNewFunction(Card_StreamKey,CN_CARD)
__vDeleteFunction(Card_StreamKey,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamKey::Card_StreamKey(void)
: Card_Key(CK_STREAMKEY,4)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamKey::~Card_StreamKey(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamKey* Card_StreamKey::pTo_streamKey(Card_Key* p_key)
{
  _static_

  Card_Key* p_key_dupe = _pCard_Key(p_key->pDupCardRecursive());

  Card_StreamKey* p_streamKey = _pCard_StreamKey(p_key_dupe->pCard(CK_STREAMKEY));

  p_streamKey->vFixedDepth(4);
  return p_streamKey;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_StreamKey::bIsEquivalentTo(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vDump(__func__ ,"this streamKey"_Stz);
  //p_key_arg->vDump(__func__ ,"p_key_arg"_Stz);


  i8 nExpr = this->nChildCards();
  if (nExpr != p_key_arg->nChildCards())
    return false;

  for (Kid xStreamKey(this); xStreamKey.bEach(CK_DIRECTION); /**/) {
    Card_Direction *p_direction_this = xStreamKey.p_direction();
    Card_Expr *p_expr_this = p_direction_this->pSub_expr();
    Card_Direction *p_direction_arg = p_key_arg->pGet_direction_z(xStreamKey.i_Child());
    Card_Expr *p_expr_arg = p_direction_arg->pSub_expr();

    if (!p_expr_this->bSameAs(p_expr_arg))
      return false;

    if (!p_direction_this->pSymbol()->bMatches(p_direction_arg->pSymbol()))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamKey::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pParent->bIsCard(CK_t_EMIT));

/**Z_KEY_DIRECTION validate*/
  vValidate_mustHave(CK_DIRECTION,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
