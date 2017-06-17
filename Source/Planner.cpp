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
  
  m_pTable          = nullptr;
  m_pCandidatePaths = nullptr;
  for (i8 i_Score = 0; i_Score < C_INDEXES_PER_TABLE; i_Score++) {
    m_pIndex[i_Score]    = nullptr;
    m_iScoreVec[i_Score] = 0;
  }
  
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
  }
  
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vGenPaths(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
  vTrace("CandidatePaths::vGenPaths \n"_Str);
  
  m_pCandidatePaths = new CandidatePaths;
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  
  for (i8 i_Idx = 0; i_Idx < pIdxVec->nItems(); i_Idx++) {
    Index* pIndex = pIdxVec->pGet(i_Idx);
    m_pIndex[i_Idx] = pIndex;
    vGenPaths(pTable, pIndex);
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void Planner::vShowPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
 
  for (i8 i_Path = 0; i_Path < m_pCandidatePaths->nItems(); i_Path++) {
    CandidatePath* pCandidatePath = m_pCandidatePaths->pGet(i_Path);
    for (i8 i_IdxCol = 0; i_IdxCol < pCandidatePath->nItems(); i_IdxCol++) {
      Column* pColumn = pCandidatePath->pGet(i_IdxCol);
      vTrace("%s%s"_Str,i_IdxCol ? "."_Str : ""_Str,pColumn->sColumnName().aStr());
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
    i8 iScore =   10 * (pIndex->bIsUnique() ? 0 : 1)
                + pCandidatePath->nItems();
    
    m_iScoreVec[i_Path] = iScore;
  }
}

/*----------------------------------------------------------------------------*/
/* test predicates                                                            */
/*----------------------------------------------------------------------------*/
void Planner::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNER,_FOOTER_);
  
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
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
_pFooter_(Planner)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
