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
    
    Object*    pAllocObject(size_t nBytes);
    Row*       pAllocRow(size_t nBytes);
    
    Object*    pFirstObject(void);
    Row*       pFirstRow(void);
    Object*    pNextObject(Object* pCurr, i8 iBytes);
    Row*       pNextRow(Row* pCurr);
    
    RowBuffer* pNextBuffer(void);
    void       vNextBuffer(RowBuffer* pNextBuffer);
    
    RowBuffer* pPrevBuffer(void);
    void       vPrevBuffer(RowBuffer* pPrevBuffer);
    
    i8         iBytesUsed(void);
    void       vCopyBuffer(RowBuffer* pFromBuffer);
    bool       bHasSpace(void);
    
    bool       bInsertRow(StrVal* pRowStr);
    bool       bInsertRow2(StrVal* pRowStr);
    
  private:
    Object* pDataArea(void);
    Footer* pFooter(void);
    i8      iFreeSpace(void);
    bool    bIsExhausted(void);
    void    vSetExhausted(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/