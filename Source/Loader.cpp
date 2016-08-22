/*============================================================================*/
/* Loader.cpp -- Table Loader                                                 */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t)
{
  void* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* pLoaderAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(pLoaderAddress);
  
  gpSmallHeap->vFreeObject(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(void)
: Object(CN_LOADER)
{
  _poolAddress_(this);
  
  m_pTable  = nullptr;
  //m_xFile
  m_pRecord = nullptr;
  m_pScanBuffer = new ScanBuffer;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void)
{
  _poolAddress_(this);
  
  delete m_pScanBuffer;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Loader::pTable(void)
{
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
void Loader::vTable(Table* pTable)
{
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void Loader::vPath(s& sPath)
{
  const char* pPath = sPath.a();
  
  m_xFile.vPath(pPath);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Loader::bOpenFile(void)
{
  bool b = m_xFile.bOpen(C_OPEN_READ);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* fetch next record                                                          */
/*----------------------------------------------------------------------------*/
bool Loader::bFetchRecord(void)
{
  i8 iBytes;

  m_pRecord = m_pScanBuffer->pScan(iBytes);
  if ( m_pRecord )
    return true;
  
  char* pInto = m_pScanBuffer->pInto(iBytes);
  
  i8 iRead = m_xFile.iReadChunk(pInto,iBytes);
  m_pScanBuffer->vMore(iRead);

  m_pRecord = m_pScanBuffer->pScan(iBytes);
  if ( m_pRecord )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* insert row                                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vInsertRow(void)
{
  StrVal xRow;
  xRow = m_pRecord;
  
  m_pTable->vInsertRow(&xRow);
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Loader::vCloseFile(void)
{
  m_xFile.vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/