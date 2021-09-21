/*============================================================================*/
/* PtnStm_cursor.cpp -- Partition Stream                                      */
/*============================================================================*/

#include "BASE.h"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"

#include "TablePtn.hpp"
#include "Table.hpp"
#include "PtnStm_cursor.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PtnStm_cursor,CN_PTNSTMCURSOR)
__aNewFunction(PtnStm_cursor,CN_PTNSTMCURSOR)
__vDeleteFunction(PtnStm_cursor,CN_PTNSTMCURSOR)
__aFooterFunction(PtnStm_cursor)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnStm_cursor::PtnStm_cursor(TablePtn* pTablePtn)
: Object(CN_PTNSTMCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
  
  m_pTablePtn    = pTablePtn;

  m_iLglBufferNo = -1; // correct

  m_pCurBuffer = nullptr;
  m_pCurBkt    = nullptr;

  m_tCurPtnNo = -1;
  m_iCurBktNo = -1;





  _checkInitialised_(CN_PTNSTMCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PtnStm_cursor::~PtnStm_cursor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ptnStream                                                       */
/*----------------------------------------------------------------------------*/
void PtnStm_cursor::vRepair(Object* pObject, eRF eRepair)
{
  _static_

  PtnStm_cursor* pPtnStm_cursor = _pPtnStm_cursor(pObject);

  // delete the ptnStream
  delete pPtnStm_cursor;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current buffer no                                                      */
/*----------------------------------------------------------------------------*/
i8 PtnStm_cursor::iBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
  vTrace("iBufferNo\n"_Stz);
  return m_iLglBufferNo;
}

/*----------------------------------------------------------------------------*/
/* close                                                                      */
/*----------------------------------------------------------------------------*/
void PtnStm_cursor::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);

  m_pCurBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* open                                                                       */
/*----------------------------------------------------------------------------*/
bool PtnStm_cursor::bOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
  
  vReset();
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void PtnStm_cursor::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);
  vTrace("PtnStm_cursor vReset\n"_Stz);

  // decrypting the partition ID into the partition number
  i8 tPtnNo = AM::PTNMAP::tPtnIDToPtnNo(m_pTablePtn->tPtnId().iAsI8());

  vTrace("m_pTablePtn->tPtnNo():%lld \n"_Stz, m_pTablePtn->tPtnId().iAsI8());
  //vTrace("m_pTablePtn->tMappedPtnNo():%lld reverted Ptn:%lld\n"_Stz, m_pTablePtn->tMappedPtnNo().iAsI8(), tPtnNo);

  m_tCurPtnNo = tPtnNo; //m_pTablePtn->pRootBuffer()->tPtnNo(); // m_pTablePtn->tPtnNo()

  m_iCurBktNo = m_pTablePtn->pTable()->iPtnToBkt(m_tCurPtnNo);

  vTrace("m_iCurBktNo:%lld\n"_Stz,m_iCurBktNo );

  m_pCurBkt = m_pTablePtn->pGetBucket(m_iCurBktNo);
  vTrace("To read from m_pCurBkt:%p\n"_Stz,m_pCurBkt);

  if (m_pCurBkt) {
    m_pCurBuffer = m_pCurBkt->pRootBuffer();
  }
  else
    m_pCurBuffer = nullptr;
  //////

  m_iLglBufferNo = -1; // correct
}


/*----------------------------------------------------------------------------*/
/* read next buffer                                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* PtnStm_cursor::pRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);

  vTrace("PtnStm_cursor pRead\n"_Stz);

#ifdef WITH_GID_PTNID
  if (!m_pCurBuffer)
    return nullptr;

  DataBuffer *pReadBuffer = m_pCurBuffer;

  m_pCurBuffer = _pDataBuffer(m_pCurBuffer->pNextLinkedBuffer());

  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  return pReadBuffer;
#else

  if (!m_pCurBuffer) {
    // Do we have another bucket for this partition
    i8 iCurBktNo = m_pTablePtn->pTable()->iPtnToBktNext(m_tCurPtnNo, m_iCurBktNo);
    if (iCurBktNo == -1) {
      return nullptr;
    }

    vTrace(" m_iCurBktNo:%lld   new m_iCurBktNo:%lld\n"_Stz, m_iCurBktNo, iCurBktNo);
    m_iCurBktNo = iCurBktNo;

    m_pCurBkt = m_pTablePtn->pGetBucket(m_iCurBktNo);
    m_pCurBuffer = m_pCurBkt->pRootBuffer();
    if (!m_pCurBuffer)
      return nullptr;
  }

  DataBuffer *pReadBuffer = m_pCurBuffer;

  m_pCurBuffer = _pDataBuffer(m_pCurBuffer->pNextLinkedBuffer());

  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  return pReadBuffer;
#endif
}



/*----------------------------------------------------------------------------*/
/* seek                                                                       */
/*----------------------------------------------------------------------------*/
bool PtnStm_cursor::bSeek(i8 iBufferNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PTNSTMCURSOR, _FOOTER_);

  vTrace("bSeek with BufferNo:%lld   m_iLglBufferNo:%lld\n"_Stz, iBufferNo, m_iLglBufferNo);
  if (iBufferNo < m_iLglBufferNo)
    vReset();

  while (m_iLglBufferNo < iBufferNo) {
    m_pCurBuffer = _pDataBuffer(m_pCurBuffer->pNextLinkedBuffer());
    if (!m_pCurBuffer)
      return false;
    m_iLglBufferNo++;
  }

  if (iBufferNo == m_iLglBufferNo)
    return true;
  else
    return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
