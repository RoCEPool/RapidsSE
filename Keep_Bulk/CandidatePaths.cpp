/*============================================================================*/
/* CandidatePaths.cpp - Set of Candidate Paths                                */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CandidatePaths* Plans::_pCandidatePaths(void* aAddr)
{
  CandidatePaths* pCandidatePaths = reinterpret_cast<CandidatePaths*>(aAddr);
  
  RASSERT( ! pCandidatePaths || CN_CANDIDATEPATHS == pCandidatePaths->eClass() );
 
  return pCandidatePaths;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CandidatePaths::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_CANDIDATEPATHS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void CandidatePaths::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CandidatePaths::CandidatePaths(void)
: Object(CN_CANDIDATEPATHS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);
  
  _checkInitialised_(CN_CANDIDATEPATHS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CandidatePaths::~CandidatePaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void CandidatePaths::vGenPaths(Table* pTable, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  for (i8 nCols = 1; nCols <= pIdxColVec->nItems(); nCols++) {
    
    CandidatePath* pCandidatePath = new CandidatePath;
    
    for (i8 i_IdxCol = 0; i_IdxCol < nCols; i_IdxCol++) {
      Column* pColumn = pIdxColVec->pGet(i_IdxCol);
      pCandidatePath->vAppend(pColumn);
    }
    
    this->vAppend(pCandidatePath);
  }
  
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void CandidatePaths::vGenPaths(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);
  
  vTrace("CandidatePaths::vGenPaths \n"_Str);
  
  /*
   
    foreach index
   
      foreach column
        recursively generate all column permutations
          does the column subset match the lhs column of a rdp predicate?
          yes: add possible path
  */
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  
  for (i8 i_Idx = 0; i_Idx < pIdxVec->nItems(); i_Idx++) {
    Index* pIndex = pIdxVec->pGet(i_Idx);
    vGenPaths(pTable, pIndex);
  }
}

/*----------------------------------------------------------------------------*/
/* generate the set of all paths for given table                              */
/*----------------------------------------------------------------------------*/
void CandidatePaths::vShowPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS,_FOOTER_);

  ListCursor_t<CandidatePath> xCursor(this);
  
  for (xCursor.vFirst(); xCursor.pObject(); xCursor.vNext()) {
    CandidatePath* pCdt = xCursor.pObject();
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CandidatePaths::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATHS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
