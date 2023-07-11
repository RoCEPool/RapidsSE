/*============================================================================*/
/* DataBuffer.hpp -- Rows Buffer                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DataBuffer : public LinkedBuffer
  {
  protected:
    Lock         m_okLock;        // lock
    i8           m_iBufferNo;     // this DataBuffer's buffer number

    BufferId     m_oBufferId;     // Location to where this buffer can be paged out to in page file on disk

    LglPtn m_tLglPtn;

    i8     m_iBufferHeight; // buffer height in the tree
    Birth  m_oWhenNewed;    // when was this buffer created?
    b8     m_bIsPermanent;  // is buffer permanent ?
  
    Table*     m_pTable_dbuff;
    StreamDef* m_pStreamDef_dbuff;

    // TODO: Arjun - Hack till we figure out a better way to handle the lock problem
    // during output operator
    b8    m_bOutputQueue; // is this Buffer in the Output queue?

    tXN m_tTxnNo; // transaction number

    PadWord m__PadToCacheLine[17];


  public:
    DataBuffer(eCN eClass, bool bIsPermanent);

    ~DataBuffer(void)

    noexcept(false);

    static bool bIsDataBuffer(eCN eClass);

    static DataBuffer *pRefdDataBuffer(Rrf &xRef);

    static void vRepairList(Object *pObject);

    static DataBuffer *pToDataBuffer(void *aAddr);

    void vStreamDef_dbuff(StreamDef *pStreamDef);

    StreamDef *pStreamDef_dbuff(void);

    Table *pTable_dbuff(void);

    void vTable_dbuff(Table *pTable);

    tXN tTxnNo(void);

    void vTxnNo(tXN tTxnNo);

    i8 iAgeSec(void);

    i8 iBufferHeight(void);

    void vBufferHeight(i8 iBufferHeight);

    void vDropRead(void);

    void vDropWrite(void);

    void vGrabRead(void);

    void vGrabWrite(void);

    void vLglPtn(LglPtn tLglPtn);

    void vLglPtn(i8 tLglPtn);

    LglPtn tLglPtn(void);

    Birth &oWhenNewed(void);
    // - - - - - - - - - - - - - -
    bool bIsLocked(void);

    bool bIsPermanent(void);

    void vIsPermanent(bool bIsPermanent);

    i8   iBufferNo(void);

    bool bOutputQueued(void);

    void vOutputQueued(bool bOutputQueued);

    void     vSetBufferNo(i8 iBufferNo);

    // For Paging
    BufferId oGetBufferId(void);
    void vSetBufferId(void* pIdAsAddr);
  };
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
