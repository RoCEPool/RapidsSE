/*============================================================================*/
/* Loader.cpp -- Parallel Table Loader                                        */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Heap*     gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader* DBobjects::_pLoader(void* aAddr)
{
  Loader* pLoader = reinterpret_cast<Loader*>(aAddr);
  
  RASSERT( !pLoader || CN_LOADER == pLoader->eClass() );
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_LOADER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t iSize, Loader* pLoader)
{
  RASSERT(iSize);
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* pLoaderAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(pLoaderAddress);
  
  Heap::vFree(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(void)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sPath    = "<none>"_Str;
  m_pTable   = nullptr;
  m_pTxRef   = nullptr;
  m_pLoadMap = new LoadMap;
  
  _checkInitialised_(CN_LOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  vFinalize();
}

/*----------------------------------------------------------------------------*/
/* dump Loader                                                                */
/*----------------------------------------------------------------------------*/
void Loader::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader* pLoader = reinterpret_cast<Loader*>(pObject);
  
  vTrace("table %s\n"_Str,pLoader->m_pTable->aTableName());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ffinalize the loader                                                       */
/*----------------------------------------------------------------------------*/
void Loader::vFinalize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* find next chunk in file being loaded                                       */
/*----------------------------------------------------------------------------*/
bool Loader::bFindChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  bool b = m_pLoadMap->bFindChunk();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get loap map                                                               */
/*----------------------------------------------------------------------------*/
LoadMap* Loader::pLoadMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_pLoadMap;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
StrVal& Loader::sPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_sPath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vPath(StrVal& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sPath = sPath;
    
  m_pLoadMap->vFilePath(sPath);
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get txref                                                                  */
/*----------------------------------------------------------------------------*/
TxRef* Loader::pTxRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_pTxRef;
}

/*----------------------------------------------------------------------------*/
/* set txref                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTxRef(TxRef* pTxRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  m_pTxRef = pTxRef;
  
  //vTrace("TxId %lld\n"_Str,pTxRef->iTxId());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Loader::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
