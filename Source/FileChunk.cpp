/*============================================================================*/
/* FileChunk.cpp -- File Chunk                                                */
/*============================================================================*/

#include "Chunks.h"

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
FileChunk::FileChunk(eCN eClassNo)
: DataChunk(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //m_oFile
  
  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FileChunk::~FileChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
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
  
  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void FileChunk::vOpenRead(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // open the file
  bool b = m_oFile.bOpen(sPath,C_OPEN_READ);
  RASSERT(b);
  
  // set buffering
  //m_oFile.vSetBuffering(C_1MB);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void FileChunk::vOpenWrite(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // open the file
  bool b = m_oFile.bOpen(sPath,C_OPEN_WRITE);
  RASSERT(b);
  
  // set buffering
  //m_oFile.vSetBuffering(C_1MB);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
i8 FileChunk::nReadBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (-1 == nBytes)
    nBytes = C_DATA_CHUNK - 1;
  else
  if (nBytes == C_DATA_CHUNK)
    nBytes -= 1;
  
  m_nDataBytes = m_oFile.iReadBytes(m_xWsp.aArea(),nBytes);

  if (! m_nDataBytes)
    return 0;
  
  ((char*)m_xWsp.aStr())[m_nDataBytes] = '\0';
  
  m_aLastDataByte = &m_xWsp.aStr()[m_nDataBytes - 1];
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* rewind file                                                                */
/*----------------------------------------------------------------------------*/
void FileChunk::vRewindFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oFile.vSeekStart();
}

/*----------------------------------------------------------------------------*/
/* write data                                                                 */
/*----------------------------------------------------------------------------*/
void FileChunk::vWriteData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bResult = m_oFile.bWriteBytes(m_xWsp.aArea(),m_nDataBytes);
  RASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get wsp as string pointer                                                  */
/*----------------------------------------------------------------------------*/
Stz* FileChunk::aStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_xWsp.aStr();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
