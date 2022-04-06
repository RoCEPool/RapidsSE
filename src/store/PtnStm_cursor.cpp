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
  vTrace("iBufferNo:%lld\n"_Stz, m_iLglBufferNo);
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

  //m_pTablePtn->pTable()->vDumpTablePtn();
  //return ;

  m_tCurPtnNo = m_pTablePtn->tPtnId();

  // Which bucket has data, get the first one
  for (i8 i_Bucket = 0; i_Bucket < C_MAX_KEYBUCKETS; i_Bucket++) {
    Bucket *pBucket = m_pTablePtn->pGetBucket(i_Bucket);
    //vTrace("PtnStm_cursor::vReset i_Bucket:%lld nBuffers:%lld\n"_Stz, i_Bucket, pBucket->nBuffers());
    if (pBucket->nBuffers() > 0) {
      m_iCurBktNo = i_Bucket;
      m_pCurBkt = pBucket;
      break;
    }
  }

  //if (m_iCurBktNo >= 0) {
  //  vTrace("PtnStm_cursor::vReset To read from m_pCurBkt:%p bkt:%lld for Ptn:%lld\n"_Stz,m_pCurBkt, m_iCurBktNo, m_tCurPtnNo.iAsI8());
  //}

  if (m_pCurBkt) {
    m_pCurBuffer = m_pCurBkt->pRootBuffer();
  }
  else {
    m_pCurBuffer = nullptr;
  }

  m_iLglBufferNo = -1; // correct
}


/*----------------------------------------------------------------------------*/
/* read next buffer                                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* PtnStm_cursor::pRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTMCURSOR,_FOOTER_);

/* Original code
  if (!m_pCurBuffer)
    return nullptr;

  vTrace("PtnStm_cursor pRead Ptn %lld Bkt:%lld\n"_Stz, m_tCurPtnNo.iAsI8(), m_iCurBktNo);
  DataBuffer *pReadBuffer = m_pCurBuffer;

  m_pCurBuffer = _pDataBuffer(m_pCurBuffer->pNextLinkedBuffer());

  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  return pReadBuffer;
*/



  if (!m_pCurBuffer) {
    if (m_iCurBktNo >= 0) {
      // Do we have another bucket for this partition
      //m_pTablePtn->pTable()->iPtnToBktNext(m_tCurPtnNo, m_iCurBktNo);
      i8 iNxtBktNo = iGetNextBucket(m_iCurBktNo);

      if (iNxtBktNo == -1) {
        return nullptr;
      }

      m_iCurBktNo = iNxtBktNo;

      m_pCurBkt = m_pTablePtn->pGetBucket(m_iCurBktNo);
      m_pCurBuffer = m_pCurBkt->pRootBuffer();
    }

    if (!m_pCurBuffer) {
      m_iLglBufferNo = -1;
      return nullptr;
    }
  }

  //vTrace("PtnStm_cursor::pRead To read from m_pCurBkt:%p bkt:%lld for Ptn:%lld\n"_Stz,m_pCurBkt, m_iCurBktNo, m_tCurPtnNo.iAsI8());
  DataBuffer *pReadBuffer = m_pCurBuffer;

  m_pCurBuffer = _pDataBuffer(m_pCurBuffer->pNextLinkedBuffer());

  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  return pReadBuffer;

}


/*----------------------------------------------------------------------------*/
/* Get Next Bucket that has data                                              */
/*----------------------------------------------------------------------------*/
i8 PtnStm_cursor::iGetNextBucket(i8 iCurBktNo)
{
  i8 iNxtBktNo  = -1;
  for (i8 i_Bucket = iCurBktNo+1; i_Bucket < C_MAX_KEYBUCKETS; i_Bucket++) {
    Bucket *pBucket = m_pTablePtn->pGetBucket(i_Bucket);
    if (pBucket->nBuffers() > 0) {
      iNxtBktNo = i_Bucket;
      break;
    }
  }

  return iNxtBktNo;
}

/*----------------------------------------------------------------------------*/
/* seek                                                                       */
/*----------------------------------------------------------------------------*/
bool PtnStm_cursor::bSeek(i8 iBufferNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PTNSTMCURSOR, _FOOTER_);

  //vTrace("bSeek with BufferNo:%lld   m_iLglBufferNo:%lld\n"_Stz, iBufferNo, m_iLglBufferNo);
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
