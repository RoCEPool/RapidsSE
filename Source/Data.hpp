/*============================================================================*/
/* Data.hpp - data portion of table partition                                 */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Data : public Object
  {
  private:
    Lock       m_zLock;          // lock covers all buffers
    i8         m_nRowStrands;    // number of row strands
    RowStrand* m_pHeadStrand;    // first strand in list
    RowStrand* m_pTailStrand;    // last strand in list
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Data(void);
    ~Data(void);
  
    void vAppendBuffer(RowBuffer* pRowBuffer);
    bool bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength);
    
    RowStrand* pHeadStrand(void);
    RowStrand* pTailStrand(void);
  
    i8 nRowStrands(void);
  
    void vTruncate(void);  
  
  private:
    bool bExtendPartition(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
