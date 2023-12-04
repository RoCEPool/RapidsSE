/*============================================================================*/
/* Card_Hash.cpp -- Hash                                                      */
/*============================================================================*/

#include "Card_Hash.hpp"

#include "Card_HshCol.hpp"
#include "Card_Expr.hpp"
#include "Card_Term.hpp"
#include "Card_Emit.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Hash, CK_o_HASH)
__aNewFunction(Card_Hash,CN_CARD)
__vDeleteFunction(Card_Hash,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Hash::Card_Hash(void)
: Card_OP(CK_o_HASH, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Hash::~Card_Hash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Hash* Card_Hash::pDerive(Card_Expr* p_expr,
                              Card_Emit* p_emitted_prev)
{
  _static_
  UNUSED(p_emitted_prev);

  Card_Hash* p_hash = new Card_Hash;

  Card_Stream* p_stream = new Card_Stream;
  p_hash->vAppendChild(p_stream);

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_hash->vAppendChild(p_level);

  Card_HshColList* p_hshColList = new Card_HshColList;
  p_hash->vAppendChild(p_hshColList);

  Card_HshCol* p_hshCol = new Card_HshCol;
  p_hshColList->vAppendChild(p_hshCol);

  Card_Term* p_card_term = p_expr->pGet_term(0)->pClone();
  p_hshCol->vAppendChild(p_card_term);

  if (p_emitted_prev)
    p_stream->vAppendChild(p_emitted_prev->pClone());

  return p_hash;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Hash::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
