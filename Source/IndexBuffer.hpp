/*============================================================================*/
/* IndexBuffer.hpp -- Index Buffer                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class IndexBuffer : public Buffer
  {
  private:
    Object*      m_pFreeSpace;  // beggining of free space within buffer
    i8           m_nEntries;    // number of entries in this buffer
    IndexBuffer* m_pPrevBuffer; // prev buffer
    IndexBuffer* m_pNextBuffer; // next buffer
    // ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* pAddress);
    void  operator delete(void*);
    
    IndexBuffer(void);
    IndexBuffer(i8 iDataBytes);
    ~IndexBuffer(void);
    
    static IndexBuffer* pNewIndexBuffer(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vAddEntry(Entry& xEntry);
    void vAppendBuffer(IndexBuffer* pArgBuffer);
    void vCopyBuffer(IndexBuffer* pFromBuffer);
    i8   nEntries(void);
    void vEntryCount(void);
    void vShow(void);
  
    Entry* pFirstEntry();
    Entry* pNextEntry(Entry* pCurr);
    
    IndexBuffer* pNextBuffer(void);
    void         vNextBuffer(IndexBuffer* pNextBuffer);
    
    IndexBuffer* pPrevBuffer(void);
    void         vPrevBuffer(IndexBuffer* pPrevBuffer);
    
    i8       nBytesUsed(void);
    Object*  pDataArea(void);
    i8       nFreeBytes(void);
  };
  
  IndexBuffer* _pIndexBuffer(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
