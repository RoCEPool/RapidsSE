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
CrvBuffer::CrvBuffer(StreamDef* pStreamDef)
: DataBuffer(CN_CRVBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
    
  m_pStreamDef = pStreamDef;
  
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
void CrvBuffer::vAddCrwVersion(Ref xwRef, Crw* pCrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nCrwBytes = m_pStreamDef->m_nFixedSized;
  i8 nTotalBytesNeeded = Crv::nCrvBytesNeed(nCrwBytes);
  
  byte* ayCrv = m_xWsp.ayAllot(nTotalBytesNeeded);
  ASSERT(ayCrv);
}

/*----------------------------------------------------------------------------*/
/* test if buffer has space for given number of bytes                         */
/*----------------------------------------------------------------------------*/
bool CrvBuffer::bSpaceForCrv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nCrwBytes = m_pStreamDef->m_nFixedSized;
  i8 nTotalBytesNeeded = Crv::nCrvBytesNeed(nCrwBytes);

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
