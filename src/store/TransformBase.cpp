/*============================================================================*/
/* TransformBase.cpp --                                                       */
/*============================================================================*/

#include "TransformBase.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TransformBase::TransformBase(eCN eClass)
: Object(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TransformBase::~TransformBase(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* access table                                                               */
/*----------------------------------------------------------------------------*/
Table* TransformBase::A_pAccess_Table(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Table* pTable = AM::pTableFind(szTableNm);

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* produce composite <indexList> of table                                     */
/*----------------------------------------------------------------------------*/
Card_IndexList* TransformBase::A_pProduce_indexList_ofTable(Table* pTable)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_Index*  p_index = new Card_Index;
    p_indexList->vAppend(p_index);

    Card_IndexNm* p_indexNm = new Card_IndexNm;
    p_indexNm->vValue(pIndex->szIndexNm());
    p_index->vAppend(p_indexNm);

    Card_IdxColList* p_idxColList = new Card_IdxColList;
    p_index->vAppend(p_idxColList);

    IndexDef* pIndexDef = pIndex->pIndexDef();

    i8 nColumns = pIndexDef->nColumns();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      Column* pColumn = pIndexDef->pGetColumn(i_Col);
      Card_IdxCol* p_idxCol = new Card_IdxCol;
      p_idxCol->vValue(pColumn->szColNm());

      p_idxColList->vAppend(p_idxCol);

      Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
      p_idxColNm->vValue(pColumn->szColNm());
      p_idxCol->vAppend(p_idxColNm);
    }
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* produce <size> of table                                                    */
/*----------------------------------------------------------------------------*/
Card_Size* TransformBase::A_pProduce_size_ofTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (! pTable) {
    vTrace("A: m_pTable not set\n"_Stz);
    return nullptr;
  }

  i8 jTableSize = pTable->pStreamDef()->jTableSize();

  Card_Size* p_size = new Card_Size;
  s32 sSize(jTableSize);
  p_size->vValue(sSize);

  return p_size;
}


/*----------------------------------------------------------------------------*/
/* produce composite <stream> of table                                        */
/*----------------------------------------------------------------------------*/
Card_Stream* TransformBase::A_pProduce_stream_ofTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (!pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  Card_Stream* p_stream = new Card_Stream;
  Card_StmColList* p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmCol->vValue(pColumn->szColNm());
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    p_stmColNm->vValue(pColumn->szColNm());
    p_stmCol->vAppend(p_stmColNm);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_stmCol->vAppend(pCard_Type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce <tablePtn> of table                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn* TransformBase::A_pProduce_tablePtn_ofTable(Table* pTable, char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pPtnColumn = pPtnDef->pGetColumn(0);

  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pStmColumn = pStreamDef->pFindColumn(pPtnColumn->szColNm());
  i8 iStmColNo = pStmColumn->iColumnNo();
  s32 sTablePtn;
  sTablePtn.vSprint("$$%c.%lld"_Stz, cTableSyn, iStmColNo + 1);

  Card_TablePtn* p_tablePtn = new Card_TablePtn;
  p_tablePtn->vValue(sTablePtn);

  return p_tablePtn;
}

/*----------------------------------------------------------------------------*/
/*A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A */
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/*T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0];
  char cSecond = szColumnRef->azChar()[1];
  char cThird  = szColumnRef->azChar()[2];
  char cFourth = szColumnRef->azChar()[3];

  if ('$' == cFirst && '$' == cSecond && '.' == cFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 TransformBase::T_iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szColumnRef = p_columnRef->sz();

  char* azColumnOrd = (szColumnRef->azChar() + 3);

  i8 iColumnOrd = StrLib::iGleanInt(azColumnOrd);

  return iColumnOrd;
}

/*----------------------------------------------------------------------------*/
/* get table syn of column ref                                                */
/*----------------------------------------------------------------------------*/
char TransformBase::T_cColumnRef_TableSyn(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char TransformBase::T_cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformBase::T_vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  psTableSyn->vSprint("%c"_Stz, T_cColumnRef_TableSyn_x(p_columnRef));
}


/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB TransformBase::T_eTerm_Bound(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eRO eRelOp = this->T_eTerm_RelOp(p_term);

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected T_eTerm_RelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value  (i.e. $In)                                        */
/*----------------------------------------------------------------------------*/
i8 TransformBase::T_iTerm_IndexColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(T_bTerm_IsIndexColOrdinal(p_term));

  i8 i_Ord = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bTerm_IsIndexColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a parameter ordinal?  (i.e. $Pn)                                   */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bTerm_IsParamOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('P' == acSecond || 'p' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a stream-column ordinal?  (i.e. $n)                                */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bTerm_IsStreamColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an table-column ordinal?  (i.e. $$c.n)                             */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bTerm_IsTableColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];
  char acFourth = szTerm->azChar()[3];
  char acFifth  = szTerm->azChar()[4];

  if ('$' == acFirst && '$' == acSecond
      && StrLib::bIsAlpha(acThird)
      && '.' == acFourth
      && StrLib::bIsDigit(acFifth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO TransformBase::T_eTerm_RelOp(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->sz();

  if (szTerm->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get stream column number n  (i.e. $n)                                      */
/*----------------------------------------------------------------------------*/
i8 TransformBase::T_iTerm_StreamColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(T_bTerm_IsStreamColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table column number n (i.e. $$c.n)                                     */
/*----------------------------------------------------------------------------*/
i8 TransformBase::T_iTerm_TableColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(T_bTerm_IsTableColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char TransformBase::T_cTerm_TableSyn(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(T_bTerm_IsTableColOrdinal(p_term));

  Stz* szTerm = p_term->sz();

  char cSyn = szTerm->azChar()[2];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE TransformBase::T_eTerm_TermEval(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->sz();

  if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("exists"_Stz))  return TE_EXISTS;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;

  else if (szTerm->bSame("<"_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))       return TE_COMPARE;

  else return TE_OTHER;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TransformBase::T_vPRED_CullByKids(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildCards = p_PRED->nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = p_PRED->pGet(i_ChildCard);
    if (!T_bPRED_IsPred(pCard_Child))
      continue;

    Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_Child;

    T_vPRED_CullByKids(pCard_ChildPRED);

    if (0 == pCard_ChildPRED->nItems()) {

      p_PRED->vRemove(pCard_ChildPRED);
      delete pCard_ChildPRED;

      i_ChildCard--;
      nChildCards--;

      continue;
    }

    if (1 == pCard_ChildPRED->nItems()) {

      Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_Child->pPop();

      p_PRED->vReplace(pCard_ChildPRED,pCard_KidPRED);
      delete pCard_ChildPRED;

      continue;
    }


  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the T_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* TransformBase::T_pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex,
                                        ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return T_pOrPred_FindBound(_pCard_OrPred(p_PRED), pIndex, eBound);

    case CK_ANDPRED:
      return T_pAndPred_FindBound(_pCard_AndPred(p_PRED), pIndex, eBound);

    case CK_CMPPRED:
      return T_pCmpPred_FindBound(_pCard_CmpPred(p_PRED), pIndex, eBound);

    default:
      vTrace("T_pPRED_FindBound: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bPRED_HasBounds(Card_PRED* p_PRED, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return T_bOrPred_HasBounds(_pCard_OrPred(p_PRED), pIndex);

    case CK_ANDPRED:
      return T_bAndPred_HasBounds(_pCard_AndPred(p_PRED), pIndex);

    case CK_CMPPRED:
      return T_bCmpPred_HasBounds(_pCard_CmpPred(p_PRED), pIndex);

    default:
      vTrace("T_bPRED_HasBounds: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bPRED_IsPred(Card* pArgCard)
{
  _static_;

  eCK eCard = pArgCard->eCard();

  switch (eCard) {
    case CK_EXPRBOOL:
    case CK_ORPRED:
    case CK_ANDPRED:
      return true;

    default: return false;
  }
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the T_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* TransformBase::T_pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (T_bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (T_bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the T_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* TransformBase::T_pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (T_bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (T_bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_this_0 =
      _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  char cTableSyn_this_0 = szColumnRef_this_0->cTableSyn();

  Card_ColumnRef* p_columnRef_this_1 =
      _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();
  char cTableSyn_this_1 = szColumnRef_this_1->cTableSyn();


  Card_ColumnRef* p_columnRef_arg_0 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  char cTableSyn_arg_0 = szColumnRef_arg_0->cTableSyn();

  Card_ColumnRef* p_columnRef_arg_1 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();
  char cTableSyn_arg_1 = szColumnRef_arg_1->cTableSyn();

  bool bAreCoPtn = false;

  if (   cTableSyn_arg_0 == cTableSyn_this_0
         || cTableSyn_arg_0 == cTableSyn_this_1
         || cTableSyn_arg_1 == cTableSyn_this_0
         || cTableSyn_arg_1 == cTableSyn_this_1
      )
    bAreCoPtn = true;

  vTrace("(%c,%c) - (%c,%c) are%s co-partitioned\n"_Stz,
         cTableSyn_this_0,cTableSyn_this_1,
         cTableSyn_arg_0,cTableSyn_arg_1,
         bAreCoPtn ? "" : " NOT");

  return bAreCoPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();

  if (szColumnRef_this_0->bSame(szColumnRef_arg_0) &&
      szColumnRef_this_1->bSame(szColumnRef_arg_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //this->vDump();
  //p_PredCmp_arg->vDump();

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = T_cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = T_cColumnRef_TableSyn(p_columnRef_this_1);

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = T_cColumnRef_TableSyn(p_columnRef_arg_0);
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = T_cColumnRef_TableSyn(p_columnRef_arg_1);

  if (   cTableSyn_this_0 == cTableSyn_arg_0
         || cTableSyn_this_0 == cTableSyn_arg_1
         || cTableSyn_this_1 == cTableSyn_arg_0
         || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void TransformBase::T_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szCmp = p_cmpPred->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  p_cmpPred->vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the T_eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* TransformBase::T_pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (eBound != T_eTerm_Bound(pTerm2))
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_CmpPred(this);
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (PB_NONE == T_eTerm_Bound(pTerm2))
    return false;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool TransformBase::T_bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = T_cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = T_cColumnRef_TableSyn(p_columnRef_this_1);

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
