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

  m_bStatistics = false;
  m_iReportQuantum = 0;
  
  //m_xElapsedTimer
  m_nBytesSoFar = 0;
  m_nNextReport = 0;
  
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
  
  if (m_bStatistics) {
    vReportStatistics();
    vExtrapolatedReport();
  }
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* report on stats extroplated to a day                                       */
/*----------------------------------------------------------------------------*/
void IoChunk::vExtrapolatedReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  i8 iAvgBytesPerSec = DIV(m_nBytesSoFar,m_xElapsedTimer.iSeconds());
  
  s32 sAvgMag;
  Magnitudes::vByteMag(iAvgBytesPerSec,2,sAvgMag);
  
  i8 iExtrapolatedPerDay = iAvgBytesPerSec * 60 * 60 * 24;
  Magnitudes::vByteMag(iExtrapolatedPerDay,2,sAvgMag);
  
  vTrace("Extrapolated %s/day\n"_Stz,
         sAvgMag.aStr());
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
  
  m_xElapsedTimer.vStart();
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
/* test for reporting                                                         */
/*----------------------------------------------------------------------------*/
void IoChunk::vPossiblyReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  if (! m_bStatistics)
    return;
  
  if (m_nBytesSoFar < m_nNextReport)
    return;
  
  vReportStatistics();
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
  
  m_nBytesSoFar += m_nDataBytes;
  
  vPossiblyReport();
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* report statistics                                                          */
/*----------------------------------------------------------------------------*/
void IoChunk::vReportStatistics(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  s32 sAvgMag;
  s32 sSumMag;

  m_xElapsedTimer.vUpdate();
  
  i8 iAvgBytesPerSec = DIV(m_nBytesSoFar,m_xElapsedTimer.iSeconds());
  
  Magnitudes::vByteMag(iAvgBytesPerSec,2,sAvgMag);
  Magnitudes::vByteMag(m_nBytesSoFar,2,sSumMag);
  
  vTrace("%s %llds: %lls sofar; %s/s\n"_Stz,
         m_sReportPrefix.aStr(),
         m_xElapsedTimer.iSeconds(),
         sSumMag.aStr(),
         sAvgMag.aStr());

  m_nNextReport += m_iReportQuantum;
}

/*----------------------------------------------------------------------------*/
/* set reporting                                                              */
/*----------------------------------------------------------------------------*/
void IoChunk::vSetReporting(Stz* sPrefix, i8 iReportQuantum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCHUNK,_FOOTER_);
  
  m_bStatistics    = true;
  m_sReportPrefix  = sPrefix;
  m_iReportQuantum = iReportQuantum;
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
