/*============================================================================*/
/* Card_TableList.cpp -- Table List                                           */
/*============================================================================*/

#include "Card_TableList.hpp"

#include "CardIter.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_Size.hpp"
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
: Card(CK_TABLELIST)
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
Card_TableList* Card_TableList::pExtract(Card_Deck* p_deck)
{
  _static_

  Card_TableList* p_tableList = new Card_TableList;

  CardIter xCI(p_deck);

  if (! xCI.pEachChild(CK_FOLD))
    xCI.vRestart();

  while (Card_Table* p_table = _pCard_Table(xCI.pEachChild(CK_TABLE))) {

    Card* p_table_dup = _pCard_Table(p_table->pDup());
    p_tableList->vAppend(p_table_dup);

    if (Card_TableNm* p_tableNm = p_table->pSub_tableNm()) {
      p_tableNm = _pCard_TableNm(p_tableNm->pDup());
      p_table_dup->vAppend(p_tableNm);
    }
    if (Card_TablePtn* p_tablePtn = p_table->pSub_tablePtn()) {
      p_tablePtn = _pCard_TablePtn(p_tablePtn->pDup());
      p_table_dup->vAppend(p_tablePtn);
    }
    if (Card_Size* p_size = p_table->pSub_size()) {
      p_size = _pCard_Size(p_size->pDup());
      p_table_dup->vAppend(p_size);
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
    char cTableSyn = p_table->cValue();
    if (cTableSyn == cTblSyn)
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TableList::bIsCmpPredCoPtn(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  char cTableSyn_0 = p_term_0->szValue()->azChar()[1];
  Card_Term* p_term_1 = _pCard_Term(p_cmpPred->pGetChild(1));
  char cTableSyn_1 = p_term_1->szValue()->azChar()[1];

  if (StrLib::bIsUpperCase(cTableSyn_0) || StrLib::bIsUpperCase(cTableSyn_1))
    return true;

  Card_Table* p_table_0 = pFind(cTableSyn_0);
  Card_TablePtn* p_tablePtn_0 = p_table_0->pSub_tablePtn();
  Stz* szTablePtn_0 = p_tablePtn_0->sz();

  Card_Table* p_table_1 = pFind(cTableSyn_1);
  Card_TablePtn* p_tablePtn_1 = p_table_1->pSub_tablePtn();
  Stz* szTablePtn_1 = p_tablePtn_1->sz();

  if (szTablePtn_0->bSame(p_term_0->sz()) && szTablePtn_1->bSame(p_term_1->sz()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TablePtn* Card_TableList::pGetTablePtn(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMainWithSub(CK_TABLE,CK_TABLEPTN)) {
    if (xCI.G.p_table->bIsValue(cTblSyn))
      return xCI.G.p_tablePtn;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Size* Card_TableList::pGetTableSize(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMainWithSub(CK_TABLE,CK_SIZE)) {
    if (xCI.G.p_table->bIsValue(cTblSyn))
      return xCI.G.p_size;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_TableList::szToTableNm(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMainWithSub(CK_TABLE,CK_TABLENM)) {
    if (xCI.G.p_table->bIsValue(cTblSyn))
      return xCI.G.p_tableNm->szValue();
  }

  return nullptr;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
