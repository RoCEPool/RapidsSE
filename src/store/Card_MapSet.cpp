/*============================================================================*/
/* Card_MapSet.cpp -- MapSet                                                  */
/*============================================================================*/

#include "Card_Map.hpp"
#include "Card_MapSet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_MapSet,CK_MAPSET)
__aNewFunction(Card_MapSet,CN_CARD)
__vDeleteFunction(Card_MapSet,CN_CARD)

__bIsCard(STORE,Card_MapSet,CK_MAPSET)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MapSet::Card_MapSet(void)
: Card_SUB(CK_MAPSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MapSet::Card_MapSet(Index* pIndex, Stz* szPrefix)
: Card_SUB(CK_MAPSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);
    vTrace("IdxCol : %s\n"_Stz,pIdxCol->szColNm());

    s256 sAltNm;
    sAltNm.vSprint("%s%s"_Stz,szPrefix,pIdxCol->szColNm());

    Card_Map* pCard_Map = new Card_Map;
    pCard_Map->vMapFrom(sAltNm);
    pCard_Map->vMapBack(pIdxCol->szColNm());

    this->vAppend(pCard_Map);
  }

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_MapSet::~Card_MapSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* look up mapFrom and return mapBack                                         */
/*----------------------------------------------------------------------------*/
Stz* Card_MapSet::szMapBack(Stz* szMapFrom)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card_Map *pCard_Map = _pCard_Map(pGetChildCard(i_Child));

    if (pCard_Map->szMapFrom()->bSame(szMapFrom))
      return pCard_Map->szMapBack();
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
