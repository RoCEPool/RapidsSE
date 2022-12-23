/*============================================================================*/
/* Card_IdxColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Stream.hpp"
#include "Card_StmCol.hpp"
#include "Card_StmColList.hpp"
#include "Card_Term.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IdxColList,CK_IDXCOLLIST)
__aNewFunction(Card_IdxColList,CN_CARD)
__vDeleteFunction(Card_IdxColList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IdxColList::Card_IdxColList(void)
    : Card(CK_IDXCOLLIST)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IdxColList::~Card_IdxColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pIdxCol_byIdxColOdn(Stz* szIdxColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_IdxCol = StrLib::iGleanInt(szIdxColOdn->azChar()) - 1;

  if (i_IdxCol < 0 || i_IdxCol >= nChildCards())
    return nullptr;

  Card_IdxCol* pIdxCol = _pCard_IdxCol(pGetChild(i_IdxCol));

  return pIdxCol;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
