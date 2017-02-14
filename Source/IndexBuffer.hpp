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
    Object*    m_pFreeSpace;  // ptr to free space within this buffer
    i8         m_nRows;       // number of rows in this buffer
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
      
    Row*       pAllocRow(i8 iDataBytes);

    void       vAppendBuffer(IndexBuffer* pArgBuffer);
    
    Row*       pFirstRow(void);
    Row*       pNextRow(Row* pCurr);
    
    IndexBuffer* pNextBuffer(void);
    void       vNextBuffer(IndexBuffer* pNextBuffer);
    
    IndexBuffer* pPrevBuffer(void);
    void       vPrevBuffer(IndexBuffer* pPrevBuffer);
    
    i8         iBytesUsed(void);
    i8         nRows(void);
    void       vRowCount(void);

 
    void       vCopyBuffer(IndexBuffer* pFromBuffer);
    
    bool       bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength);
    
    Object*    pDataArea(void);
    
  //private:
    
    i8      iFreeSpace(void);
  };
  
  IndexBuffer* _pIndexBuffer(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
