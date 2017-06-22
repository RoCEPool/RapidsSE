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
  mpChosenPath->vAccess(AT_LOWER);
  
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
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  //
  // create a possible path for every index column combination
  // covering lower bound, upper bound, and exact match
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // upper bound
  
  // tbd
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // range bound
  
  // tbd
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
    vGenPaths(mpTable, mpIndex);
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
  
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    AccessPath* mpAccPath = m_pPathVec->pGet(i_Path);
   
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lower bound
    
    PredicateVec* mpLowPredVec = mpAccPath->pLowerPredicates();
    
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpAccPath->pLowerPredicates()->nItems(); i_Col++) {
      
      Predicate* mpLowColPred = mpLowPredVec->pGet(i_Col);
      
      s32 sColumnName = mpLowColPred->pArg1()->sArgVal().aStr();
      
      i8 i_Token = 0;
      Token* pColNmTkn = m_pWhereTokenVec->pFindToken(sColumnName,i_Token);
      if ( ! pColNmTkn ) {
        bMatchAll = false;
        break;
      }
    
      
      Token* mpLowValTkn = m_pWhereTokenVec->pGet(i_Token+1);
      
      Arg* pLitValue = new Arg;
      s256 sLitValue = mpLowValTkn->aText();
      pLitValue->vArgVal(sLitValue);
      mpLowColPred->vArg2(pLitValue);

    } // i_Col
    
    if ( bMatchAll )
      mpAccPath->vIsCandidate(true);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // upper bound
    
    // tbd
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // range bound
    
    // tbd
    
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
  
  // step thru paths and score them
  for (i8 i_Path = 0; i_Path < m_pPathVec->nItems(); i_Path++) {
    
    // get access path
    AccessPath* pAccessPath = m_pPathVec->pGet(i_Path);
    
    // get index of access path
    Index* pIndex = pAccessPath->pIndex();
    
    // initially just a simple scoring method      //! move this into AccessPath
    i8 iScore = 10 * (pIndex->bIsUnique() ? 0 : 1)
                  + pAccessPath->pLowerPredicates()->nItems();
    
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
