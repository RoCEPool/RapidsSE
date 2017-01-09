/*============================================================================*/
/* BufferFree.cpp -- buffer when its in the buffer pool free chain            */
/*============================================================================*/

#include "Pool.h"
  
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* constructor -- never called                                                */
/*----------------------------------------------------------------------------*/
BufferFree::BufferFree(void)
: Buffer(CN_BUFFER) // correct
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER,_FOOTER_);
  
  _checkInitialised_(CN_BUFFER);
  
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* destructor -- never called                                                 */
/*----------------------------------------------------------------------------*/
BufferFree::~BufferFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER,_FOOTER_);
  
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* get next free buffer                                                       */
/*----------------------------------------------------------------------------*/
bn BufferFree::nNextFree(void)
{
  if (CN_BUFFER != eClass())
    int debug = 1;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER,_FOOTER_);
  
  return m_nNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free buffer                                                       */
/*----------------------------------------------------------------------------*/
void BufferFree::vNextFree(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFER,_FOOTER_);
  
  m_nNextFree = nBuffer;
}

/*----------------------------------------------------------------------------*/
/* set footer                                                                 */
/*----------------------------------------------------------------------------*/
void BufferFree::vSetFooter(void)
{
  _setFooter_();
}

/*----------------------------------------------------------------------------*/
/* zero data                                                                  */
/*----------------------------------------------------------------------------*/
void BufferFree::vZeroData(void)
{
  char* pData  = pcAdd(this,sizeof(BufferFree));
  i8    nBytes = C_SIZE_POOL_BUFFER - sizeof(BufferFree) - sizeof(Footer);
  
  Memory::vSet(pData,0xEF,nBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
