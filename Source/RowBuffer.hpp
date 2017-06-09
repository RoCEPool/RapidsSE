/*============================================================================*/
/* RowBuffer.hpp -- Entry Buffer                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RowBuffer : public EntryBuffer
  {
  private:
  
  public:
  
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
  
    Entry*       pAllocRow(i8 nDataBytes);
    void       vAppendBuffer(RowBuffer* pSrcBuffer);
    void       vCopyBuffer(RowBuffer* pSrcBuffer);
    byte*      aDataArea(void);
    Entry*       pFirstRow(void);
    i8         nFreeBytes(void);
    bool       bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength, Entry** ppRow);
    Entry*       pNextRow(Entry* pCurrRow);
    void       vRowCount(void);
    i8         nUsedBytes(void);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
