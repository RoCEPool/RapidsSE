/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPredList,CK_JOINPREDLIST)
__aNewFunction(Card_JoinPredList,CN_CARD)
__vDeleteFunction(Card_JoinPredList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::Card_JoinPredList(void)
: Card(CK_JOINPREDLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::~Card_JoinPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vExtract_joinPredList(Card_BoolCalc* p_calcBool)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  auto bfExtract = [this,p_calcBool](Card *pCard) {

    Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);

    if (! p_predCmp->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_predCmp->pGetChild(0));
    Card_ColumnRef* p_columnRef_0 = p_term_0->pSub_columnRef();
    if (! p_columnRef_0)
      return true;
    Card_Term* p_term_1 = _pCard_Term(p_predCmp->pGetChild(1));
    Card_ColumnRef* p_columnRef_1= p_term_1->pSub_columnRef();
    if (! p_columnRef_1->bIsColumnRef())
      return true;

    Card_CmpPred* p_predCmp_new = new Card_CmpPred(p_predCmp->sz());
    p_predCmp_new->vAppend(p_columnRef_0->pDupCardAndSubCards());
    p_predCmp_new->vAppend(p_columnRef_1->pDupCardAndSubCards());

    this->vAppend(p_predCmp_new);

    return true;
  };

  // - - - - - - - - -

  p_calcBool->vVisitRecursive(CK_CMPPRED, bfExtract);

}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
