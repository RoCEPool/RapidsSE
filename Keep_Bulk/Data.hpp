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
    i8         m_nEbStrands;    // number of row strands
    EbStrand* m_pHeadStrand;    // first strand in list
    EbStrand* m_pTailStrand;    // last strand in list
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    Data(void);
    ~Data(void);
  
    void vAppendBuffer(EntryBuffer* pRowBuffer);
    bool bInsertRow(TxRef* pTxRef, Stz* pRecord, i8 iLength);
    
    EbStrand* pHeadStrand(void);
    EbStrand* pTailStrand(void);
  
    i8 nEbStrands(void);
  
    void vTruncate(void);  
  
  private:
    bool bExtendPartition(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
