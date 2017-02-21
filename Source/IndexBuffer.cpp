/*============================================================================*/
/* IndexBuffer.cpp -- Index Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexBuffer* DBobjects::_pIndexBuffer(void* pAddr)
{
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pAddr);
  
  RASSERT( !pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass() );
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* IndexBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_INDEXBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* IndexBuffer::operator new(size_t iSize, void* pAddr)
{
  RASSERT(iSize);
  
  return pAddr;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void IndexBuffer::operator delete(void* pIndexBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pIndexBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBuffer::IndexBuffer(void)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  m_pFreeSpace  = pDataArea();
  m_nEntries    = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  _checkInitialised_(CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBuffer::IndexBuffer(i8 iDataBytes)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pFreeSpace  = static_cast<Object*>(pvAdd(pDataArea(),iDataBytes));
  m_nEntries    = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexBuffer::~IndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* index buffer factory                                                       */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pNewIndexBuffer(void)
{
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(
                            Heap::pAllocate(CN_INDEXBUFFER));
  if (! pIndexBuffer)
    return nullptr;
  
  new(pIndexBuffer) IndexBuffer;
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* dump index buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_INDEXBUFFER);
  ASSERT(pObject->eClass() == CN_INDEXBUFFER);
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pObject);
  
  vTrace("entries %lld freeSpace %p prevBuffer %p nextBuffer %p\n",
         pIndexBuffer->m_nEntries,
         pIndexBuffer->m_pFreeSpace,
         pIndexBuffer->m_pPrevBuffer,
         pIndexBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an entry into the index buffer                                         */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAddEntry(Entry& xEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // Copy Entry into IndexBuffer's DataBytes
  void* pDst = pvAdd(pDataArea(),nBytesUsed());
  void* pSrc = &xEntry;
  i8 nBytes  = xEntry.iSize();
  
  // test sufficient free space
//  if (nFreeBytes() < iBytes) {
//    return nullptr;
//  }
  
  Memory::vCopy(pDst,pSrc,nBytes);
  
  // calculate beginning of free space
  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,nBytes));
  
  // inc the number of entries in this buffer
  m_nEntries++;
}

/*----------------------------------------------------------------------------*/
/* append entries of arg buffer into this buffer                              */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAppendBuffer(IndexBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // copy arg entries into this buffer
  void* pDst = pvAdd(pDataArea(),nBytesUsed());
  void* pSrc = pArgBuffer->pDataArea();
  i8 nBytes  = pArgBuffer->nBytesUsed();
  Memory::vCopy(pDst,pSrc,nBytes);
  
  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,nBytes));
  
  // inc the number of entries in this buffer
  i8 nArgEntries = pArgBuffer->nEntries();
  m_nEntries += nArgEntries;
}

/*----------------------------------------------------------------------------*/
/* first entry                                                                */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pFirst = reinterpret_cast<Entry*>(pDataArea());
  
  if ((char*)pFirst < (char*)m_pFreeSpace)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* next entry                                                                 */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pNextEntry(Entry* pCurr)
{
  i8 iSize = pCurr->iSize();
  
  Entry* pNext = static_cast<Entry*>(pvAdd(pCurr,iSize));
  
  if ((char*)pNext < (char*)m_pFreeSpace)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vNextBuffer(IndexBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vPrevBuffer(IndexBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* calc bytes used                                                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nBytesUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = iSub(m_pFreeSpace,pDataArea());
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of entries in buffer                                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vShow(void)
{
  vTrace("IndexBuffer %p: entries %lld\n",
         this,
         m_nEntries
         );
}

/*----------------------------------------------------------------------------*/
/* set entry count                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vEntryCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_nEntries = 0;
   for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry))
     m_nEntries++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vCopyBuffer(IndexBuffer* pFromBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = pFromBuffer->nBytesUsed();
  
  Memory::vCopy(pDataArea(),pFromBuffer->pDataArea(),nBytes);
  
  char* pRevised = pcAdd(pDataArea(),nBytes);
  
  m_pFreeSpace = reinterpret_cast<Object*>(pRevised);

  vEntryCount();
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
Object* IndexBuffer::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Object* pStart = static_cast<Object*>(pvAdd(this,sizeof(IndexBuffer)));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in data area                           */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = iSub(m_pLastByte,m_pFreeSpace);

  nBytes -= sizeof(Footer);
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
