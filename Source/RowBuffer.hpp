/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowBuffer : public Buffer
  {
  private:
    bool       m_bHasSpace;   // this buffer has more space?
    Object*    m_pFreeSpace;  // ptr to free space within this buffer
    RowBuffer* m_pPrevBuffer; // prev buffer
    RowBuffer* m_pNextBuffer; // next buffer
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowBuffer(void);
    ~RowBuffer(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Row*       pAllocRow(i8 iDataBytes);
    
    Row*       pFirstRow(void);
    Row*       pNextRow(Row* pCurr);
    
    RowBuffer* pNextBuffer(void);
    void       vNextBuffer(RowBuffer* pNextBuffer);
    
    RowBuffer* pPrevBuffer(void);
    void       vPrevBuffer(RowBuffer* pPrevBuffer);
    
    i8         iBytesUsed(void);
    void       vCopyBuffer(RowBuffer* pFromBuffer);
    bool       bHasSpace(void);
    
    bool       bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength);
    
  private:
    Object* pDataArea(void);
    i8      iFreeSpace(void);
    bool    bIsExhausted(void);
    void    vSetExhausted(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/