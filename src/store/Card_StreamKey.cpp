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

__pCastFunction_Card(STORE, Card_StreamKey, CK_c_STREAMKEY)
__aNewFunction(Card_StreamKey,CN_CARD)
__vDeleteFunction(Card_StreamKey,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamKey::Card_StreamKey(void)
: Card_Key(CK_c_STREAMKEY, 3)
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

  Card_StreamKey* p_streamKey = _pCard_StreamKey(p_key_dupe->pCard(CK_c_STREAMKEY));

  p_streamKey->vFixedDepth(3);

  return p_streamKey;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamKey::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

/**Z_KEY_DIRECTION validate*/
#ifdef Z_PART_STREAMKEY
  RASSERT(CK_c_EMIT != pParent->eCard())
#else
  RASSERT(pParent->bIsCard(CK_c_EMIT));
#endif

  vValidate_mustHave(CK_EXPR,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
