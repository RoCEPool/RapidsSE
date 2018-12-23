/*============================================================================*/
/* IoChunk.cpp -- I/O Chunk                                                   */
/*============================================================================*/

#include "Loader.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IoChunk* Loader::_pIoChunk(void* aAddr)
{
  IoChunk* pIoChunk = reinterpret_cast<IoChunk*>(aAddr);
  
  RASSERT_TYPE(pIoChunk,CN_IOCHUNK);
  
  return pIoChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IoChunk::operator new(size_t)
{
  IoChunk* pIoChunk = (IoChunk*)HeapMem::pAllocate(CN_IOCHUNK);
  
  return pPoolCheck<void*>(pIoChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IoChunk::operator delete(void* pIoChunkAddress)
{
  IoChunk* pIoChunk = static_cast<IoChunk*>(pIoChunkAddress);
  
  HeapMem::vFree(pIoChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IoChunk::IoChunk(void)
: DataChunk(CN_IOCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);

  //m_xFile
  
  _checkInitialised_(CN_IOCHUNK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IoChunk::~IoChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump IoChunk                                                               */
/*----------------------------------------------------------------------------*/
void IoChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //IoChunk* pIoChunk = _pIoChunk(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void IoChunk::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void IoChunk::vOpenRead(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool b = m_xFile.bOpen(C_OPEN_READ);
  RASSERT(b);
  
  // set no buffering
  m_xFile.vBuffer(0);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void IoChunk::vOpenWrite(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool b = m_xFile.bOpen(C_OPEN_WRITE);
  RASSERT(b);
  
  // set no buffering
  m_xFile.vBuffer(0);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
i8 IoChunk::nReadBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  if (-1 == nBytes)
    nBytes = C_IO_CHUNK - 1;
  else
  if (nBytes == C_IO_CHUNK)
    nBytes -= 1;
  
  m_nDataBytes = m_xFile.iReadBytes(m_xWsp.aArea(),nBytes);

  if (! m_nDataBytes)
    return 0;
  
  ((char*)m_xWsp.aStr())[m_nDataBytes] = '\0';
  
  m_aLastDataByte = &m_xWsp.aStr()[m_nDataBytes - 1];
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* write data                                                                 */
/*----------------------------------------------------------------------------*/
void IoChunk::vWriteData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  bool bResult = m_xFile.bWriteBytes(m_xWsp.aArea(),m_nDataBytes);
  RASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get wsp as string pointer                                                  */
/*----------------------------------------------------------------------------*/
Stz* IoChunk::aStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IoChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
