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
void Planner::operator delete(void* aAddr)
{
  Planner* pPlanner = reinterpret_cast<Planner*>(aAddr);
  
  Heap::vFree(pPlanner);
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
  m_pWhereTknVec = nullptr;
  
  for (i8 i_Phrase = 0; i_Phrase < C_MAX_PHRASES; i_Phrase++) {
    m_pPhraseTknVec[i_Phrase] = nullptr;
    m_i_Chosen[i_Phrase] = -1;
  }
  m_nPhrases = 0;
  
  _checkInitialised_(CN_PLANNER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Planner::~Planner(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  for (i8 i_Vec = 0; i_Vec < C_MAX_PHRASES; i_Vec++)
    if (m_pPhraseTknVec[i_Vec]) {
      m_pPhraseTknVec[i_Vec]->vSetAllTokens();
      delete m_pPhraseTknVec[i_Vec];
    }
  
  m_pWhereTknVec->vSetAllTokens();
  delete m_pWhereTknVec;
  
  delete m_pPathVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* choose best path based on score                                            */
/*----------------------------------------------------------------------------*/
void Planner::vChoosePath(i8 i_Phrase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);

  ASSERT(0 == i_Phrase);
  
  AccessPath* npChosenAccessPath = nullptr;
  
  // step thru path vector and find first candidate
  for (m_i_Chosen[i_Phrase] = 0;
       m_i_Chosen[i_Phrase] < m_pPathVec->nItems();
       m_i_Chosen[i_Phrase]++) {
    
    AccessPath* mpAccPath = m_pPathVec->pGet(m_i_Chosen[i_Phrase]);
    
    if ( ! mpAccPath->bIsCandidate() )
      continue;

    npChosenAccessPath = mpAccPath;
    break;
  }
  
  ASSERT(npChosenAccessPath);
  
  // step thru remaining paths attempting to find a better candidate
  for (i8 i_Test = m_i_Chosen[i_Phrase] + 1;
       i_Test < m_pPathVec->nItems();
       i_Test++) {
    
    AccessPath* mpTestAccPath = m_pPathVec->pGet(i_Test);
    
    if ( ! mpTestAccPath->bIsCandidate() )
      continue;
    
    if ( mpTestAccPath->iPathScore() < npChosenAccessPath->iPathScore() ) {
      m_i_Chosen[i_Phrase] = i_Test;
      npChosenAccessPath = mpTestAccPath;
    }
  }
  
  //vTrace("Chosen: %lld\n"_Stz,m_i_Chosen);
}

/*----------------------------------------------------------------------------*/
/* get chosen path                                                            */
/*----------------------------------------------------------------------------*/
AccessPath* Planner::pChosenPath(i8 i_Phrase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  ASSERT(0 == i_Phrase);
  
  AccessPath* pChosenPath = m_pPathVec->pGet(m_i_Chosen[i_Phrase]);
  
  return pChosenPath;
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pPathVec = new PathVec;
  
  IdxVec* pIdxVec = m_pTable->pIdxVec();
  
  for (i8 i_Idx = 0; i_Idx < pIdxVec->nItems(); i_Idx++) {
    
    Index* mpIndex = pIdxVec->pGet(i_Idx);
    
    if (mpIndex->sIndexName().bSame("hidden"_Stz)) {
      AccessPath* mpAccessPath = new AccessPath(AT_SCAN);
      mpAccessPath->vIndex(mpIndex);
      mpAccessPath->vIsCandidate(true);
      m_pPathVec->vAdd(mpAccessPath);
      continue;
    }
    
    vGenPaths(mpIndex, AT_EXACT);
    vGenPaths(mpIndex, AT_LOWER);
    vGenPaths(mpIndex, AT_UPPER);
    vGenPaths(mpIndex, AT_RANGE);
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given index and access technique         */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Index* pIndex, eAT eAccess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  //
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  // step thru index column combinations
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    // first, create an access path for this combo
    AccessPath* pAccessPath = new AccessPath(eAccess);
    m_pPathVec->vAdd(pAccessPath);
    
    // set the index in path
    pAccessPath->vIndex(pIndex);
    
    if (AT_EXACT == eAccess || AT_LOWER == eAccess || AT_RANGE == eAccess) {
      
      // create the lower bound predicate vector
      PredicateVec* pLowerPredicateVec = new PredicateVec;
      pAccessPath->vLowerPredicates(pLowerPredicateVec);
      
      // create predicates of the lower bound predicate vector
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
    
    if (AT_UPPER == eAccess || AT_RANGE == eAccess) {
    
      // create the upper bound predicate vector
      PredicateVec* pUpperPredicateVec = new PredicateVec;
      pAccessPath->vUpperPredicates(pUpperPredicateVec);
      
      // create predicates of the upper bound predicate vector
      for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
        
        // create an upper bound predicate
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
/* does given relational op match with given bounds?                          */
/*----------------------------------------------------------------------------*/
bool Planner::bIsMatchingOp(eAT eAccess, eRO eRelOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if ( AT_EXACT == eAccess ) {
    if ( bIsExactOp(eRelOp) )
      return true;
  }
  else if ( AT_LOWER == eAccess ) {
    if ( bIsLowerOp(eRelOp) )
      return true;
  }
  else {
    ASSERT( AT_UPPER == eAccess );
    if ( bIsUpperOp(eRelOp) )
      return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is token a relational op?                                                  */
/*----------------------------------------------------------------------------*/
bool Planner::bIsRelationalOp(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  if (pToken->bSame("<"_Stz)  ||
      pToken->bSame("<="_Stz) ||
      pToken->bSame(">="_Stz) ||
      pToken->bSame(">"_Stz)  )
    return true;
  
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
      PredicateVec* npLwrPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npLwrPredVec,AT_EXACT);
      
      if (npLwrPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_LOWER == eAccess) {
      PredicateVec* npLwrPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npLwrPredVec,AT_LOWER);
      
      if (npLwrPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_UPPER == eAccess) {
      PredicateVec* npUprPredVec = mpAccPath->pUpperPredicates();
      vMatchPredicates(npUprPredVec,AT_UPPER);
      
      if (npUprPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else
    if (AT_RANGE == eAccess) {
      PredicateVec* npLwrPredVec = mpAccPath->pLowerPredicates();
      vMatchPredicates(npLwrPredVec,AT_LOWER);
      
      PredicateVec* npUprPredVec = mpAccPath->pUpperPredicates();
      vMatchPredicates(npUprPredVec,AT_UPPER);
      
      if (npLwrPredVec->bFullyMatched() && npUprPredVec->bFullyMatched())
        mpAccPath->vIsCandidate(true);
    }
    else {
      ASSERT(AT_SCAN == eAccess);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to match path columns to where columns                             */
/*----------------------------------------------------------------------------*/
void Planner::vMatchPredicates(PredicateVec* mpPathPredVec, eAT eAccess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  Token* pColNmTkn_Where;
  Token* mpLitTkn_Where;
  Token* mpOpTkn_Where;
  s256 sOpDatum_Where;
  eRO eRelOp_Where;
  
  // step thru each predicate in path
  for (i8 i_PathPred = 0; i_PathPred < mpPathPredVec->nItems(); i_PathPred++) {

    // get path predicate
    Predicate* mpPathPred = mpPathPredVec->pGet(i_PathPred);
    
    // get the column name from path predicate
    s256 sColumnNm_Path = mpPathPred->sColumnNm();
    
    // search where clause to find predicate with column name and suitable op
    for (i8 i_ColumnTkn_Where = -1;;) {
      
      // get next columnNm token from where clause
      pColNmTkn_Where =
        m_pPhraseTknVec[0]->pFindToken(sColumnNm_Path,i_ColumnTkn_Where);
      
      // unable to find a matching column
      if ( ! pColNmTkn_Where )
        return;
      
      // get the associated literal token
      mpLitTkn_Where = m_pPhraseTknVec[0]->pGet(i_ColumnTkn_Where+1);
      
      // get the associated op token
      mpOpTkn_Where = m_pPhraseTknVec[0]->pGet(i_ColumnTkn_Where+2);
      sOpDatum_Where = mpOpTkn_Where->aText();
      eRelOp_Where = Op::eDecode(sOpDatum_Where.aStr());
      
      // have we found a bound where predicate?
      if (bIsMatchingOp(eAccess,eRelOp_Where))
        break;
      
      // the where column predicate does not have a matching op, try again
    }
    
    // we have found a bound where predicate
    
    // set the access path bound predicate value
    s256 sLitDatum_Where = mpLitTkn_Where->aText();
    mpPathPred->vNewLitArg(sLitDatum_Where);
    mpPathPred->vNewRelOp(eRelOp_Where);
    
    mpPathPredVec->vSetMatched(i_PathPred);
  }
}

/*----------------------------------------------------------------------------*/
/* parse phrases                                                              */
/*----------------------------------------------------------------------------*/
void Planner::vParsePhrases(s48K& sWhereClause)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  // for the moment just make the phrase vector a copy of the where vector
  m_pPhraseTknVec[0] = new TokenVec;
  for (i8 i_Token = 0; i_Token < m_pWhereTknVec->nTokens(); i_Token++)
    m_pPhraseTknVec[0]->vAddToken(m_pWhereTknVec->pGet(i_Token)->aText());
  
  m_nPhrases = 1;
  
  vShowPhrases();
}
//void Planner::vParsePhrases(s48K& sWhereClause)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
//  
//  TokenVec* pWorkTknVec1 = new TokenVec;
//  TokenVec* pWorkTknVec2 = new TokenVec;
//  
//  bool bCopy1 = true;
//  for (i8 i_Token = 0; i_Token < m_pWhereTknVec->nTokens(); i_Token++) {
//    
//    Token* pToken = m_pWhereTknVec->pGet(i_Token);
//    
//    if (bCopy1)
//      pWorkTknVec1->vAddToken(pToken->aText());
//    else
//      pWorkTknVec2->vAddToken(pToken->aText());
//
//    if (bIsRelationalOp(pToken)) {
//      bCopy1 = false;
//      continue;
//    }
//    
//    if (pToken->bSame("and"_Stz)) {
//      
//    }
////
////    if (pToken->bSame("or"_Stz))
////      bCopy1 = false;
//  }
//  
//  delete pWorkTknVec1;
//  delete pWorkTknVec2;
//
//  vShowPhrases();
//}

/*----------------------------------------------------------------------------*/
/* parse where clause                                                         */
/*----------------------------------------------------------------------------*/
void Planner::vParseWhere(s48K& sWhereClause)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  m_pWhereTknVec = Tokeniser::pTokenise(sWhereClause);
  
}

/*----------------------------------------------------------------------------*/
/* get the number of phrases                                                  */
/*----------------------------------------------------------------------------*/
i8 Planner::nPhrases(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  return m_nPhrases;
}

/*----------------------------------------------------------------------------*/
/* score the set of all paths (lower is better)                               */
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
    switch (pAccessPath->eAccess()) {
    
      case AT_EXACT:
        iScore = 100 + (pIndex->bIsUnique() ? 0 : 50)
                     + pAccessPath->pLowerPredicates()->nItems();
        break;
        
      case AT_RANGE:
        iScore = 200 + (pIndex->bIsUnique() ? 0 : 50)
                     + pAccessPath->pLowerPredicates()->nItems()
                     + pAccessPath->pUpperPredicates()->nItems();
        break;
        
      case AT_LOWER:
        iScore = 300 + (pIndex->bIsUnique() ? 0 : 50)
                     + pAccessPath->pLowerPredicates()->nItems();
        break;
        
      case AT_UPPER:
        iScore = 300 + (pIndex->bIsUnique() ? 0 : 50)
                     + pAccessPath->pUpperPredicates()->nItems();
        break;
        
      default:
        ASSERT(AT_SCAN == pAccessPath->eAccess());
        iScore = 400;
    }
    
    pAccessPath->vPathScore(iScore);
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
/* show all phrases extracted from the where clause                           */
/*----------------------------------------------------------------------------*/
void Planner::vShowPhrases(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  vTrace("-- Phrases -------------------\n"_Stz);
  for (i8 i_Phrase = 0; i_Phrase < m_nPhrases; i_Phrase++) {
    vTrace("[%lld] "_Stz,i_Phrase);
    for (i8 i_Token = 0;
         i_Token < m_pPhraseTknVec[i_Phrase]->nTokens();
         i_Token++) {
      Token* pToken = m_pPhraseTknVec[i_Phrase]->pGet(i_Token);
      vTrace("%s "_Stz,pToken->aText());
    }
    vTrace("\n"_Stz);
  }
  vTrace("------------------------------\n"_Stz);
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
