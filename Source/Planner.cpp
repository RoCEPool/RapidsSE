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
  m_pCandidatePaths = nullptr;
  for (i8 i_Cdt = 0; i_Cdt < C_MAX_CANDIDATES; i_Cdt++) {
    m_pIndex[i_Cdt] = nullptr;
    m_iScoreVec[i_Cdt] = 0;
    m_bIsCandidate[i_Cdt] = false;
  }
  m_nCandidates = 0;
  //m_sWhere
  m_pPredicateVec = nullptr;
  m_i_Chosen = -1;
  
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
  
  ASSERT(m_pCandidatePaths->nItems());
  
  CandidatePath* npChosenPath = nullptr;
  
  for (m_i_Chosen = 0; m_i_Chosen < m_pCandidatePaths->nItems(); m_i_Chosen++) {
    if (m_bIsCandidate[m_i_Chosen]) {
      npChosenPath = m_pCandidatePaths->pGet(m_i_Chosen);
      break;
    }
  }
  
  for (i8 i_Test = m_i_Chosen + 1; i_Test < m_pCandidatePaths->nItems(); i_Test++) {
    CandidatePath* npTestPath = m_pCandidatePaths->pGet(i_Test);
    if (m_bIsCandidate[i_Test] && m_iScoreVec[i_Test] < m_iScoreVec[m_i_Chosen]) {
      m_i_Chosen = i_Test;
      npChosenPath = m_pCandidatePaths->pGet(m_i_Chosen);
    }
  }
  
  vTrace("Chosen: %lld\n"_Str,m_i_Chosen);
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    CandidatePath* pCandidatePath = new CandidatePath;
    
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      pCandidatePath->vAdd(pColumn);
    }
    
    m_pCandidatePaths->vAdd(pCandidatePath);
    m_pIndex[m_nCandidates] = pIndex;
    m_nCandidates++;
  }
  
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* mpTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  vTrace("CandidatePaths::vGenPaths \n"_Str);
  
  m_pCandidatePaths = new CandidatePaths;
  
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
  
  for (i8 i_Path = 0; i_Path < m_pCandidatePaths->nItems(); i_Path++) {
    CandidatePath* mpPath = m_pCandidatePaths->pGet(i_Path);
    
    bool bMatchAll = true;
    for (i8 i_Col = 0; i_Col < mpPath->nItems(); i_Col++) {
      Column* mpColumn = mpPath->pGet(i_Col);
      
      bool bHasColumn = false;
      for (i8 i_Pred = 0; i_Pred < m_pPredicateVec->nItems(); i_Pred++) {
        Predicate* mpPredicate = m_pPredicateVec->pGet(i_Pred);
        
        s256 sColumnNm = mpPredicate->pArg1()->sArgVal();
 
        if (mpColumn->bIsName((s32)sColumnNm)) {
          bHasColumn = true;
          break;
        }
      }
      
      if (!bHasColumn) {
        bMatchAll = false;
        break;
      }
    }
    
    if (bMatchAll)
      m_bIsCandidate[i_Path] = true;
  }
}

/*----------------------------------------------------------------------------*/
/* show the generated set of paths                                            */
/*----------------------------------------------------------------------------*/
void Planner::vShowPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
 
  for (i8 i_Path = 0; i_Path < m_pCandidatePaths->nItems(); i_Path++) {
    
    CandidatePath* pCandidatePath = m_pCandidatePaths->pGet(i_Path);
    
    vTrace("[Cdt %2lld : index %s : score %2lld : cdt %s] "_Str,
           i_Path,
           m_pIndex[i_Path]->aIndexName(),
           m_iScoreVec[i_Path],
           m_bIsCandidate[i_Path] ? "yes" : "no ");
    
    for (i8 i_IdxCol = 0; i_IdxCol < pCandidatePath->nItems(); i_IdxCol++) {
      Column* pColumn = pCandidatePath->pGet(i_IdxCol);
      vTrace("%s%s"_Str,i_IdxCol ? ", "_Str : ""_Str,pColumn->sColumnName().aStr());
    }
    vTrace("\n"_Str);
  }
}

