/*============================================================================*/
/* CandidatePath.cpp - Set of Candidate Paths                                 */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CandidatePath* Plans::_pCandidatePath(void* aAddr)
{
  CandidatePath* pCandidatePath = reinterpret_cast<CandidatePath*>(aAddr);
  
  RASSERT( ! pCandidatePath || CN_CANDIDATEPATH == pCandidatePath->eClass() );
 
  return pCandidatePath;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CandidatePath::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_CANDIDATEPATH);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void CandidatePath::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CandidatePath::CandidatePath(void)
: Object(CN_CANDIDATEPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATH,_FOOTER_);
  
  _checkInitialised_(CN_CANDIDATEPATH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CandidatePath::~CandidatePath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
void CandidatePath::vGenPaths(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATH,_FOOTER_);
  
  /*
   
    foreach index
   
      foreach column
        recursively generate all column permutations
          does the column subset match the lhs column of a rdp predicate?
          yes: add possible path
  */
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CandidatePath::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CANDIDATEPATH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
