/*============================================================================*/
/* IndexStream.cpp -- Index Stream                                            */
/*============================================================================*/

#include "Partitioning.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Partitioning,IndexStream,CN_INDEXSTREAM)
__aNewFunction(IndexStream,CN_INDEXSTREAM)
__vDeleteFunction(IndexStream,CN_INDEXSTREAM)
__aFooterFunction(IndexStream)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexStream::IndexStream(TablePtn* pTablePtn)
: Object(CN_PTNSTREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  m_pTablePtn    = pTablePtn;
  m_pNextBuffer  = nullptr;
  m_iLglBufferNo = -1; // correct

  _checkInitialised_(CN_PTNSTREAM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexStream::~IndexStream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current buffer no                                                      */
/*----------------------------------------------------------------------------*/
i8 IndexStream::iBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
 
  return m_iLglBufferNo;
}

/*----------------------------------------------------------------------------*/
/* close                                                                      */
/*----------------------------------------------------------------------------*/
void IndexStream::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
 
  m_pNextBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* open                                                                       */
/*----------------------------------------------------------------------------*/
bool IndexStream::bOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  vReset();
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* read next buffer                                                           */
/*----------------------------------------------------------------------------*/
CrwBuffer* IndexStream::pRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  if (! m_pNextBuffer)
    return nullptr;

  CrwBuffer* pReadBuffer = m_pNextBuffer;
  
  m_pNextBuffer = m_pNextBuffer->pNextCrwBuffer();
  
  if (pReadBuffer)
    m_iLglBufferNo++;
  else
    m_iLglBufferNo = -1;

  
  return pReadBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void IndexStream::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
 
  m_pNextBuffer  = m_pTablePtn->pRootCrwBuffer();
  m_iLglBufferNo = -1; // correct
}

/*----------------------------------------------------------------------------*/
/* seek                                                                       */
/*----------------------------------------------------------------------------*/
bool IndexStream::bSeek(i8 iBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  if (iBufferNo < m_iLglBufferNo)
    vReset();
  
  while (m_iLglBufferNo < iBufferNo) {
    m_pNextBuffer = m_pNextBuffer->pNextCrwBuffer();
    if (! m_pNextBuffer)
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
