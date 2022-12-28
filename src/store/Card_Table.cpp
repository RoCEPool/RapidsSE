/*============================================================================*/
/* Card_Table.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_Table.hpp"

#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"
#include "Card_Filter.hpp"
#include "Card_BoolCalc.hpp"
#include "CardIter.hpp"
#include "Card_Index.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Bounds.hpp"
#include "Card_Bound.hpp"
#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Table, CK_TABLE)
__aNewFunction(Card_Table, CN_CARD)
__vDeleteFunction(Card_Table, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(void)
: Card(CK_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(Stz* szTableNm)
    : Card(CK_TABLE,szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table::~Card_Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Card_Table::vBounds_DeriveFrom_filter(Card_Filter* p_filter,
                                          bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // TODO: this is a hack to put bounds under one and only one index

  Card_IndexList* p_indexList = this->pSub_indexList();
  //Card_Index* p_index = this->pSub_index();

  CardIter xCI(p_indexList);
  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bLambdaFn = [this, p_index, bParamAllowedInBounds](Card_PRED *p_pred_Of_filter, bool bUpwards) {
      if (bUpwards)
        return C_CONTINUE_RECURSION;

      if (!p_pred_Of_filter->bIsCard(CK_CMPPRED))
        return C_CONTINUE_RECURSION;

      Card_CmpPred *p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);

      //if (!T_bCmpPred_MaybeBounds(p_cmpPred,this,bParamAllowedInBounds))
      if (!p_cmpPred->bMaybeBounds_v2(this, p_index, bParamAllowedInBounds))
        return C_CONTINUE_RECURSION;

      // - - - -

      Card_Bounds *p_bounds = this->pSub_bounds();
      if (!p_bounds) {
        p_bounds = new Card_Bounds;
        p_index->vAppend(p_bounds);
      }

      char cTblSyn = this->cValue();
      Card_Bound *p_bound = Card_Bound::pBound_CreateFrom_cmpPred(p_cmpPred, cTblSyn);
      p_bounds->vAppend(p_bound);

      p_cmpPred->vMark1(MARK_UP);

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bIgnore = p_pred_ofFilter_ROOT->bPredRecursive(bLambdaFn);

  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
