/*============================================================================*/
/* DataBuffer.hpp -- Rows Buffer                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class DataBuffer : public Operand
  {
    // TODO: Arjun - Remove this padding
    PadWord m__PadToCacheLineLinkedBuffer[8];
  protected:
    Lock         m_okLock;        // lock
    i8           m_iBufferNo;     // this DataBuffer's buffer number

    BufferId     m_oBufferId;     // Location to where this buffer can be paged out to in page file on disk
    bool        m_bHashMap;

    LglPtn m_tLglPtn;

    i8     m_iBufferHeight; // buffer height in the tree
    Birth  m_oWhenNewed;    // when was this buffer created?
    b8     m_bIsPermanent;  // is buffer permanent ?
  
    Table*     m_pTable_dbuff;
    StreamDef* m_pStreamDef_dbuff;

    tXN m_tTxnNo; // transaction number

    PadWord m__PadToCacheLine[17];


  public:
    DataBuffer(eCN eClass, bool bIsPermanent);
    DataBuffer(bool bAddToHashMap, eCN eClass, bool bIsPermanent);

    ~DataBuffer(void)

    noexcept(false);

    static bool bIsDataBuffer(eCN eClass);

    static DataBuffer *pRefdDataBuffer(Rrf &xRef);

    static void vRepair(Object *pObject);

    static DataBuffer *pToDataBuffer(void *aAddr);

    void vStreamDef_dbuff(StreamDef *pStreamDef);

    StreamDef *pStreamDef_dbuff(void);

    Table *pTable_dbuff(void);

    void vTable_dbuff(Table *pTable);

    tXN tTxnNo(void);

    void vTxnNo(tXN tTxnNo);

    i8 iAgeSec(void);

    void vBufferHeight(i8 iBufferHeight);

    void vDropRead(void);

    void vDropWrite(void);

    void vGrabRead(void);

    void vGrabWrite(void);

    void vHashMap(bool bInHashMap);

    bool bHashMap(void);

    void vLglPtn(LglPtn tLglPtn);

    void vLglPtn(i8 tLglPtn);

    LglPtn tLglPtn(void);

    Birth &oWhenNewed(void);
    // - - - - - - - - - - - - - -
    bool bIsLocked(void);

    bool bIsPermanent(void);

    void vIsPermanent(bool bIsPermanent);

    i8   iBufferNo(void);

    void     vSetBufferNo(i8 iBufferNo);

    // For Paging
    BufferId oGetBufferId(void);
    void vSetBufferId(void* pIdAsAddr);

		/*----------------------------------------------------------------------------*/
		/* get the buffer height                                                      */
		/*----------------------------------------------------------------------------*/
		inline i8 iBufferHeight(void)
		{
			_precondition_(_ADDRESS_,_MAGIC_);

			return m_iBufferHeight;
		}

		};
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
