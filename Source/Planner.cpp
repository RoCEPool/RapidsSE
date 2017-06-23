/*============================================================================*/
/* Planner.cpp - Planner                                                      */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Planner* Plans::_pPlanner(void* aAddr)
{
  Planner* pPlanner = reinterpret_cast<Planner*>(aAddr);
  
  RASSERT( ! pPlanner || CN_PLANNER == pPlanner->eClass() );
 
  return pPlanner;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Planner::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_PLANNER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Planner::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Planner::Planner(void)
: Object(CN_PLANNER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pTable = nullptr;
  m_pPathVec = nullptr;
  
  for (i8 i_Cdt = 0; i_Cdt < C_MAX_CANDIDATES; i_Cdt++) {
    m_pIndex[i_Cdt] = nullptr;
    m_iScoreVec[i_Cdt] = 0;
    m_bIsCandidate[i_Cdt] = false;
  }
  m_nCandidates = 0;
  //m_sWhere
  m_pPredicateVec = nullptr;
  m_i_Chosen = -1;
  
  m_pWhereTokenVec = nullptr;
  
  _checkInitialised_(CN_PLANNER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Planner::~Planner(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* choose best path based on score                                            */
/*----------------------------------------------------------------------------*/
void Planner::vChoosePath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  

  AccessPath* npChosenAccessPath = nullptr;
  
  // step thru path vector and find first candidate               //! assumption
  for (m_i_Chosen = 0; m_i_Chosen < m_pPathVec->nItems(); m_i_Chosen++) {
    
    AccessPath* mpAccPath = m_pPathVec->pGet(m_i_Chosen);
    
    if (mpAccPath->bIsCandidate()) {
      npChosenAccessPath = mpAccPath;
      break;
    }
  }
  
  ASSERT(npChosenAccessPath);
  
  // step thru remaining paths attempting to find a better candidate
  for (i8 i_Test = m_i_Chosen + 1; i_Test < m_pPathVec->nItems(); i_Test++) {
    
    AccessPath* mpTestAccPath = m_pPathVec->pGet(i_Test);
    
    if (mpTestAccPath->bIsCandidate() &&
        mpTestAccPath->iPathRating() < npChosenAccessPath->iPathRating()) {
      m_i_Chosen = i_Test;
      npChosenAccessPath = mpTestAccPath;
    }
  }
  
  AccessPath* mpChosenPath = m_pPathVec->pGet(m_i_Chosen);
  
  vTrace("Chosen: %lld\n"_Str,m_i_Chosen);
}

/*----------------------------------------------------------------------------*/
/* get chosen path                                                            */
/*----------------------------------------------------------------------------*/
AccessPath* Planner::pChosenPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  AccessPath* pChosenPath = m_pPathVec->pGet(m_i_Chosen);
  
  return pChosenPath;
  
}

/*----------------------------------------------------------------------------*/
/* generate the set of all exact paths for given table                        */
/*----------------------------------------------------------------------------*/
void Planner::vGenExactPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  // for exact bound
  //
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // exact bound
  
  // step thru index column combinations for exact bound
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    // create an access path for this combo
    AccessPath* pAccessPath = new AccessPath(AT_EXACT);
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    // create the lower bound predicate vector
    PredicateVec* pExactPredicateVec = new PredicateVec;
    pAccessPath->vLowerPredicates(pExactPredicateVec);
    
    // step thru predicates of the lower bound predicate vector
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      
      // create a lower bound predicate
      Predicate* pExactPredicate = new Predicate;
      pExactPredicateVec->vAdd(pExactPredicate);
      
      // add column arg to predicate
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      Arg* pColumnArg = new Arg;
      s256 sColumnNm = pColumn->sColumnName().aStr();
      pColumnArg->vArgVal(sColumnNm);
      pExactPredicate->vArg1(pColumnArg);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all lower paths for given table                        */
/*----------------------------------------------------------------------------*/
void Planner::vGenLowerPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  // for lower bound
  //
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lower bound
  
  // step thru index column combinations for lower bound
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    // create an access path for this combo
    AccessPath* pAccessPath = new AccessPath(AT_LOWER);
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    // create the lower bound predicate vector
    PredicateVec* pLowerPredicateVec = new PredicateVec;
    pAccessPath->vLowerPredicates(pLowerPredicateVec);
    
    // step thru predicates of the lower bound predicate vector
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      
      // create a lower bound predicate
      Predicate* pLowerPredicate = new Predicate;
      pLowerPredicateVec->vAdd(pLowerPredicate);
      
      // add column arg to predicate
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      Arg* pColumnArg = new Arg;
      s256 sColumnNm = pColumn->sColumnName().aStr();
      pColumnArg->vArgVal(sColumnNm);
      pLowerPredicate->vArg1(pColumnArg);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all exact paths for given table                        */
/*----------------------------------------------------------------------------*/
void Planner::vGenUpperPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  // for upper bound
  //
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // upper bound
  
  // step thru index column combinations for upper bound
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    // create an access path for this combo
    AccessPath* pAccessPath = new AccessPath(AT_UPPER);
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    // create the upper bound predicate vector
    PredicateVec* pUpperPredicateVec = new PredicateVec;
    pAccessPath->vUpperPredicates(pUpperPredicateVec);
    
    // step thru predicates of the upper bound predicate vector
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      
      // create a upper bound predicate
      Predicate* pUpperPredicate = new Predicate;
      pUpperPredicateVec->vAdd(pUpperPredicate);
      
      // add column arg to predicate
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      Arg* pColumnArg = new Arg;
      s256 sColumnNm = pColumn->sColumnName().aStr();
      pColumnArg->vArgVal(sColumnNm);
      pUpperPredicate->vArg1(pColumnArg);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all range paths for given table                        */
/*----------------------------------------------------------------------------*/
void Planner::vGenRangePaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  // for range bound
  //
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // range bound
  
  // step thru index column combinations for range bound
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    // create an access path for this combo
    AccessPath* pAccessPath = new AccessPath(AT_RANGE);
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    // create the lower bound predicate vector
    PredicateVec* pLowerPredicateVec = new PredicateVec;
    pAccessPath->vLowerPredicates(pLowerPredicateVec);
    
    // step thru predicates of the lower bound predicate vector
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      
      // create a lower bound predicate
      Predicate* pLowerPredicate = new Predicate;
      pLowerPredicateVec->vAdd(pLowerPredicate);
      
      // add column arg to predicate
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      Arg* pColumnArg = new Arg;
      s256 sColumnNm = pColumn->sColumnName().aStr();
      pColumnArg->vArgVal(sColumnNm);
      pLowerPredicate->vArg1(pColumnArg);
    }
    
    // create the upper bound predicate vector
    PredicateVec* pUpperPredicateVec = new PredicateVec;
    pAccessPath->vUpperPredicates(pUpperPredicateVec);
    
    // step thru predicates of the upper bound predicate vector
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      
      // create a upper bound predicate
      Predicate* pUpperPredicate = new Predicate;
      pUpperPredicateVec->vAdd(pUpperPredicate);
      
      // add column arg to predicate
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      Arg* pColumnArg = new Arg;
      s256 sColumnNm = pColumn->sColumnName().aStr();
      pColumnArg->vArgVal(sColumnNm);
      pUpperPredicate->vArg1(pColumnArg);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* mpTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pPathVec = new PathVec;
  
  IdxVec* pIdxVec = mpTable->pIdxVec();
  
  for (i8 i_Idx = 0; i_Idx < pIdxVec->nItems(); i_Idx++) {
    Index* mpIndex = pIdxVec->pGet(i_Idx);
    m_pIndex[i_Idx] = mpIndex;
    vGenExactPaths(mpTable, mpIndex);
    vGenLowerPaths(mpTable, mpIndex);
    vGenUpperPaths(mpTable, mpIndex);
    //vGenRangePaths(mpTable, mpIndex);
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  vGenPaths(m_pTable);
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to paths                                       */
/*----------------------------------------------------------------------------*/
void Planner::vMatchPaths(void)
{
  //vMatchExactPaths();
  vMatchGivenPaths(BK_EXACT);
  //vMatchLowerPaths();
  vMatchGivenPaths(BK_LOWER);
  //vMatchUpperPaths();
  vMatchGivenPaths(BK_UPPER);
  //vMatchRangePaths();
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to paths                                       */
/*----------------------------------------------------------------------------*/
void Planner::vMatchExactPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* mpEqlValTkn;
  Token* mpOpTkn;
  s256 sOpValue;
  eRO eRelOp;
  i8 i_Token;
  Token* pColNmTkn;
  Predicate* mpEqlColPred;
  s32 sColumnName;
  Arg* pLitValue;
  s256 sLitValue;
  Op* pOp;
  AccessPath* mpAccPath;
  PredicateVec* mpEqlPredVec;
  
  // step thru all generated access paths
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    mpAccPath = m_pPathVec->pGet(i_Path);
    
    // test access path is an exact bound fetch
    if ( AT_EXACT != mpAccPath->eAccess() )
      continue;
    
    // get the exact (lower) bound predicate vector from the access path
    mpEqlPredVec = mpAccPath->pLowerPredicates();
    
    // attempt to find all access path columns
    // in the where clause as exact predicates
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpAccPath->pLowerPredicates()->nItems(); i_Col++) {
      
      // get access path lower predicate for the column
      mpEqlColPred = mpEqlPredVec->pGet(i_Col);
      sColumnName = mpEqlColPred->pArg1()->sArgVal().aStr();
      
      // does this column exist in the where clause as a exact predicate?
      i_Token = -1;
      for (;;) {
        
        // attempt to find the column in the where clause
        pColNmTkn = m_pWhereTokenVec->pFindToken(sColumnName,i_Token);
        if ( ! pColNmTkn ) {
          bMatchAll = false;
          goto L_MatchTest;
        }
        
        // found column in where clause
        
        // get the associated value from the where predicate
        // remember that predicates are in rpn form
        mpEqlValTkn = m_pWhereTokenVec->pGet(i_Token+1);
        
        // get the operator from the where predicate
        mpOpTkn = m_pWhereTokenVec->pGet(i_Token+2);
        sOpValue = mpOpTkn->aText();
        eRelOp = Op::eDecodeText(sOpValue.aStr());
        
        // haver we found a lower where predicate?
        if ( bIsExactOp(eRelOp) )
          break;
      }
      
      // column exists in the where clause as an exact predicate
      
      // set the access path lower predicate value
      pLitValue = new Arg;
      sLitValue = mpEqlValTkn->aText();
      pLitValue->vArgVal(sLitValue);
      mpEqlColPred->vArg2(pLitValue);
      
      // set the access path lower predicate operator
      pOp = new Op;
      pOp->vOpKind(eRelOp);
      mpEqlColPred->vOp(pOp);
      
      // mark the access path predicate as: column literal op
      mpEqlColPred->vPredicate(PK_COLUMN_LITERAL_OP);
      
    } // i_Col
    
  L_MatchTest:
    
    // does this acess path qualify as a candidate lower access path?
    if ( bMatchAll )
      mpAccPath->vIsCandidate(true);
    
  } // i_Path
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to lower access paths                          */
/*----------------------------------------------------------------------------*/
void Planner::vMatchLowerPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* mpLowValTkn;
  Token* mpOpTkn;
  s256 sOpValue;
  eRO eRelOp;
  i8 i_Token;
  Token* pColNmTkn;
  Predicate* mpLowColPred;
  s32 sColumnName;
  Arg* pLitValue;
  s256 sLitValue;
  Op* pOp;
  AccessPath* mpAccPath;
  PredicateVec* mpLowPredVec;
  
  // step thru all generated access paths
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    mpAccPath = m_pPathVec->pGet(i_Path);
    
    // test access path is a lower bound fetch
    if ( AT_LOWER != mpAccPath->eAccess() )
      continue;
    
    // get the low bound predicate vector from the access path
    mpLowPredVec = mpAccPath->pLowerPredicates();
    
    // attempt to find all access path columns
    // in the where clause as lower predicates
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpLowPredVec->nItems(); i_Col++) {
      
      // get access path lower predicate for the column
      mpLowColPred = mpLowPredVec->pGet(i_Col);
      sColumnName = mpLowColPred->pArg1()->sArgVal().aStr();
      
      // does this column exist in the where clause as a lower predicate?
      i_Token = -1;
      for (;;) {
        
        // attempt to find the column in the where clause
        pColNmTkn = m_pWhereTokenVec->pFindToken(sColumnName,i_Token);
        if ( ! pColNmTkn ) {
          bMatchAll = false;
          goto L_MatchTest;
        }
      
        // found column in where clause
        
        // get the associated value from the where predicate
        // remember that predicates are in rpn form
        mpLowValTkn = m_pWhereTokenVec->pGet(i_Token+1);
        
        // get the operator from the where predicate
        mpOpTkn = m_pWhereTokenVec->pGet(i_Token+2);
        sOpValue = mpOpTkn->aText();
        eRelOp = Op::eDecodeText(sOpValue.aStr());
        
        // haver we found a lower where predicate?
        if ( bIsLowerOp(eRelOp) )
          break;
      }
      
      // column exists in the where clause as a lower predicate
      
      // set the access path lower predicate value
      pLitValue = new Arg;
      sLitValue = mpLowValTkn->aText();
      pLitValue->vArgVal(sLitValue);
      mpLowColPred->vArg2(pLitValue);
      
      // set the access path lower predicate operator
      pOp = new Op;
      pOp->vOpKind(eRelOp);
      mpLowColPred->vOp(pOp);
      
      // mark the access path predicate as: column literal op
      mpLowColPred->vPredicate(PK_COLUMN_LITERAL_OP);
    
    } // i_Col
    
  L_MatchTest:
    
    // does this acess path qualify as a candidate lower access path?
    if ( bMatchAll )
      mpAccPath->vIsCandidate(true);
    
  } // i_Path
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to paths                                       */
/*----------------------------------------------------------------------------*/
void Planner::vMatchUpperPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* mpUppValTkn;
  Token* mpOpTkn;
  s256 sOpValue;
  eRO eRelOp;
  i8 i_Token;
  Token* pColNmTkn;
  Predicate* mpUppColPred;
  s32 sColumnName;
  Arg* pLitValue;
  s256 sLitValue;
  Op* pOp;
  AccessPath* mpAccPath;
  PredicateVec* mpUppPredVec;
  
  // step thru all generated access paths
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    mpAccPath = m_pPathVec->pGet(i_Path);
    
    // test access path is an upper bound fetch
    if ( AT_UPPER != mpAccPath->eAccess() )
      continue;
    
    // get the upper bound predicate vector from the access path
    mpUppPredVec = mpAccPath->pUpperPredicates();
    
    // attempt to find all access path columns
    // in the where clause as upper predicates
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpUppPredVec->nItems(); i_Col++) {
      
      // get access path upper predicate for the column
      mpUppColPred = mpUppPredVec->pGet(i_Col);
      sColumnName = mpUppColPred->pArg1()->sArgVal().aStr();
      
      // does this column exist in the where clause as a upper predicate?
      i_Token = -1;
      for (;;) {
        
        // attempt to find the column in the where clause
        pColNmTkn = m_pWhereTokenVec->pFindToken(sColumnName,i_Token);
        if ( ! pColNmTkn ) {
          bMatchAll = false;
          goto L_MatchTest;
        }
        
        // found column in where clause
        
        // get the associated value from the where predicate
        // remember that predicates are in rpn form
        mpUppValTkn = m_pWhereTokenVec->pGet(i_Token+1);
        
        // get the operator from the where predicate
        mpOpTkn = m_pWhereTokenVec->pGet(i_Token+2);
        sOpValue = mpOpTkn->aText();
        eRelOp = Op::eDecodeText(sOpValue.aStr());
        
        // haver we found an upper where predicate?
        if ( bIsUpperOp(eRelOp) )
          break;
      }
      
      // column exists in the where clause as an upper predicate
      
      // set the access path upper predicate value
      pLitValue = new Arg;
      sLitValue = mpUppValTkn->aText();
      pLitValue->vArgVal(sLitValue);
      mpUppColPred->vArg2(pLitValue);
      
      // set the access path upper predicate operator
      pOp = new Op;
      pOp->vOpKind(eRelOp);
      mpUppColPred->vOp(pOp);
      
      // mark the access path predicate as: column literal op
      mpUppColPred->vPredicate(PK_COLUMN_LITERAL_OP);
      
    } // i_Col
    
  L_MatchTest:
    
    // does this acess path qualify as a candidate upper access path?
    if ( bMatchAll )
      mpAccPath->vIsCandidate(true);
    
  } // i_Path
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to paths                                       */
/*----------------------------------------------------------------------------*/
void Planner::vMatchRangePaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* attempt to match predicates to paths                                       */
/*----------------------------------------------------------------------------*/
void Planner::vMatchGivenPaths(eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* mpValTkn;
  Token* mpOpTkn;
  s256 sOpValue;
  eRO eRelOp;
  i8 i_Token;
  Token* pColNmTkn;
  Predicate* mpColPred;
  s32 sColumnName;
  Arg* pLitValue;
  s256 sLitValue;
  Op* pOp;
  AccessPath* mpAccPath;
  PredicateVec* mpPredVec;
  
  // step thru all generated access paths
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    mpAccPath = m_pPathVec->pGet(i_Path);
    
    // test access path is an upper bound fetch
    if ( BK_UPPER == eBound ) {
      if ( AT_UPPER != mpAccPath->eAccess() )
        continue;
    }
    else {
      if ( AT_LOWER != mpAccPath->eAccess() )
        continue;
    }
    
    // get the bound predicate vector from the access path
    if ( BK_UPPER == eBound )
      mpPredVec = mpAccPath->pUpperPredicates();
    else
      mpPredVec = mpAccPath->pLowerPredicates();
    
    // attempt to find all access path columns
    // in the where clause as upper predicates
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpPredVec->nItems(); i_Col++) {
      
      // get access path upper predicate for the column
      mpColPred = mpPredVec->pGet(i_Col);
      sColumnName = mpColPred->pArg1()->sArgVal().aStr();
      
      // does this column exist in the where clause as a upper predicate?
      i_Token = -1;
      for (;;) {
        
        // attempt to find the column in the where clause
        pColNmTkn = m_pWhereTokenVec->pFindToken(sColumnName,i_Token);
        if ( ! pColNmTkn ) {
          bMatchAll = false;
          goto L_MatchTest;
        }
        
        // found column in where clause
        
        // get the associated value from the where predicate
        // remember that predicates are in rpn form
        mpValTkn = m_pWhereTokenVec->pGet(i_Token+1);
        
        // get the operator from the where predicate
        mpOpTkn = m_pWhereTokenVec->pGet(i_Token+2);
        sOpValue = mpOpTkn->aText();
        eRelOp = Op::eDecodeText(sOpValue.aStr());
        
        // haver we found an upper where predicate?
        if ( BK_EXACT == eBound ) {
          if ( bIsExactOp(eRelOp) )
            break;
        }
        else if ( BK_LOWER == eBound ) {
          if ( bIsLowerOp(eRelOp) )
            break;
        }
        else {
          ASSERT( BK_UPPER == eBound );
          if ( bIsUpperOp(eRelOp) )
            break;
        }
      }
      
      // column exists in the where clause as an upper predicate
      
      // set the access path upper predicate value
      pLitValue = new Arg;
      sLitValue = mpValTkn->aText();
      pLitValue->vArgVal(sLitValue);
      mpColPred->vArg2(pLitValue);
      
      // set the access path upper predicate operator
      pOp = new Op;
      pOp->vOpKind(eRelOp);
      mpColPred->vOp(pOp);
      
      // mark the access path predicate as: column literal op
      mpColPred->vPredicate(PK_COLUMN_LITERAL_OP);
      
    } // i_Col
    
  L_MatchTest:
    
    // does this acess path qualify as a candidate upper access path?
    if ( bMatchAll )
      mpAccPath->vIsCandidate(true);
    
  } // i_Path
}

