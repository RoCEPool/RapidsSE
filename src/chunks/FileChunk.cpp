/*============================================================================*/
/* FileChunk.cpp -- File Chunk                                                */
/*============================================================================*/

#include "Chunks.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FileChunk* Chunks::_pFileChunk(void* aAddr)
{
  FileChunk* pFileChunk = reinterpret_cast<FileChunk*>(aAddr);
  
  return pFileChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FileChunk::operator new(size_t)
{
  FileChunk* pFileChunk = (FileChunk*)HeapMem::pAllocate(CN_IOCHUNK);
  
  return pPoolCheck<void*>(pFileChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void FileChunk::operator delete(void* pFileChunkAddress)
{
  FileChunk* pFileChunk = static_cast<FileChunk*>(pFileChunkAddress);
  
  HeapMem::vFree(pFileChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FileChunk::FileChunk(eCN eClassNo, Format* pFmt)
: RecordChunk(eClassNo,pFmt)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pFile = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FileChunk::~FileChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void FileChunk::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pFile->vClose();
  
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool FileChunk::bOpenRead(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pFile = new File(nullptr);
  
  // open the file
  bool bOpened = m_pFile->bOpenRead(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool FileChunk::bOpenWrite(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pFile = new File(nullptr);

  // open the file
  bool bOpened = m_pFile->bOpenWrite(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
i8 FileChunk::nReadBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (-1 == nBytes)
    nBytes = C_WSP_DATACHUNK - 1;
  else
  if (nBytes == C_WSP_DATACHUNK)
    nBytes -= 1;
  
  m_nDataBytes = m_pFile->nReadBytes(m_xWsp.ayArea(),nBytes);

  if (! m_nDataBytes)
    return 0;
  
  ((char*)m_xWsp.szStz())[m_nDataBytes] = '\0';
  
  m_aLastDataByte = &m_xWsp.szStz()[m_nDataBytes - 1];
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* read given chunk from file                                                 */
/*----------------------------------------------------------------------------*/
void FileChunk::vReadSpecificChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pFile->vSeek(iSeek);
  
  Stz* aStartArea = (Stz*)aDataArea();
  Stz* aIntoAddr  = aStartArea;
  i8   nAsMuchAs  = nBytes;
  
  i8 nBytesRead = m_pFile->nReadBytes(aIntoAddr,nAsMuchAs);
  
  RASSERT(nBytesRead == nAsMuchAs);
  
  m_nUnprocessedBytes = nBytesRead;
  m_nDataBytes = m_nUnprocessedBytes;
  
  m_aLastDataByte = &(aStartArea)[m_nDataBytes - 1];
  m_szNextRecord  = aStartArea;
}

/*----------------------------------------------------------------------------*/
/* rewind file                                                                */
/*----------------------------------------------------------------------------*/
void FileChunk::vRewindFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pFile->vSeekStart();
}

/*----------------------------------------------------------------------------*/
/* write data                                                                 */
/*----------------------------------------------------------------------------*/
void FileChunk::vWriteData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bResult = m_pFile->bWriteBytes(m_xWsp.ayArea(),m_nDataBytes);
  RASSERT(bResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
