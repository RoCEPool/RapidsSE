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
i8 Card_Index::nCullNonusedBounds(CardIter* pCI) {

  _static_

//  UNUSED(pCI);
//
//  return 0;
//
//  // todo: ...

  i8 nCulled = 0;

  //pCI->vSave();

  //pCI->vChangeCard(pCI->G.p_indexList);

  Kid xKid(pCI->G.p_indexList);
  while (xKid.bEach(CK_INDEX)) {

    Card_Bounds* p_bounds = xKid.p_index()->pSub_bounds();
    if (! p_bounds)
      continue;

    Kid xCI_bounds(p_bounds);
    while (xCI_bounds.bEach(CK_BOUND)) {
      if (xCI_bounds.pChild()->pSymbol()->Y_bHas(P_ChosenBound)) {
        continue;
      }

      xCI_bounds.pChild()->vMark1(MARK_AT);
    }

   // pCI->vPrintBoard(__func__,__LINE__);
    nCulled += p_bounds->nCullBound(MARK_AT);

  }

  //pCI->vRestore();

  return nCulled;
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
  p_index->vAppendChild(p_indexNm);

  if (pIndexDef->bIsUnique()) {
    Card_Unique* p_unique = new Card_Unique;
    p_index->vAppendChild(p_unique);
  }

  Card_IdxColList* p_idxColList = new Card_IdxColList;
  p_index->vAppendChild(p_idxColList);

  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    Card_IdxCol* p_idxCol = new Card_IdxCol;
    p_idxCol->SYM(SF_NMD_COL, pColumn->szColNm());
    p_idxCol->pSymbol()->vRotateFacet(SF_NMD_COL);

    p_idxColList->vAppendChild(p_idxCol);

//    Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
//    p_idxColNm->vCastFacets(pColumn->szColNm());
//    p_idxCol->vAppendChild(p_idxColNm);
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
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Index::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Bounds* p_bounds = pSub_bounds())
    p_bounds->vX_tblCol_To_expCol(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vDeleteBounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vCut(P_ChosenIndex);
  if (Card_Bounds* p_Bounds = pSub_bounds())
    vDeleteChild(p_Bounds);
}

/*----------------------------------------------------------------------------*/
/* does this index have a bound logically equivalent to given compare?        */
/*----------------------------------------------------------------------------*/
bool Card_Index::bHasEquivalentBound(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSub_bounds())
    if (pSub_bounds()->bHasEquivalentBound(p_rlnConj))
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
bool Card_Index::bTblCol_IsKeyIn_index(Stz* szTblCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(Symbol::bIs_Tbl_Col(szTblCol));

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  Kid xKid(p_idxColList);
  while (xKid.bEach(CK_IDXCOL)) {

    // ? does n]idxCol:$c.n match given:$c.n
    if (xKid.pChild()->pSymbol()->bHasFacet(szTblCol))
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

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_TBL_NM));
  if (! pTable)
    return nullptr;

  Index* pIndex = pTable->pFindIndex(pSub_indexNm()->szFacet(SF_IDX_NM));
  if (! pIndex) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("index: %s"_Stz, pSub_indexNm()->szFacet(SF_IDX_NM));
    Error xError(EC_NO_SUCH_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vEnsure_Cast(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_bounds = pSub_bounds()))
    pCI->G.p_bounds->vEnsure_Cast(pCI);
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
