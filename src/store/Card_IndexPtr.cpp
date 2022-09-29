/*============================================================================*/
/* Card_IndexPtr.cpp -- Index                                                    */
/*============================================================================*/

#include "Card_IdxColList.hpp"
#include "Card_IndexPtr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_IndexPtr, CK_INDEXPTR)
__aNewFunction(Card_IndexPtr, CN_CARD)
__vDeleteFunction(Card_IndexPtr, CN_CARD)

__bIsCard(STORE, Card_IndexPtr, CK_INDEXPTR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexPtr::Card_IndexPtr(Index* pIndex)
: Card(CK_INDEXPTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

   vIndex(pIndex);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexPtr::~Card_IndexPtr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IndexPtr::pFind_indexCol(Stz *szColNm, i8& i_IdxCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  Card_IdxCol* p_idxCol = p_idxColList->pFind_idxCol(szColNm,i_IdxCol);

  return p_idxCol;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
