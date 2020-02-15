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
    IdxColVec* m_pIdxColVec;     // index column vector describing the key

    // - - - - - - - - - - - - - //
    //       entry proper        //
    Ent    m_xEnt;               // the index entry being formed
    byte   m_yaSpace[C_MAX_KEY]; // the bytes of the key
    // - - - - - - - - - - - - - //
  
    Footer m_xFooter;
  
  public:
    EntWrk(void);
   ~EntWrk(void) noexcept(false);
    
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
    void  vAppendDatum(Datum* pDatum, Column* pCol);
    void  vAppendElement(Row* pRow, Column* pCol);
    void  vAppendInt(i8 iArg);
    void  vAppendMinusInfinity(void);
    void  vAppendNull(void);
    void  vAppendPlusInfinity(void);
    void  vAppendRef(Ref xwArg);
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
  
    IdxColVec* pIdxColVec(void);
    void  vIdxColVec(IdxColVec* pIdxColVec);
    void  vIncElmSeqSize(i8 nElementSize);
    i8    iItem(void);
    void  vItem(i8 iItem);
    
    i8    nOffsets(void);
    void  vOffsets(i8 nLevels);

    void  vRandomEnt(IdxColVec* pIdxColVec);
    Ref&  xRef(void);
    void  vRef(Ref& xRef);
    void  vReset(void);

    void  vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry);
    void  vShow(IdxColVec* pIdxColVec);
    i8    nSize_EntKey(void);
    i8    nSize_EntKeyNeo(void);
    StreamDef* pStreamDef(void);
    void  vStreamDef(StreamDef* pStreamDef);

  private:

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
