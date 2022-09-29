/*============================================================================*/
/* Card_IdxColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Stream.hpp"
#include "Card_StreamCol.hpp"
#include "Card_StreamColList.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IdxColList,CK_IDXCOLLIST)
__aNewFunction(Card_IdxColList,CN_CARD)
__vDeleteFunction(Card_IdxColList,CN_CARD)

__bIsCard(STORE,Card_IdxColList,CK_IDXCOLLIST)
__pGetSubFunction_Card(STORE,Card_IdxColList,CK_IDXCOLLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IdxColList::Card_IdxColList(Index* pIndex)
: Card(CK_IDXCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  IndexDef* pIndexDef = pIndex->pIndexDef();

  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    Card_IdxCol* pCard_idxCol = new Card_IdxCol(pColumn);
    vAppend(pCard_idxCol);
  }
  
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
/* find <idxCol> which has given column name                                  */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pFind_idxCol(Stz *szColNm, i8& i_IdxColNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card *pChildCard = this->pGetChild(i_Child);
    if (CK_IDXCOL != pChildCard->eCard())
      continue;

    Card_IdxCol* p_idxCol = _pCard_IdxCol(pChildCard);

    Stz* szIdxColNm = p_idxCol->szColNm();
    if (szIdxColNm->bSame(szColNm)) {
      i_IdxColNo = i_Child;
      return p_idxCol;
    }
  }

  i_IdxColNo = C_UNKNOWN;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vOrdinate(Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iOrdLo = pCard_Stream->iOrdLo();

  Card_StreamColList* pCard_StreamColList = _pGetSub_Card_StreamColList(pCard_Stream);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(pGetChild(i_Sub));

    i8 i_StreamColNo = C_UNKNOWN;
    Stz* szColNm = pCard_idxCol->szColNm();
    Card_StreamCol* pCard_streamCol =
        pCard_StreamColList->pFind_streamCol(szColNm,i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vOrdinal(sOrdinal);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
