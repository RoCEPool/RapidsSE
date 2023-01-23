/*============================================================================*/
/* Card_JoinPermList.cpp -- Join Permutation List                             */
/*============================================================================*/

#include "Card_JoinPermList.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_TableList.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPermList,CK_JOINPERMLIST)
__aNewFunction(Card_JoinPermList,CN_CARD)
__vDeleteFunction(Card_JoinPermList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPermList::Card_JoinPermList(void)
: Card(CK_JOINPERMLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPermList::~Card_JoinPermList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute one join permutation                                               */
/*----------------------------------------------------------------------------*/
void Card_JoinPermList::vOnePermutedJoin(Card_TableList*    p_tableList,
                                         Card_JoinPredList* p_joinPredList,
                                         Card_JoinPermSet*  p_joinPermSet,
                                         PermGen*           pPermGen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nTables = p_tableList->nChildCards();

  vTrace("%lld :"_Stz,pPermGen->nPermutations());
  for (i8 i_Item = 0; i_Item < nTables; i_Item++) {
    i8 i_Table = pPermGen->iItem(i_Item);
    Card_Table* p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
    if (i_Item)
      vTrace(", "_Stz,i_Table);
    vTrace("%c"_Stz,p_table->cValue());
  }
  vTrace("\n"_Stz);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
