/*============================================================================*/
/* LoadTable.cpp -- Parallel Table LoadTable                                        */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadTable* Utilities::_pLoader(void* aAddr)
{
  LoadTable* pLoadTable = reinterpret_cast<LoadTable*>(aAddr);
  
  RASSERT( !pLoadTable || CN_LOADTABLE == pLoadTable->eClass() );
  
  return pLoadTable;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* LoadTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = HeapMem::pAllocate(CN_LOADTABLE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* LoadTable::operator new(size_t iSize, LoadTable* pLoadTable)
{
  RASSERT(iSize);
  return pLoadTable;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void LoadTable::operator delete(void* pLoaderAddress)
{
  LoadTable* pLoadTable = reinterpret_cast<LoadTable*>(pLoaderAddress);
  
  HeapMem::vFree(pLoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadTable::LoadTable(Job_LoadTable* pJob_LoadTable)
: Object(CN_LOADTABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  m_pJob_LoadTable = pJob_LoadTable;
  
  _checkInitialised_(CN_LOADTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadTable::~LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get job_loadTable                                                          */
/*----------------------------------------------------------------------------*/
Job_LoadTable* LoadTable::pJob_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  return m_pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoadTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
