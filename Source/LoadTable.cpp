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
  
  m_pLoadMap = new LoadMap;
  m_pLoadMap->vFilePath(m_pJob_LoadTable->m_sFileName);
  
  m_pTable   = nullptr;

  _checkInitialised_(CN_LOADTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadTable::~LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* dump LoadTable                                                                */
/*----------------------------------------------------------------------------*/
void LoadTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_LOADTABLE);
  RASSERT(pObject->eClass() == CN_LOADTABLE);
  
  LoadTable* pLoadTable = reinterpret_cast<LoadTable*>(pObject);
  
  vTrace("table %s\n"_Stz,pLoadTable->m_pTable->aTableName());
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
  
  pLoadTable->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the load map                                                          */
/*----------------------------------------------------------------------------*/
void LoadTable::vDumpMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  vTrace("Load Map for: %s:\n"_Stz,m_pJob_LoadTable->m_sFileName.aStr());
  
  m_pLoadMap->vDumpMap();
}

/*----------------------------------------------------------------------------*/
/* find next chunk in file being loaded                                       */
/*----------------------------------------------------------------------------*/
bool LoadTable::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  bool b = m_pLoadMap->bFindChunk();
  
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
  
  return m_pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* LoadTable::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void LoadTable::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);

  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void LoadTable::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADTABLE,_FOOTER_);
  
  delete m_pLoadMap;

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
