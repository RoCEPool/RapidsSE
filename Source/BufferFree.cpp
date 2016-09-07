/*============================================================================*/
/*  BufferFree.cpp - buffer when its in the buffer pool free chain            */
/*============================================================================*/

#include "Pool.h"
  
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* constructor -- never called                                                */
/*----------------------------------------------------------------------------*/
BufferFree::BufferFree(void)
: Buffer(CN_BUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER);
  
  Memory::vCheckInitialised(this,sizeof(BufferFree));
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* destructor -- never called                                                 */
/*----------------------------------------------------------------------------*/
BufferFree::~BufferFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER);
  
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* get next free buffer                                                       */
/*----------------------------------------------------------------------------*/
bn BufferFree::nNextFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free buffer                                                       */
/*----------------------------------------------------------------------------*/
void BufferFree::vNextFree(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nNextFree = nBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
