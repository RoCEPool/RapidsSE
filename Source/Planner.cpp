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
/* generate the set of all range paths for given table                        */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* pTable, Index* pIndex, eBK eBound)
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
    AccessPath* pAccessPath = nullptr;
    if (BK_EXACT == eBound)
      pAccessPath = new AccessPath(AT_EXACT);
    else
    if (BK_LOWER == eBound)
      pAccessPath = new AccessPath(AT_LOWER);
    else
    if (BK_UPPER == eBound)
      pAccessPath = new AccessPath(AT_UPPER);
    else
    if (BK_RANGE == eBound)
      pAccessPath = new AccessPath(AT_RANGE);
    
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    if (BK_EXACT == eBound || BK_LOWER == eBound || BK_RANGE == eBound) {
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
    
    if (BK_UPPER == eBound || BK_RANGE == eBound) {
    
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
    vGenPaths(mpTable, mpIndex, BK_EXACT);
    vGenPaths(mpTable, mpIndex, BK_LOWER);
    vGenPaths(mpTable, mpIndex, BK_UPPER);
    vGenPaths(mpTable, mpIndex, BK_RANGE);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  // step thru all generated access paths
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    AccessPath* mpAccPath = m_pPathVec->pGet(i_Path);
    
    eAT eAccess = mpAccPath->eAccess();
    
    if (AT_EXACT == eAccess) {
      PredicateVec* npUprPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npUprPredVec,BK_EXACT);
      
      if (npUprPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_LOWER == eAccess) {
      PredicateVec* npLwrPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npLwrPredVec,BK_LOWER);
      
      if (npLwrPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_UPPER == eAccess) {
      PredicateVec* npUprPredVec = mpAccPath->pUpperPredicates();
      vMatchPredicates(npUprPredVec,BK_UPPER);
      
      if (npUprPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_RANGE == eAccess) {
      PredicateVec* npLwrPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npLwrPredVec,BK_LOWER);
      
      PredicateVec* npUprPredVec = mpAccPath->pUpperPredicates();
      vMatchPredicates(npUprPredVec,BK_UPPER);
      
      if (npLwrPredVec->bFullyMatched() && npUprPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to match path columns to where columns                             */
/*----------------------------------------------------------------------------*/
void Planner::vMatchPredicates(PredicateVec* mpPathPredVec, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* pColNmTkn_Where;
  Token* mpLitTkn_Where;
  Token* mpOpTkn_Where;
  s256 sOpDatum_Where;
  eRO eRelOp_Where;
  
  // ste thru each predicate in path
  for (i8 i_PathPred = 0; i_PathPred < mpPathPredVec->nItems(); i_PathPred++) {

    // get path predicate
    Predicate* mpPathPred = mpPathPredVec->pGet(i_PathPred);
    
    // get the column name from path predicate
    s256 sColumnNm_Path = mpPathPred->sColumnNm();
    
    // search where clause to find predicate with column name and suitable op
    for (i8 i_ColumnTkn_Where = -1;;) {
      
      // get next columnNm token from where clause
      pColNmTkn_Where =
        m_pWhereTokenVec->pFindToken(sColumnNm_Path,i_ColumnTkn_Where);
      
      // unable to find a matching column
      if ( ! pColNmTkn_Where )
        return;
      
      // get the associated literal token
      mpLitTkn_Where = m_pWhereTokenVec->pGet(i_ColumnTkn_Where+1);
      
      // get the associated op token
      mpOpTkn_Where = m_pWhereTokenVec->pGet(i_ColumnTkn_Where+2);
      sOpDatum_Where = mpOpTkn_Where->aText();
      eRelOp_Where = Op::eDecode(sOpDatum_Where.aStr());
      
      // have we found a bound where predicate?
      if (bIsMatchingOp(eBound,eRelOp_Where))
        break;
      
      // the where column predicate does not have a matching op, try again
    }
    
    // we have found a bound where predicate
    
    // set the access path bound predicate value
    s256 sLtDatum_Where = mpLitTkn_Where->aText();
    mpPathPred->vNewLitArg(sLtDatum_Where);
    mpPathPred->vNewRelOp(eRelOp_Where);
    
    mpPathPredVec->vSetMatched(i_PathPred);
  }
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
    
    // initially just a simple scoring method   
    if (AT_EXACT == pAccessPath->eAccess())
      iScore = 100 + (pIndex->bIsUnique() ? 0 : 50)
                   + pAccessPath->pLowerPredicates()->nItems();
    else
    if (AT_RANGE == pAccessPath->eAccess())
      iScore = 200 + (pIndex->bIsUnique() ? 0 : 50)
                   + pAccessPath->pUpperPredicates()->nItems()
                   + pAccessPath->pUpperPredicates()->nItems();
    else
    if (AT_LOWER == pAccessPath->eAccess())
      iScore = 300 + (pIndex->bIsUnique() ? 0 : 50)
                   + pAccessPath->pLowerPredicates()->nItems();
    else
    if (AT_UPPER == pAccessPath->eAccess())
      iScore = 300 + (pIndex->bIsUnique() ? 0 : 50)
                   + pAccessPath->pUpperPredicates()->nItems();
    else
      iScore = 400;
    
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
/* does given relational op match with given bounds?                          */
/*----------------------------------------------------------------------------*/
bool Planner::bIsMatchingOp(eBK eBound, eRO eRelOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if ( BK_EXACT == eBound ) {
    if ( bIsExactOp(eRelOp) )
      return true;
  }
  else if ( BK_LOWER == eBound ) {
    if ( bIsLowerOp(eRelOp) )
      return true;
  }
  else {
    ASSERT( BK_UPPER == eBound );
    if ( bIsUpperOp(eRelOp) )
      return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
