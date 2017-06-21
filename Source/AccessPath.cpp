/*============================================================================*/
/* AccessPath.cpp - Access Path                                               */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
AccessPath* Plans::_pAccessPath(void* aAddr)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(aAddr);
  
  RASSERT( !pAccessPath || CN_ACCESSPATH == pAccessPath->eClass() );
 
  return pAccessPath;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AccessPath::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_ACCESSPATH);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void AccessPath::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AccessPath::AccessPath(void)
: Object(CN_ACCESSPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pTable     = nullptr;
  m_pPartition = nullptr;
  m_pIndex     = nullptr;
  
  m_bIsCandidate = false;
  m_iPathRating = 0;
  
  m_eAccess = FT_NONE;
  m_pLowerPredicates = nullptr;
  m_pUpperPredicates = nullptr;
  
  _checkInitialised_(CN_ACCESSPATH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AccessPath::~AccessPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* AccessPath::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void AccessPath::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* AccessPath::pPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void AccessPath::vPartition(Partition* pPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pPartition = pPartition;
}

/*----------------------------------------------------------------------------*/
/* is candidate path?                                                         */
/*----------------------------------------------------------------------------*/
bool AccessPath::bIsCandidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_bIsCandidate;
}

/*----------------------------------------------------------------------------*/
/* set is candidate path?                                                     */
/*----------------------------------------------------------------------------*/
void AccessPath::vIsCandidate(bool bIsCandidate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_bIsCandidate = bIsCandidate;
}

/*----------------------------------------------------------------------------*/
/* get index                                                                  */
/*----------------------------------------------------------------------------*/
Index* AccessPath::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void AccessPath::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get score                                                                  */
/*----------------------------------------------------------------------------*/
i8 AccessPath::iPathRating(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_iPathRating;
}

/*----------------------------------------------------------------------------*/
/* set score                                                                  */
/*----------------------------------------------------------------------------*/
void AccessPath::vPathRating(i8 iPathRating)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_iPathRating = iPathRating;
}

/*----------------------------------------------------------------------------*/
/* get access technique                                                       */
/*----------------------------------------------------------------------------*/
eFT AccessPath::eAccess(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_eAccess;
}

/*----------------------------------------------------------------------------*/
/* set access technique                                                       */
/*----------------------------------------------------------------------------*/
void AccessPath::vAccess(eFT eAccess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_eAccess = eAccess;
}

/*----------------------------------------------------------------------------*/
/* get lower bound predicates                                                 */
/*----------------------------------------------------------------------------*/
PredicateVec* AccessPath::pLowerPredicates(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pLowerPredicates;
}

/*----------------------------------------------------------------------------*/
/* set lower bound predicates                                                 */
/*----------------------------------------------------------------------------*/
void AccessPath::vLowerPredicates(PredicateVec* pLowerPredicates)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pLowerPredicates = pLowerPredicates;
}

/*----------------------------------------------------------------------------*/
/* get upper bound predicates                                                 */
/*----------------------------------------------------------------------------*/
PredicateVec* AccessPath::pUpperPredicates(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  return m_pUpperPredicates;
}

/*----------------------------------------------------------------------------*/
/* set upper bound predicates                                                 */
/*----------------------------------------------------------------------------*/
void AccessPath::vUpperPredicates(PredicateVec* pUpperPredicates)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  m_pUpperPredicates = pUpperPredicates;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void AccessPath::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ACCESSPATH,_FOOTER_);
  
  vTrace(" %s "_Str,m_bIsCandidate ? "cdt"_Str : "   "_Str);
  
  if (m_pLowerPredicates) {
    for (i8 i_Pred = 0; i_Pred < m_pLowerPredicates->nItems(); i_Pred++) {
      Predicate* pLowerPredicate = m_pLowerPredicates->pGet(i_Pred);
      pLowerPredicate->vShow();
    }
  }
  
  if (m_pUpperPredicates) {
    for (i8 i_Pred = 0; i_Pred < m_pUpperPredicates->nItems(); i_Pred++) {
      Predicate* pUpperPredicate = m_pUpperPredicates->pGet(i_Pred);
      pUpperPredicate->vShow();
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(AccessPath)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
