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
  
  m_nCrv = 0;
  m_tMaxPsp = C_UNKNOWN;
  
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
/* get crv given buffer number and crv subscript                              */
/*----------------------------------------------------------------------------*/
Crv* CrvBuffer::aToCrv(i8 iBufferNo,i8 i_Crv)
{
  CrvBuffer* pCrvBuffer = aToCrvBuffer(iBufferNo);
  
  Crv* aCrv = pCrvBuffer->aGetCrv(i_Crv);
  
  return aCrv;
}

/*----------------------------------------------------------------------------*/
/* get crv buffer given buffer no                                             */
/*----------------------------------------------------------------------------*/
CrvBuffer* CrvBuffer::aToCrvBuffer(i8 i_Buffer)
{
  CrvBuffer* pCrvBuffer = _pCrvBuffer(AM::pToLargeObect(i_Buffer));

  return pCrvBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add crw version into this buffer, return crv item subscript                */
/*----------------------------------------------------------------------------*/
i8 CrvBuffer::iAddCrwVersion(Rrf xwRef, Crw* pCrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nCrwBytes = m_pStreamDef->m_nFixedSized;
  i8 nTotalBytesNeeded = Crv::nCrvBytesNeed(nCrwBytes);
  
  if (! bHasSpace(nTotalBytesNeeded)) {
    //vDumpAllCrv();
    return C_UNKNOWN;
  }
   
  Crv* aCrv = _aCrv(m_xWsp.ayAllot(nTotalBytesNeeded));
  ASSERT(aCrv);
  
  t8 tCrwPsp = pCrw->tPerspective();

  if (tCrwPsp > m_tMaxPsp)
    m_tMaxPsp = tCrwPsp;
  
  m_nCrv++;
  
  aCrv->vTakeVersion(xwRef,pCrw,nCrwBytes);
  
  return m_nCrv - 1;
}

/*----------------------------------------------------------------------------*/
/* get number of crv's in this buffer                                         */
/*----------------------------------------------------------------------------*/
i8 CrvBuffer::nCrv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  return m_nCrv;
}

/*----------------------------------------------------------------------------*/
/* dump all crv's in this buffer                                             */
/*----------------------------------------------------------------------------*/
void CrvBuffer::vDumpAllCrv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
    
  
  HV_RowSingle hvRowSingle(m_pStreamDef);
  HV_Record hvRecord(m_pStreamDef);

  for (i8 i_Crv = 0; i_Crv < m_nCrv; i_Crv++) {
    
    Crv* aCrv = aGetCrv(i_Crv);
  
    Crw* aCrw = aCrv->aCrw();
    CrwBuffer::vToRow(hvRowSingle,aCrw);
    
    vTrace("Crv %lld Rrf %lld.%lld Psp %lld: %s\n"_Stz,
           i_Crv,
           aCrv->xwRef().iBuffer(),
           aCrv->xwRef().iItem(),
           aCrw->tPerspective(),
           hvRecord->sz(hvRowSingle->pRowCurrent()));

    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* get crv for given subscript                                                */
/*----------------------------------------------------------------------------*/
Crv* CrvBuffer::aGetCrv(i8 i_Crv)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  i8 nCrwBytes = m_pStreamDef->m_nFixedSized;
  i8 nSizeCrv  = Crv::nCrvBytesNeed(nCrwBytes);
  
  i8 i_Offset  = nSizeCrv * i_Crv;
  
  Crv* aCrv = aAdd<Crv*>(m_xWsp.ayArea(),i_Offset);

  return aCrv;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
CrvBuffer* CrvBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  return _pCrvBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
CrvBuffer* CrvBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  return _pCrvBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if buffer has space for given number of bytes                         */
/*----------------------------------------------------------------------------*/
bool CrvBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRVBUFFER,_FOOTER_);
  
  if (m_xWsp.nFree() >= nBytes)
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
