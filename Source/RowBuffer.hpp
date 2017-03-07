/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  static const i8 C_BUFFER_THRESHOLD = 100;
  static const RowBuffer* C_ROWBUFFER_MARKER = (RowBuffer*)0xFFFFFFFFFFFFFFFF;
  
  
  enum eRD // Row Buffer Durability
  {
  /* 0 */ RD_NONE=0,
  /* 1 */ RD_TEMPORARY,     // rb is temporary
  /* 2 */ RD_PERMANENT,     // rb is permanent
  /* 3 */ RD_ENUMS
  };
  
  static Str* aRD[]
  {
  /* 0 */ "NONE"_Str,
  /* 1 */ "TEMPORARY"_Str,
  /* 2 */ "PERMANENT"_Str,
  /* 3 */ "ENUMS"_Str,
  };
  
  class RowBuffer : public Buffer
  {
  private:
    eRD        m_eDurability; // row buffer durability
    Object*    m_pFreeSpace;  // ptr to free space within this buffer
    i8         m_nRows;       // number of rows in this buffer
    RowBuffer* m_pPrevBuffer; // prev buffer
    RowBuffer* m_pNextBuffer; // next buffer
    // ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
    
    RowBuffer(void);
    RowBuffer(i8 iDataBytes);
    ~RowBuffer(void);
    
    static RowBuffer* pNewRowBuffer(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    eRD        eDurability(void);
    void       vDurability(eRD eDurability);
  
    Row*       pAllocRow(i8 iDataBytes);

    void       vAppendBuffer(RowBuffer* pArgBuffer);
    
    Row*       pFirstRow(void);
    Row*       pNextRow(Row* pCurr);
    
    RowBuffer* pNextBuffer(void);
    void       vNextBuffer(RowBuffer* pNextBuffer);
    
    RowBuffer* pPrevBuffer(void);
    void       vPrevBuffer(RowBuffer* pPrevBuffer);
    
    i8         iBytesUsed(void);
    i8         nRows(void);
    void       vRowCount(void);

 
    void       vCopyBuffer(RowBuffer* pFromBuffer);
    
    bool       bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength);
    
    Object*    pDataArea(void);
    
  //private:
    
    i8      iFreeSpace(void);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
