/*============================================================================*/
/* CrvBuffer.cpp -- Compact Row Buffer                                        */
/*============================================================================*/

#include "ReadConsistency.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CrvBuffer* ReadConsistency::_pCrvBuffer(void* aAddress)
{
  CrvBuffer* pCrvBuffer = reinterpret_cast<CrvBuffer*>(aAddress);
  
  RASSERT_TYPE(pCrvBuffer,CN_CRVBUFFER);
  
  return pCrvBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new buffer from heap                                            */
/*----------------------------------------------------------------------------*/
void* CrvBuffer::operator new(size_t)
{
  CrvBuffer* pCrvBuffer =
    static_cast<CrvBuffer*>(HeapMem::pAllocate(CN_CRVBUFFER));
  
  return pCrvBuffer;
}

/*----------------------------------------------------------------------------*/
/* free a buffer back to the heap                                             */
/*----------------------------------------------------------------------------*/
void CrvBuffer::operator delete(void* aAddress)
{
  CrvBuffer* pCrvBuffer = static_cast<CrvBuffer*>(aAddress);
  
  HeapMem::vFree(pCrvBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrvBuffer::CrvBuffer(void)
: DataBuffer(CN_CRVBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
    
  #ifdef WITH_CHECK_INITIALISED
    m_xWsp.vZero(CN_CRVBUFFER);
  #endif

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_CRVBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrvBuffer::~CrvBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add crw version into this buffer                                           */
/*----------------------------------------------------------------------------*/
void CrvBuffer::vAddCrwVersion(Ref xwRef, Crw* pCrw, StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nTotalBytesNeeded = pStreamDef->m_nFixedSized + /*lets say*/64/*for them moment*/;
  
  byte* ayCrv = m_xWsp.ayAllot(nTotalBytesNeeded);
  ASSERT(ayCrv);
}

/*----------------------------------------------------------------------------*/
/* test if buffer has space for given number of bytes                         */
/*----------------------------------------------------------------------------*/
bool CrvBuffer::bSpaceFor(i8 nFixedCrwSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nTotalBytesNeeded = nFixedCrwSize + /*lets say*/64/*for them moment*/;
  if (m_xWsp.nFree() >= nTotalBytesNeeded)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(CrvBuffer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
