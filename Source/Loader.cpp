/*============================================================================*/
/* Loader.cpp -- Parallel Table Loader                                        */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

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
  
  m_pSEitf   = nullptr;
  m_sPath    = "<none>"_Stz;
  m_pTable   = nullptr;
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
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* dump Loader                                                                */
/*----------------------------------------------------------------------------*/
void Loader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader* pLoader = reinterpret_cast<Loader*>(pObject);
  
  vTrace("table %s\n"_Stz,pLoader->m_pTable->aTableName());
}

/*----------------------------------------------------------------------------*/
/* get loap map                                                               */
/*----------------------------------------------------------------------------*/
void Loader::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_LOADER);
  ASSERT(pObject->eClass() == CN_LOADER);
  
  Loader* pLoader = reinterpret_cast<Loader*>(pObject);
  ASSERT(pLoader->bWordBody());
  
  pLoader->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get enclosed by char                                                       */
/*----------------------------------------------------------------------------*/
StrVal& Loader::sEnclosedBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_sEnclosedBy;
}

/*----------------------------------------------------------------------------*/
/* set enclosed by char                                                       */
/*----------------------------------------------------------------------------*/
void Loader::vEnclosedBy(StrVal& sEnclosedBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sEnclosedBy = sEnclosedBy;
}

/*----------------------------------------------------------------------------*/
/* get escape char                                                            */
/*----------------------------------------------------------------------------*/
StrVal& Loader::sEscapeChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_sEscapeChar;
}

/*----------------------------------------------------------------------------*/
/* set escape char                                                            */
/*----------------------------------------------------------------------------*/
void Loader::vEscapeChar(StrVal& sEscapeChar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sEscapeChar = sEscapeChar;
}

/*----------------------------------------------------------------------------*/
/* get field separator                                                        */
/*----------------------------------------------------------------------------*/
StrVal& Loader::sFieldSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_sFieldSeparator;
}

/*----------------------------------------------------------------------------*/
/* set field separator                                                        */
/*----------------------------------------------------------------------------*/
void Loader::vFieldSeparator(StrVal& sFieldSeparator)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sFieldSeparator = sFieldSeparator;
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
/* get record separator                                                       */
/*----------------------------------------------------------------------------*/
StrVal& Loader::sRecordTerminator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_sRecordTerminator;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vRecordTerminator(StrVal& sRecordTerminator)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_sRecordTerminator = sRecordTerminator;
}

/*----------------------------------------------------------------------------*/
/* get SEitf                                                                  */
/*----------------------------------------------------------------------------*/
Interface::SEitf* Loader::pSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/* set SEitf                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vSEitf(Interface::SEitf* pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pSEitf = pSEitf;
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
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Loader::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pLoadMap;

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Loader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