/*----------------------------------------------------------------------------*/
/* parse where clause                                                         */
/*----------------------------------------------------------------------------*/
void Planner::vParseWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pWhereTokenVec = Tokeniser::pTokenise(m_sWhere);
  for (i8 i_Token = 0; i_Token < m_pWhereTokenVec->nTokens(); i_Token++) {
    Token* pToken = m_pWhereTokenVec->pGet(i_Token);
  }
}

/*----------------------------------------------------------------------------*/
/* score the set of all paths                                                 */
/*----------------------------------------------------------------------------*/
void Planner::vScorePaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  i8 iScore;
  // step thru paths and score them
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    AccessPath* pAccessPath = m_pPathVec->pGet(i_Path);
    
    // get index of access path
    Index* pIndex = pAccessPath->pIndex();
    
    // initially just a simple scoring method      //! move this into AccessPath
    if (AT_LOWER == pAccessPath->eAccess())
      iScore = 10 * (pIndex->bIsUnique() ? 0 : 1)
                  + pAccessPath->pLowerPredicates()->nItems();
    else
    if (AT_UPPER == pAccessPath->eAccess())
      iScore = 10 * (pIndex->bIsUnique() ? 0 : 1)
                  + pAccessPath->pUpperPredicates()->nItems();
    else
      iScore = 100;
    
    pAccessPath->vPathRating(iScore);
  }
}

