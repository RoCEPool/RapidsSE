/*============================================================================*/
/* Loader.cpp -- Parallel Table Loader                                        */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader* Utilities::_pLoader(void* aAddr)
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
Loader::Loader(Job_LoadTable* pJob_LoadTable)
: Object(CN_LOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pJob_LoadTable = pJob_LoadTable;
  
  m_pLoadMap = new LoadMap;
  m_pLoadMap->vFilePath(m_pJob_LoadTable->m_sFileName);
  
  m_ziInvalidRecords = 0;
  m_ziValidRecords   = 0;

  m_pSEitf   = nullptr;
  m_sPath    = "<none>"_Stz;
  m_pTable   = nullptr;

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
  RASSERT(eClass == CN_LOADER);
  RASSERT(pObject->eClass() == CN_LOADER);
  
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
/* add invalid records                                                        */
/*----------------------------------------------------------------------------*/
void Loader::vAddInvalidRecords(i8 iInvalidRecords)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_ziInvalidRecords.iAdd(iInvalidRecords);
}

/*----------------------------------------------------------------------------*/
/* add valid records                                                          */
/*----------------------------------------------------------------------------*/
void Loader::vAddValidRecords(i8 iValidRecords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_ziValidRecords.iAdd(iValidRecords);
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
/* get invalid records                                                        */
/*----------------------------------------------------------------------------*/
i8 Loader::iInvalidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return *m_ziInvalidRecords;
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
/* get valid records                                                          */
/*----------------------------------------------------------------------------*/
i8 Loader::iValidRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  return *m_ziValidRecords;
}

/*----------------------------------------------------------------------------*/
/* advance to last character of record ( \n or \r )                           */
/*----------------------------------------------------------------------------*/
void Loader::vAdvanceRecordEnd(char** aaSrc, Formatters* pFormatters)
{
  // test for explicit record ends
  
  switch (pFormatters->m_eRecordEnds) {
      
    case RE_NL: // advance to next \n
      for (;;) {
        // find next \n
        while (C_NL != **aaSrc)
          (*aaSrc)++;
        // if there is no record terminator we have found ending \n
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \n if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      ASSERT(C_NL == **aaSrc);
      break;
      
    case RE_CR: // advance to next \r
      for (;;) {
        // find next \r
        while (C_CR != **aaSrc)
          (*aaSrc)++;
        // if there is no record terminator we have found ending \r
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      ASSERT(C_CR == **aaSrc);
      break;
      
    case RE_CRNL: // advance to next \r\n
      for (;;) {
        // find next \r\n
        while (C_CR != **aaSrc && C_NL != *(*aaSrc+1))
          (*aaSrc)++;
        // if there is no record terminator we have found ending \r\n
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r\n if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      // skip over \r to be on the \n
      ASSERT(C_CR == **aaSrc);
      (*aaSrc)++;
      ASSERT(C_NL == **aaSrc);
      break;
      
    default:
      RASSERT(0);
      break;
  }
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
