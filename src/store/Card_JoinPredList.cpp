/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "Card_CalcBool.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_PredCmp.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPredList,CK_JOINPREDLIST)
__aNewFunction(Card_JoinPredList,CN_CARD)
__vDeleteFunction(Card_JoinPredList,CN_CARD)

__bIsCard(STORE,Card_JoinPredList,CK_JOINPREDLIST)
__pGetSubFunction_Card(STORE,JoinPredList,CK_JOINPREDLIST)

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
void Card_JoinPredList::vExtract_joinPredList(Card_CalcBool* p_calcBool)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  auto bfExtract = [this,p_calcBool](Card *pCard) {

    Card_PredCmp* p_predCmp = _pCard_PredCmp(pCard);

    if (! p_predCmp->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_predCmp->pGetChild(0));
    Card_ColumnRef* p_columnRef_0 = _pGetSub_ColumnRef(p_term_0);
    if (! p_columnRef_0)
      return true;
    Card_Term* p_term_1 = _pCard_Term(p_predCmp->pGetChild(1));
    Card_ColumnRef* p_columnRef_1= _pGetSub_ColumnRef(p_term_1);
    if (! p_columnRef_1->bIsColumnRef())
      return true;

    Card_PredCmp* p_predCmp_new = new Card_PredCmp;
    p_predCmp_new->vAppend(p_columnRef_0->pDupCardAndSubCards());
    p_predCmp_new->vAppend(p_columnRef_1->pDupCardAndSubCards());

    this->vAppend(p_predCmp_new);

    return true;
  };

  // - - - - - - - - -

  p_calcBool->vVisitAllSub(CK_PREDCMP,bfExtract);

}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
