/*============================================================================*/
/* Card_IdxColNm.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxColNm.hpp"
#include "Card_Term.hpp"
#include "Column.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_IdxColNm, CK_IDXCOLNM)
__aNewFunction(Card_IdxColNm, CN_CARD)
__vDeleteFunction(Card_IdxColNm, CN_CARD)

__bIsCard(STORE, Card_IdxColNm, CK_IDXCOLNM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IdxColNm::Card_IdxColNm(Column* pColumn)
: Card(CK_IDXCOLNM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vValue(pColumn->szColNm());

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IdxColNm::~Card_IdxColNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
