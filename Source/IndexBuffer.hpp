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
    i8           m_nEntries;    // number of entries in this buffer
    IndexBuffer* m_pPrevBuffer; // prev buffer
    IndexBuffer* m_pNextBuffer; // next buffer
    char*        m_pFreeSpace;  // beggining of free space within buffer
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexBuffer(void);
    ~IndexBuffer(void);
    
    static IndexBuffer* pNewIndexBuffer(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void  vAppendBuffer(IndexBuffer* pArgBuffer);
    void  vCopyBuffer(IndexBuffer* pFromBuffer);
    i8    nEntries(void);
    void  vEntryCount(void);
    void  vInsertEntry(EntWsp& xEntWsp);
    bool  bInsertEntry(EntWsp& xEntWsp);
    IndexBuffer* pFindBuffer(EntWsp& xEntWsp);
    void  vShow(void);
    void  vSplitBuffer(void);
    void  vVerify(void);
  
    Entry* pFirstEntry();
    Entry* pNextEntry(Entry* pCurr);
    
    IndexBuffer* pNextBuffer(void);
    void         vNextBuffer(IndexBuffer* pNextBuffer);
    
    IndexBuffer* pPrevBuffer(void);
    void         vPrevBuffer(IndexBuffer* pPrevBuffer);
    
    i8    nBytesUsed(void);
    char* pDataArea(void);
    i8    nFreeBytes(void);
  };
  
  IndexBuffer* _pIndexBuffer(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
