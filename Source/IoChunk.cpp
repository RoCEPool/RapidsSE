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
: Object(CN_IOCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);

  m_xWsp.vFill(0xEF,CN_IOCHUNK);
  
  
  ASSERT(! m_aEndChunk);
  ASSERT(! m_aNextRecord);
 
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
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void IoChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  IoChunk* pIoChunk = _pIoChunk(pObject);
  
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
void IoChunk::vOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool b = m_xFile.bOpen(C_OPEN_READ);
  RASSERT(b);
  
  // set buffer
  //m_xFile.vBuffer(C_1MB);
  
  // set no buffering
  m_xFile.vBuffer(0);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
bool IoChunk::bReadChunk(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  m_nBytesRead = m_xFile.iReadBytes(m_xWsp.aArea(),nBytes);

  m_aEndChunk = &m_xWsp.aStr()[m_nBytesRead - 1];
  
  if (m_nBytesRead)
    return true;
  else
    return false;
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
