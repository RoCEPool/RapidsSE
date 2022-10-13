/*============================================================================*/
/* Card_TableList.cpp -- Table List                                           */
/*============================================================================*/

#include "Card_TableList.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtn.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CmpPred.hpp"

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Card_TableList::pFind_table(char cSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_TABLE != pChildCard->eCard())
      continue;

    Card_Table* p_table = _pCard_Table(pChildCard);
    char cTableSyn = p_table->Y_cVal(P_TableSyn);
    if (cTableSyn == cSyn)
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TableList::bPredCmpIsCoPtn(Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_ColumnRef* p_columnRef_0 =
      _pCard_ColumnRef(p_predCmp->pGetChild(0));
  Stz* szPredColumnRef_0 = p_columnRef_0->sz();

  Card_ColumnRef* p_columnRef_1 =
      _pCard_ColumnRef(p_predCmp->pGetChild(1));
  Stz* szPredColumnRef_1 = p_columnRef_1->sz();

  char cTableSyn_0 = p_columnRef_0->cColumnRef_tableSyn();
  char cTableSyn_1 = p_columnRef_1->cColumnRef_tableSyn();

  if (StrLib::bIsUpperCase(cTableSyn_0) || StrLib::bIsUpperCase(cTableSyn_1))
    return true;

  Card_Table* p_table_0 = pFind_table(cTableSyn_0);
  Card_TablePtn* p_tablePtn_0 = p_table_0->pSub_tablePtn();
  Stz* szTablePtn_0 = p_tablePtn_0->sz();

  Card_Table* p_table_1 = pFind_table(cTableSyn_1);
  Card_TablePtn* p_tablePtn_1 = p_table_1->pSub_tablePtn();
  Stz* szTablePtn_1 = p_tablePtn_1->sz();

  if (szTablePtn_0->bSame(szPredColumnRef_0) && szTablePtn_1->bSame(szPredColumnRef_1))
    return true;

  return false;
}


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
