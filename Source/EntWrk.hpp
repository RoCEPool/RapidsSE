/*============================================================================*/
/* EntWrk.hpp -- Working Index Entry                                          */
/*============================================================================*/
#pragma once

//! entry work is used to form an entry which is then inserted into an index

namespace Entries
{
  class EntWrk : public StackObject
  {
  private:
  
    StreamDef* m_pStreamDef;     // stream definition
    IndexDef*  m_pIndexDef;      // index definition
  
    // - - - - - - - - - - - - - - - //
    //        entry                  //
    Ent    m_xEnt;                   // the index entry being formed
    byte   m_yaSpace[C_MAX_KEY];     // the bytes of the key
    // - - - - - - - - - - - - - - - //
  
    // - - - - - - - - - - - - - - - //
    //     opt. payload              //
    byte   m_yaPaySpace[C_MAX_PAY];  // more space to cater for payload
    // - - - - - - - - - - - - - - - //
    
    Footer m_xFooter;
  
  public:
    EntWrk(void);
   ~EntWrk(void) noexcept(false);
    
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
    void  vAppendDatum(Datum* pDatum, Column* pCol);
    void  vAppendElement(RowSingle* pRow, Column* pCol);
    void  vAppendElement(Rsb* aRsb, Column* pCol);
    void  vAppendElement(Rsb* aRsb, StreamPix* pStreamPix, i8 i_Col);
    void  vAppendElementInt(Rsb* aRsb, i8 iInt);
    void  vAppendElementStz(Rsb* aRsb, Stz* szStr);
    void  vAppendInt(i8 iArg);
    void  vAppendMinusInfinity(void);
    void  vAppendNull(void);
    void  vAppendPlusInfinity(void);
    void  vAppendRef(Rrf xwArg);
    void  vAppendStz(Stz* szArg);

    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);

    byte* ayElmNext(void);
    byte* ayElmSeq(void);
    void  vElmSeq(i8 nElmSeqSize, byte* ayElmSeq);
    void  vElmSeqSize(i8 nElmSeqSize);
    Ent*  aEnt(void);
    
    i8    iHashKey(void);

    Key*  aKey(void);
  
    IndexDef* pIndexDef(void);
    void  vIndexDef(IndexDef* pIndexDef);
    void  vIncElmSeqSize(i8 nElementSize);
    i8    iItem(void);
    void  vItem(i8 iItem);
    
    i8    nOffsets(void);
    void  vOffsets(i8 nLevels);
  
    Pay*  aPay(void);
    i8*   aiPayCell(i8 iCell);
    byte* ayPayload(void);
    bool  bPayload(void);
    void  vPayload(bool bPayload);
    i8    nPayloadBytes(void);
    void  vPayloadBytes(i8 nPayloadBytes);

    void  vRandomEnt(IndexDef* pIndexDef);
    Rrf&  rxRef(void);
    void  vRef(Rrf& xRef);
    void  vReset(void);

    void  vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry);
    void  vShow(IndexDef* pIndexDef);
    StreamDef* pStreamDef(void);
    void  vStreamDef(StreamDef* pStreamDef);

    i8    nTotalSize(void);

  private:

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