/*----------------------------------------------------------------------------*/
/* show the generated set of paths                                            */
/*----------------------------------------------------------------------------*/
void Planner::vShowPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
 
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    AccessPath* pAccessPath = m_pPathVec->pGet(i_Path);
    pAccessPath->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* show predicate vector                                                      */
/*----------------------------------------------------------------------------*/
void Planner::vShowPredicates(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  ASSERT(m_pPredicateVec);
  for (i8 i_Pred = 0; i_Pred < m_pPredicateVec->nItems(); i_Pred++) {
    Predicate* mpPredicate = m_pPredicateVec->pGet(i_Pred);
    mpPredicate->vShow();
  }
  
//  for (i8 i_Token = 0; i_Token < m_pWhereTokenVec->nItems(); i_Token++) {
//    Token* pToken = m_pWhereTokenVec->pGet(i_Token);
//    vTrace("  %s\n"_Str,pToken->sText().aStr());
//  }
}

/*----------------------------------------------------------------------------*/
/* get current table                                                          */
/*----------------------------------------------------------------------------*/
Table* Planner::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* score the set of all paths                                                 */
/*----------------------------------------------------------------------------*/
void Planner::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get where clause                                                           */
/*----------------------------------------------------------------------------*/
s48K& Planner::sWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  return m_sWhere;
}