/*----------------------------------------------------------------------------*/
/* score the set of all paths                                                 */
/*----------------------------------------------------------------------------*/
void Planner::vScorePaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  for (i8 i_Path = 0; i_Path < m_pCandidatePaths->nItems(); i_Path++) {
    CandidatePath* pCandidatePath = m_pCandidatePaths->pGet(i_Path);
    Index* pIndex = m_pIndex[i_Path];

    // initially just a simple scoring method 
    i8 iScore = 10 * (pIndex->bIsUnique() ? 0 : 1)
                + pCandidatePath->nItems();
    
    m_iScoreVec[i_Path] = iScore;
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
/* parse where clause                                                         */
/*----------------------------------------------------------------------------*/
void Planner::vParseWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
    
  TokenSet xTokenSet;
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPush();
  pTokeniser->vUseString(m_sWhere);
  
  eFT eFetchTechnique;
  Op* pOp;
  
  m_pPredicateVec = new PredicateVec;
  for (i8 i_Predicate = 0; ; i_Predicate++) {
    
    if (i_Predicate) {
      pTokeniser->vNextToken();
      if (TK_NONE == pTokeniser->eToken())
        break;
      ASSERT(pTokeniser->sToken().bSame("and"_Str));
    }
    
    pTokeniser->vNextToken();
  
    Arg* pArg1 = new Arg(AK_COLUMNNM,pTokeniser->sToken());

    pTokeniser->vNextToken();
    if (pTokeniser->sToken().bSame(">"_Str)) {
      pOp = new Op(RO_GREATER_THAN);
      eFetchTechnique = FT_LOWER;
    }
    else
    if (pTokeniser->sToken().bSame(">="_Str)) {
      pOp = new Op(RO_GREATER_EQUAL);
      eFetchTechnique = FT_LOWER;
    }
    else
    if (pTokeniser->sToken().bSame("="_Str) ||
        pTokeniser->sToken().bSame("=="_Str)) {
      pOp = new Op(RO_EQUAL_TO);
      eFetchTechnique = FT_EQUAL;
    }
    else
    if (pTokeniser->sToken().bSame("<="_Str)) {
      pOp = new Op(RO_LESS_EQUAL);
      eFetchTechnique = FT_UPPER;
    }
    else
    if (pTokeniser->sToken().bSame("<"_Str)) {
      pOp = new Op(RO_LESS_THAN);
      eFetchTechnique = FT_UPPER;
    }
    else {
      ASSERT(0);
    }
    
    pTokeniser->vNextToken();
    Arg* pArg2 = new Arg(AK_LITERAL,pTokeniser->sToken());
    
    Predicate* pPredicate =
      new Predicate(PK_COLUMN_OP_LITERAL,pArg1,pOp,pArg2,eFetchTechnique);
    
    m_pPredicateVec->vAdd(pPredicate);
  }

}

/*----------------------------------------------------------------------------*/
/* test predicates                                                            */
/*----------------------------------------------------------------------------*/
void Planner::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
/*
  Arg* pArg1 = new Arg(AK_NONE,"jA"_Str);
  Op*  pOp   = new Op(RO_LESS_EQUAL);
  Arg* pArg2 = new Arg(AK_NONE,"7"_Str);
  Predicate* pPredicate = new Predicate(PB_UPPER,PK_COLUMN_OP_LITERAL,pArg1,pOp,pArg2);
  
  IdxVec* pIdxVec = m_pTable->pIdxVec();
  
  bool bFoundColumn = false;
  s256 sColumnNm;
  for (i8 i_Idx = 0; i_Idx < pIdxVec->nItems(); i_Idx++) {
    Index* pIndex = pIdxVec->pGet(i_Idx);
    IdxColVec* pIdxColVec = pIndex->pIdxColVec();
    for (i8 i_IdxCol = 0; i_IdxCol < pIdxColVec->nItems(); i_IdxCol++) {
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      sColumnNm = pColumn->sColumnName();
      if (pArg1->bIsColumnNm(sColumnNm)) {
        pArg1->vArgKind(AK_COLUMNNM);
        pArg2->vArgKind(AK_LITERAL);
        pPredicate->vPredicate(PK_COLUMN_OP_LITERAL);
      }
      else if (pArg2->bIsColumnNm(sColumnNm)) {
        pArg2->vArgKind(AK_COLUMNNM);
        pArg1->vArgKind(AK_LITERAL);
        pPredicate->vPredicate(PK_LITERAL_OP_COLUMN);
      }
      else
        pPredicate->vPredicate(PK_LITERAL_OP_LITERAL);
      
      if (pPredicate->pOp()->eOpKind() == RO_GREATER_THAN ||
          pPredicate->pOp()->eOpKind() == RO_GREATER_EQUAL)
        pPredicate->vBounds(PB_LOWER);
      else
      if (pPredicate->pOp()->eOpKind() == RO_LESS_THAN ||
          pPredicate->pOp()->eOpKind() == RO_LESS_EQUAL)
        pPredicate->vBounds(PB_UPPER);
      else
      if (pPredicate->pOp()->eOpKind() == RO_EQUAL_TO)
        pPredicate->vBounds(PB_EXACT);
      else
        pPredicate->vBounds(PB_NONE);
    }
    
  }
*/
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
CandidatePath* Planner::pCandidatePath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  return m_pCandidatePaths->pGet(m_i_Chosen);
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

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
_aFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
