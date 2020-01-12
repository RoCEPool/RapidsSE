/*============================================================================*/
/* SrtBuffer.hpp -- Reference Buffer                                          */
/*============================================================================*/
#pragma once

namespace SortBuffers
{
  class SrtBuffer : public DataBuffer,
                    public MiniHeap_t<SrtSet>
  {
  protected:
  
    i8    m_nRowRefs;
    i8    m_nSrtSetBytes;
    bool  m_bIsLast;      // is this the last buffer in its sequence?
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SrtBuffer(i8 nRowRefs, i8 iPartition);
    ~SrtBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    SrtSet*    aAllotSrtSet(void);
    SrtBuffer* aCopySrtSet(SrtSet* aSrtSet);
    
    i8         nBufferCount(void);
    SrtSet*    aFirstSrtSet(void);
    void       vGetKeys(StreamDef* pStreamDef, i8 iKeyCol, i8 iColVec);
    void       vIsLast(bool bIsLast);
    bool       bIsLast(void);
    i8         nItemCount(void);
    void       vLinkBuffer(SrtBuffer* pSrtBuffer);
    SrtBuffer* pNextBuffer(void);
    SrtSet*    aNextSrtSet(SrtSet* aPriorSrtSet);
    SrtBuffer* pPrevBuffer(void);
    i8         nRowRefs(void);
    void       vPrintRows(StreamDef* pStreamDef);
    void       vShow(void);
    void       vShow(StreamDef* pStreamDef);
    SrtSet*    aSrtSet(i8 iSrtSet);
    void       vSwap(i8 iSrtSet1, i8 iSrtSet2);
    void       vSwap(i8 iSrtSet1, SrtBuffer* aSrtBuffer2, i8 iSrtSet2);
    bool       bVerifyNextPrev(void);
    bool       bVerifyOrder(void);
    
  private:
    Footer* aFooter(void);
  };
  
  SrtBuffer* _pSrtBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
