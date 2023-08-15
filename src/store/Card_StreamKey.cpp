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
: Card_Key(CK_STREAMKEY)
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

  UNUSED(pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
