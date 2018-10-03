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
/* dump LoadTable                                                             */
/*----------------------------------------------------------------------------*/
void LoadTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_LOADTABLE);
  RASSERT(pObject->eClass() == CN_LOADTABLE);
}

/*----------------------------------------------------------------------------*/
/* get loap map                                                               */
/*----------------------------------------------------------------------------*/
void LoadTable::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_LOADTABLE);
  ASSERT(pObject->eClass() == CN_LOADTABLE);
  
  LoadTable* pLoadTable = reinterpret_cast<LoadTable*>(pObject);
  ASSERT(pLoadTable->bWordBody());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find next chunk in file being loaded                                       */
/*----------------------------------------------------------------------------*/
bool LoadTable::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  bool b = m_pJob_LoadTable->bFindChunk();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get job_loadTable                                                          */
/*----------------------------------------------------------------------------*/
Job_LoadTable* LoadTable::pJob_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  return m_pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* get loap map                                                               */
/*----------------------------------------------------------------------------*/
LoadMap* LoadTable::pLoadMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  return m_pJob_LoadTable->m_pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* LoadTable::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);

  return m_pJob_LoadTable->m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void LoadTable::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);

  m_pJob_LoadTable->m_pTable = pTable;
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
