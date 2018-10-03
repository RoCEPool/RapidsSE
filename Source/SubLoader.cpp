/*============================================================================*/
/* SubLoader.cpp -- Subordinate Parallel Table LoadTable                      */
/*============================================================================*/

#include "Utilities.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SubLoader* Utilities::_pLoaderSub(void* aAddr)
{
  SubLoader* pSubLoader = reinterpret_cast<SubLoader*>(aAddr);
  
  RASSERT( !pSubLoader || CN_SUBLOADER == pSubLoader->eClass() );
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = HeapMem::pAllocate(CN_SUBLOADER);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* SubLoader::operator new(size_t iSize, SubLoader* pSubLoader)
{
  RASSERT(iSize);
  
  return pSubLoader;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void SubLoader::operator delete(void* pLoaderAddress)
{
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pLoaderAddress);
  
  HeapMem::vFree(pLoadTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubLoader::SubLoader(Job_SubLoader* pJob_SubLoader)
: Object(CN_SUBLOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);

  m_pJob_SubLoader = pJob_SubLoader;

  _checkInitialised_(CN_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubLoader::~SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void SubLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SUBLOADER);
  RASSERT(pObject->eClass() == CN_SUBLOADER);
  
  SubLoader* pLoadTable = reinterpret_cast<SubLoader*>(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool SubLoader::bFetchRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  Stz* aRawData;
  i8 iLength;
  bool bValid = false;
  bool bBlankLine;
  
  Job_LoadTable* pJob_LoadTable = m_pJob_SubLoader->pJob_LoadTable();
  Formatters* pFormatters = &pJob_LoadTable->m_xFormatters;
  Table* pTable = pJob_LoadTable->m_pTable;
  
  if (! m_pJob_SubLoader->m_bMoreChunks)
    return false;
  
  while (true) {
    
    // fetch next record from current chunk
    aRawData = m_pJob_SubLoader->m_pLoadChunk->aNextRecord(iLength,pFormatters);
    
    if (! aRawData) {
      // read next chunk
      m_pJob_SubLoader->m_bMoreChunks = m_pJob_SubLoader->bReadChunk();
      if (! m_pJob_SubLoader->m_bMoreChunks)
        return false;
      continue;
    }
    
    if (iLength > C_16KB) {
      m_pJob_SubLoader->m_iInvalidRecords++;
      vWarn("Skipping row %.20s... - record in file > 16KB\n"_Stz,
            aRawData);
      return false;
    }

    // format the raw data into a row
    bValid = Row::bFormatRow(pTable->pColumnVec(),
                             pFormatters,
                             aRawData, iLength,
                             m_pJob_SubLoader->m_aRecord.aStr(), m_pJob_SubLoader->m_iLength,
                             bBlankLine,
                             false /* allowNonPrintable */);
    if (bValid) {
      m_pJob_SubLoader->m_iValidRecords++;
      return bValid;
    }
    
    if ( ! bBlankLine )
      m_pJob_SubLoader->m_iInvalidRecords++;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get loader                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader* SubLoader::pJob_SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  return m_pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* process records                                                            */
/*----------------------------------------------------------------------------*/
bool SubLoader::bProcessRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER,_FOOTER_);
  
  while (bFetchRecord()) {
    
    // hash the record to get m_iPartition
    m_pJob_SubLoader->vHashRecord();
    
    // attempt to add record into a buffer
    bool bInserted = m_pJob_SubLoader->bInsertRow();
    
    // check if target buffer was full
    if (! bInserted) {
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      return true;
    }
    
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SubLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBLOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
