/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ROWBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: Buffer(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  //m_xWsp
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(i8 iDataBytes)
: Buffer(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_xWsp.vUsed(iDataBytes);
  
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  //m_xWsp
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(eCN eClassNo)
: Buffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_eDurability = BD_TEMPORARY;
  m_nEntries    = 0;
  
  m_pPrevBuffer  = nullptr;
  m_pNextBuffer  = nullptr;
  m_pHigherLevel = nullptr;
  
  m_nBufferLevel = 0;
  m_pIndex       = nullptr;
  
  _checkInitialised_(eClassNo);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* get number of enteries in the buffer                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vNextBuffer(EntryBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPrevBuffer(EntryBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
