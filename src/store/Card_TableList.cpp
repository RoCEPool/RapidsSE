/*============================================================================*/
/* Card_TableList.cpp -- Table List                                           */
/*============================================================================*/

#include "Card_TableList.hpp"

#include "CardIter.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_TableSize.hpp"
#include "Card_Table.hpp"
#include "Card_TableNm.hpp"
#include "Card_TablePtn.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TableList,CK_TABLELIST)
__aNewFunction(Card_TableList,CN_CARD)
__vDeleteFunction(Card_TableList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TableList::Card_TableList(void)
: Card_symbol(CK_TABLELIST, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TableList::~Card_TableList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TableList* Card_TableList::pExtractList(Card_Deck* p_deck)
{
  _static_

  Card_TableList* p_tableList = new Card_TableList;

  Kid xCI(p_deck);
  xCI.vEnded();
  xCI.vBeginEach_reverse();

  if (xCI.bEach_reverse(CK_FOLD)) {
    xCI.vForwards(true);
  }
  else {
    xCI.vEnded();
    xCI.vBeginEach_forward();
  }

  while (xCI.bEach(CK_FROM,CK_TABLE)) {

    Card_Table* p_table = _pCard_Table(xCI.pSub1());

    Card* p_table_dup = _pCard_Table(p_table->pDupCard());
    p_tableList->vAppend(p_table_dup);

    if (Card_TableNm* p_tableNm = p_table->pSub_tableNm()) {
      p_tableNm = _pCard_TableNm(p_tableNm->pDupCard());
      p_table_dup->vAppend(p_tableNm);
    }
    if (Card_TablePtn* p_tablePtn = p_table->pSub_tablePtn()) {
      p_tablePtn = _pCard_TablePtn(p_tablePtn->pDupCard());
      p_table_dup->vAppend(p_tablePtn);
    }
    if (Card_TableSize* p_tableSize = p_table->pSub_tableSize()) {
      p_tableSize = _pCard_TableSize(p_tableSize->pDupCard());
      p_table_dup->vAppend(p_tableSize);
    }
    if (Card_RowSize* p_rowSize = p_table->pSub_rowSize()) {
      p_rowSize = _pCard_RowSize(p_rowSize->pDupCard());
      p_table_dup->vAppend(p_rowSize);
    }
    if (Card_RowCount* p_rowCount = p_table->pSub_rowCount()) {
      p_rowCount = _pCard_RowCount(p_rowCount->pDupCard());
      p_table_dup->vAppend(p_rowCount);
    }

    if (Card_KeyExists* p_keyExists = p_table->pSub_keyExists()) {
      p_keyExists = _pCard_KeyExists(p_keyExists->pDupCard());
      p_table_dup->vAppend(p_keyExists);
    }
    else
    if (Card_KeyNotExists* p_keyNotExists = p_table->pSub_keyNotExists()) {
      p_keyNotExists = _pCard_KeyNotExists(p_keyNotExists->pDupCard());
      p_table_dup->vAppend(p_keyNotExists);
    }
  }

  return p_tableList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Card_TableList::pFind(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_TABLE != pChildCard->eCard())
      continue;

    Card_Table* p_table = _pCard_Table(pChildCard);
    char cTableSyn = p_table->pSymbol()->cSF_char();
    if (cTableSyn == cTblSyn)
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RowCount* Card_TableList::pGetRowCount(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Table* p_table = pFind(cTblSyn))
    return p_table->pSub_rowCount();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RowSize* Card_TableList::pGetRowSize(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Table* p_table = pFind(cTblSyn))
    return p_table->pSub_rowSize();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TablePtn* Card_TableList::pGetTablePtn(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Table* p_table = pFind(cTblSyn))
    return p_table->pSub_tablePtn();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_TableList::nGetTableSize(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_TableSize* p_tableSize = pGetTableSize(cTblSyn))
    return p_tableSize->pSymbol()->iSF_number();

  return 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TableSize* Card_TableList::pGetTableSize(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Table* p_table = pFind(cTblSyn))
    return p_table->pSub_tableSize();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_TableList::szToTableNm(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Table* p_table = pFind(cTblSyn))
    return p_table->pSub_tableNm()->szSymbol(SF_NONE);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_TableList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