/*----------------------------------------------------------------------------*/
/* set where clause                                                           */
/*----------------------------------------------------------------------------*/
void Planner::vWhere(s48K& sWhere)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_sWhere = sWhere;
}

/*----------------------------------------------------------------------------*/
/* get chosen index                                                           */
/*----------------------------------------------------------------------------*/
Index* Planner::pChosenIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Index* pChosen = m_pIndex[m_i_Chosen];
  
  return pChosen;
}

/*----------------------------------------------------------------------------*/
/* get predicate vector                                                       */
/*----------------------------------------------------------------------------*/
PredicateVec* Planner::pPredicateVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  return m_pPredicateVec;
}

/*----------------------------------------------------------------------------*/
/* is given relational op an exact op?                                        */
/*----------------------------------------------------------------------------*/
bool Planner::bIsExactOp(eRO eRelOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if (RO_EQUAL_TO == eRelOp)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is given relational op a lower op?                                         */
/*----------------------------------------------------------------------------*/
bool Planner::bIsLowerOp(eRO eRelOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if (RO_GREATER_EQUAL == eRelOp || RO_GREATER_THAN == eRelOp)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is given relational op an upper op?                                        */
/*----------------------------------------------------------------------------*/
bool Planner::bIsUpperOp(eRO eRelOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if (RO_LESS_THAN == eRelOp || RO_LESS_EQUAL == eRelOp)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
