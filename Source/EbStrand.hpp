/*============================================================================*/
/* EbStrand.hpp -- Entry Buffer Strand                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EbStrand : public Object
  {
  private:
    static const i8 C_STRAND_LENGTH = 16;
    
    Lock         m_zLock;          // lock on this strand (all buffers)
    i8           m_nEntryBuffers;  // number of buffers in this strand
    EntryBuffer* m_pFirstBuffer;   // first buffer in this strand
    EntryBuffer* m_pLastBuffer;    // last buffer in this strand
    
    EbStrand*    m_pPrevStrand;    // prev strand
    EbStrand*    m_pNextStrand;    // next strand
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    EbStrand(void);
    ~EbStrand(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bAppendBuffer(EntryBuffer* pEntryBuffer);
    bool bInsertRow(TxRef* pTxRef, Stz* pRecord, i8 iLength);
    i8   nRows(void);
    i8   nEntryBuffers(void);
    
    EntryBuffer* pFirstBuffer(void);
    EntryBuffer* pLastBuffer(void);
    
    EbStrand* pNextStrand(void);
    void      vNextStrand(EbStrand* pNextStrand);
    
    EbStrand* pPrevStrand(void);
    void      vPrevStrand(EbStrand* pPrevStrand);
    
    void vTruncate(void);
    
  private:
    bool    bExtendStrand(void);
    bool    bExtendStrand(EntryBuffer* pRowBuffer);
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
