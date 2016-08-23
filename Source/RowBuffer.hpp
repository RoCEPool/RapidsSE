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
    bool     m_bHasSpace;         // this buffer has more space?
    Object*  m_pFreeSpace;        // ptr to free space within this buffer
    RowBuffer* m_pPrevBuffer; // prev buffer
    RowBuffer* m_pNextBuffer; // next buffer
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowBuffer(void);
    ~RowBuffer(void);
    
    Object* pAllocRow(size_t nBytes);
    
    Object* pFirst(void);
    Object* pNext(Object* pCurr, i8 iBytes);
    
    RowBuffer* pNextBuffer(void);
    void       vNextBuffer(RowBuffer* pNextBuffer);
    
    RowBuffer* pPrevBuffer(void);
    void       vPrevBuffer(RowBuffer* pPrevBuffer);
    
    void vCopy(RowBuffer* pFromBuffer);
    bool bHasSpace(void);
    i8   iBytesUsed(void);
    Object* pFirstObject(void);
    
    bool    bInsertRow(StrVal* pRowStr);
    
  private:
    Footer* pFooter(void);
    i8      iFreeSpace(void);
    bool    bIsExhausted(void);
    void    vSetExhausted(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/