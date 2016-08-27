/*============================================================================*/
/*  BufferPoolIter.cpp - Buffer Pool Iterator                                 */
/*============================================================================*/

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferPoolIter::BufferPoolIter(const BufferPool* pBufferPoolator,
                                     BufferFree*  pCurrentBuffer)
{
  _magic_();
  
  m_nClassNo        = CN_BUFFERPOOLITER;
  m_pBufferPoolator = pBufferPoolator;
  m_pCurrentBuffer  = pCurrentBuffer;
  
  ASSERT(m_nClassNo == CN_BUFFERPOOLITER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPoolIter::~BufferPoolIter(void)
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLITER);}

/*----------------------------------------------------------------------------*/
/* test position of iterator                                                  */
/*----------------------------------------------------------------------------*/
bool BufferPoolIter::operator !=(const BufferPoolIter& other) const
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLITER);
  return m_pCurrentBuffer != other.m_pCurrentBuffer;
}

/*----------------------------------------------------------------------------*/
/* get current buffer                                                         */
/*----------------------------------------------------------------------------*/
BufferFree* BufferPoolIter::operator *(void) const
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLITER);
  
  return pPoolCheck<BufferFree*>(m_pCurrentBuffer);
}

/*----------------------------------------------------------------------------*/
/* prefix increment iterator                                                  */
/*----------------------------------------------------------------------------*/
const BufferPoolIter& BufferPoolIter::operator ++(void)
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLITER);
  
  //bn nNextBufferNo = m_pBufferPoolator->nBufferNext(m_pCurrentBuffer->nBufferNo());
  bn nNextBufferNo = m_pCurrentBuffer->nBufferNo() + 1;
  
  m_pCurrentBuffer = m_pBufferPoolator->pToBufferFree(nNextBufferNo);
  
  return *this;
}

/*----------------------------------------------------------------------------*/
/* ASSERT class no                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolIter::vAssertClass(eCN eClassNo) const
{
  _magic_();
  
  ASSERT(m_nClassNo == eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/