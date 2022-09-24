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
: Card_SUB(CK_IDXCOLLIST)
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
/* find <idxCol> which is given ordinal ($n)                                  */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pFind_idxCol(Stz *szOrdinal, i8& i_IdxColNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxCol* pCard_idxCol = nullptr;

  i8 nSubCards = this->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = this->pGetSubCard(i_SubCard);

    if (CK_IDXCOL != pSubCard->eCard())
      continue;

    Card_IdxCol* pCard_idxCol_test = _pCard_IdxCol(pSubCard);

    Stz* szOrdinal_col = pCard_idxCol_test->szOrdinal();
    if (szOrdinal_col->bSame(szOrdinal)) {
      pCard_idxCol = pCard_idxCol_test;
      i_IdxColNo = i_SubCard;
      break;
    }
  }

  return pCard_idxCol;
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vOrdinate(Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iOrdLo = pCard_Stream->iOrdLo();

  Card_StreamColList* pCard_StreamColList = _pGetSub_Card_StreamColList(pCard_Stream);

  i8 nSubCards = this->nSubCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(pGetSubCard(i_Sub));

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
