/*============================================================================*/
/* Card_Index.cpp -- Index                                                    */
/*============================================================================*/

#include "Card_Index.hpp"

#include "Table.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"
#include "IndexDef.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Index,CK_INDEX)
__aNewFunction(Card_Index,CN_CARD)
__vDeleteFunction(Card_Index,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Index::Card_Index(void)
: Card_symbol(CK_INDEX, 4)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Index::~Card_Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bound                                                        */
/*----------------------------------------------------------------------------*/
void Card_Index::vCullNonusedBounds(CardIter* pCI) {

  _static_

  pCI->vSave();

  pCI->vChangeCard(pCI->G.p_indexList);

  while (pCI->G_bEachMain(CK_INDEX)) {

    pCI->G.p_bounds = pCI->G.p_index->pSub_bounds();
    if (! pCI->G.p_bounds)
      continue;

    Kid xCI_bounds(pCI->G.p_bounds);
    while (xCI_bounds.bEach(CK_BOUND)) {
      if (xCI_bounds.pChild()->pSymbol()->Y_bHas(P_ChosenBound)) {
        continue;
      }

      xCI_bounds.pChild()->vMark1(MARK_AT_STZ);
    }

    pCI->vPrintBoard(__LINE__);
    pCI->G.p_bounds->vCullBound(MARK_AT);

  }

  pCI->vRestore();

}

/*----------------------------------------------------------------------------*/
/* produce composite <index> of table                                         */
/*----------------------------------------------------------------------------*/
Card_Index* Card_Index::pDerive(Index* pIndex)
{
  _static_

  Card_Index* p_index = new Card_Index;
  IndexDef* pIndexDef = pIndex->pIndexDef();

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->SYM(SF_IDX_NM,pIndex->szIndexNm());
  p_index->vAppend(p_indexNm);

  if (pIndexDef->bIsUnique()) {
    Card_Unique* p_unique = new Card_Unique;
    p_index->vAppend(p_unique);
  }

  Card_IdxColList* p_idxColList = new Card_IdxColList;
  p_index->vAppend(p_idxColList);

  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    Card_IdxCol* p_idxCol = new Card_IdxCol;
    p_idxCol->SYM(SF_COL_NM, pColumn->szColNm());

    p_idxColList->vAppend(p_idxCol);

//    Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
//    p_idxColNm->vSymbol(pColumn->szColNm());
//    p_idxCol->vAppend(p_idxColNm);
  }

  return p_index;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vAdd_streamKey_exprs(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_idxColList = pSub_idxColList();

  pCI->G.p_idxColList->vAdd_streamKey_exprs(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vDeleteBounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vCut(P_ChosenIndex);
  Card_Bounds* p_Bounds = pSub_bounds();
  vDeleteChild(p_Bounds);
}

/*----------------------------------------------------------------------------*/
/* does this index have a bound logically equivalent to given compare?        */
/*----------------------------------------------------------------------------*/
bool Card_Index::bHasEquivalentBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSub_bounds())
    if (pSub_bounds()->bHasEquivalentBound(p_cmpPred))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Index::bUsesSqlRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Bounds* p_bounds = pSub_bounds())
    return p_bounds->bUsesSqyRsl();

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vSet_Choose_index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  this->pSymbol()->Y_vSet(P_ChosenIndex);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Index::bTblColOdn_IsKeyIn_index(Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  Kid xCI(p_idxColList);
  while (xCI.bEach(CK_IDXCOL)) {

    // ? does n]idxCol:$c.n match given:$c.n
    if (xCI.pChild()->pSymbol()->bSame(szTblColOdn))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Index* Card_Index::pGetIndex(Card_TableNm* p_tableNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szSymbol(SF_NONE));
  if (! pTable)
    return nullptr;

  Index* pIndex = pTable->pFindIndex(pSub_indexNm()->szSymbol(SF_NONE));
  if (! pIndex) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("index: %s"_Stz,pSub_indexNm()->szSymbol(SF_NONE));
    Error xError(EC_NO_SUCH_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vEnsureTypes_For_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_bounds = pSub_bounds()))
    pCI->G.p_bounds->vEnsureTypes_For_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_INDEXNM,CK_NONE);

  if (pSymbol()->Y_bHas(P_ChosenIndex)) {
    RASSERT(pFindPpyRecursive(P_ChosenBound, CK_BOUND));
  }
  else {
    RASSERT(! pFindPpyRecursive(P_ChosenBound, CK_BOUND));
  }

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
