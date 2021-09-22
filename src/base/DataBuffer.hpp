/*============================================================================*/
/* DataBuffer.hpp -- Rows Buffer                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DataBuffer : public LinkedBuffer
  {
  protected:
    Lock   m_okLock;        // lock
    i8     m_iBufferNo;     // this DataBuffer's buffer number
#ifdef WITH_PTNID
    PtnId  m_tPtnNo;
#else
    tPN    m_tPtnNo;        // partition no this buffer belongs to
#endif
    i8     m_iBucketNo;     // this DataBuffer's bucket number

    i8     m_iBufferHeight; // buffer height in the tree
    Birth  m_oWhenNewed;    // when was this buffer created?
    b8     m_bIsPermanent;  // is buffer permanent ?
  
    Table*     m_pTable;
    StreamDef* m_pStreamDef;

    tXN m_tTxnNo; // transaction number

    PadWord m__PadToCacheLine[18];

  public:
    DataBuffer(eCN eClass, bool bIsPermanent);
   ~DataBuffer(void) noexcept(false);

    static DataBuffer* pRefdDataBuffer(Rrf& xRef);
    static void        vRepairList(Object* pObject);
    static DataBuffer* pToDataBuffer(void* aAddr);

    void       vStreamDef(StreamDef* pStreamDef);
    StreamDef* pStreamDef(void);

    Table* pTable(void);
    void   vTable(Table* pTable);

    tXN    tTxnNo(void);
    void   vTxnNo(tXN tTxnNo);

    i8     iAgeSec(void);

    i8     iBucketNo(void);
    void   vBucketNo(i8 iBucketNo);
  
    i8     iBufferHeight(void);
    void   vBufferHeight(i8 iBufferHeight);
  
    void   vDropWrite(void);
    void   vGrabWrite(void);
  #ifdef WITH_PTNID
    void   vPtnNo(PtnId tPtnNo);
    PtnId  tPtnNo(void);
  #else
    void   vPtnNo(tPN tPtnNo);
    tPN    tPtnNo(void);
  #endif
    
    Birth& oWhenNewed(void);
  
    // - - - - - - - - - - - - - -
    bool bIsPermanent(void);
    void vIsPermanent(bool bIsPermanent);
    i8   iBufferNo(void);
  };
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
