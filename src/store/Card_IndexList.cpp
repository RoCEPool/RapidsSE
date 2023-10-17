/*============================================================================*/
/* Card_IndexList.cpp -- Index List                                           */
/*============================================================================*/

#include "Card_IndexList.hpp"

#include "Index.hpp"
#include "Card_Index.hpp"
#include "Table.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexList,CK_INDEXLIST)
__aNewFunction(Card_IndexList,CN_CARD)
__vDeleteFunction(Card_IndexList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexList::Card_IndexList(void)
: Card_symbol(CK_INDEXLIST, 3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexList::~Card_IndexList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce composite <indexList> of table                                     */
/*----------------------------------------------------------------------------*/
Card_IndexList* Card_IndexList::pDerive(Table* pTable)
{
  _static_

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_Index* p_index = Card_Index::pDerive(pIndex);
    p_indexList->vAppendChild(p_index);
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete all bounds (within this indexList)                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vDeleteAll_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX))
    _pCard_Index(xKid.pChild())->vDeleteBounds();
}


/*----------------------------------------------------------------------------*/
/* find chosen index                                                          */
/*----------------------------------------------------------------------------*/
Card_Index* Card_IndexList::pFindChosenIndex(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX)) {
    if (xKid.pChild()->pSymbol()->Y_bHas(P_ChosenIndex))
      return _pCard_Index(xKid.pChild());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for crossJoin                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_crossJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX)) {
    Card_Index* p_index = _pCard_Index(xKid.pChild());
    Card_Bounds* p_bounds = xKid.pChild()->pSub_bounds();
    if (! p_bounds)
      continue;
    Card_Bound* p_bound = p_bounds->pSub_bound();
    if (! p_bound)
      continue;
    p_index->vSet_Choose_index();
    p_bound->pSymbol()->Y_vSet(P_ChosenBound);
    return;
  }

}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for indexJoin                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX)) {
    if (! xKid.pChild()->pSub_bounds())
      continue;
    Card_Index* p_index = _pCard_Index(xKid.pChild());
    Kid xCI_bounds(p_index->pSub_bounds());
    while (xCI_bounds.bEach(CK_BOUND)) {
      Card_Bound* p_bound = _pCard_Bound(xCI_bounds.pChild());

      if (p_bound->bIsEBound()) {
        p_index->vSet_Choose_index();
        p_bound->pSymbol()->Y_vSet(P_ChosenBound);
        return;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for indexScan                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX)) {
    if (! xKid.pChild()->pSub_bounds())
      continue;

    Kid xCI_bounds(xKid.pChild()->pSub_bounds());
    while (xCI_bounds.bEach(CK_BOUND)) {
      if (_pCard_Bound(xCI_bounds.pChild())->bIsLBound() ||
          _pCard_Bound(xCI_bounds.pChild())->bIsUBound()) {
        _pCard_Index(xKid.pChild())->vSet_Choose_index();
        xCI_bounds.pChild()->pSymbol()->Y_vSet(P_ChosenBound);
        return;
      }
    }
  }

}

/*----------------------------------------------------------------------------*/
/* does this index-list have a bound logically equivalent to given compare?   */
/*----------------------------------------------------------------------------*/
bool Card_IndexList::bHasEquivalentBound(Card_CmpConj* p_cmpConj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xIndexList(this);
  while (xIndexList.bEach(CK_INDEX)) {
    if (xIndexList.p_index()->bHasEquivalentBound(p_cmpConj))
      return true;
  }
  return false;
}

/*----------------------------------------------------------------------------*/
/* does an index in this indexList use sql result?                            */
/*----------------------------------------------------------------------------*/
bool Card_IndexList::bUsesSqyRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_INDEX); /**/) {
    if (xKid.p_index()->bUsesSqlRsl())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* ensure types for all stream-columns                                        */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vTypes_For_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_INDEX); /**/)
    xKid.p_index()->vEnsureTypes_For_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_INDEX))
    xKid.p_index()->vX_tblCol_To_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  i8 nChosenIndexes = 0;
  for (Kid xKid(this); xKid.bEach(CK_INDEX); /**/) {
    pCI->G.p_index = xKid.p_index();
    if (pCI->G.p_index->pSymbol()->Y_bHas(P_ChosenIndex))
      nChosenIndexes++;
  }
  RASSERT(nChosenIndexes <= 1);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
