/*============================================================================*/
/* PtnStream.cpp -- Partition Stream                                          */
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
#include "PtnStm_cursor.hpp"
#include "PtnStream.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PtnStream,CN_PTNSTREAM)
__aNewFunction(PtnStream,CN_PTNSTREAM)
__vDeleteFunction(PtnStream,CN_PTNSTREAM)
__aFooterFunction(PtnStream)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnStream::PtnStream(TablePtn* pTablePtn)
: Object(CN_PTNSTREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);
  
  m_pTablePtn    = pTablePtn;
  m_pNextBuffer  = nullptr;
  m_iLglBufferNo = -1; // correct

  vTrace("PtnStream for partition:%lld\n"_Stz, pTablePtn->tPtnId().iAsI8());
  m_pPtnStm_cursor = new PtnStm_cursor(pTablePtn);

  _checkInitialised_(CN_PTNSTREAM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PtnStream::~PtnStream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);
  delete m_pPtnStm_cursor;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ptnStream                                                       */
/*----------------------------------------------------------------------------*/
void PtnStream::vRepair(Object* pObject)
{
  _static_

  PtnStream* pPtnStream = _pPtnStream(pObject);

  // delete the ptnStream
  delete pPtnStream;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current buffer no                                                      */
/*----------------------------------------------------------------------------*/
i8 PtnStream::iBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);


  return m_pPtnStm_cursor->iBufferNo();

  //return m_iLglBufferNo;


}

/*----------------------------------------------------------------------------*/
/* close                                                                      */
/*----------------------------------------------------------------------------*/
void PtnStream::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);

  m_pPtnStm_cursor->vClose();

  //m_pNextBuffer = nullptr;

}

/*----------------------------------------------------------------------------*/
/* open                                                                       */
/*----------------------------------------------------------------------------*/
bool PtnStream::bOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);


  return m_pPtnStm_cursor->bOpen();

  //vReset();
  //return true;

}

/*----------------------------------------------------------------------------*/
/* read next buffer                                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* PtnStream::pRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);


  return m_pPtnStm_cursor->pRead();

/*
  if (!m_pNextBuffer)
    return nullptr;

  DataBuffer *pReadBuffer = m_pNextBuffer;

  m_pNextBuffer = _pDataBuffer(m_pNextBuffer->pNextLinkedBuffer());

  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  return pReadBuffer;
*/
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void PtnStream::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNSTREAM,_FOOTER_);


  return m_pPtnStm_cursor->vReset();

  //m_pNextBuffer  = m_pTablePtn->pRootBuffer();
  //m_iLglBufferNo = -1; // correct

}

/*----------------------------------------------------------------------------*/
/* seek                                                                       */
/*----------------------------------------------------------------------------*/
bool PtnStream::bSeek(i8 iBufferNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PTNSTREAM, _FOOTER_);


  return m_pPtnStm_cursor->bSeek(iBufferNo);

/*
  if (iBufferNo < m_iLglBufferNo)
    vReset();

  while (m_iLglBufferNo < iBufferNo) {
    m_pNextBuffer = _pDataBuffer(m_pNextBuffer->pNextLinkedBuffer());
    if (!m_pNextBuffer)
      return false;
    m_iLglBufferNo++;
  }

  if (iBufferNo == m_iLglBufferNo)
    return true;
  else
    return false;
*/
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
