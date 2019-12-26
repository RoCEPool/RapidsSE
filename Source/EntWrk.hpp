/*============================================================================*/
/* EntWrk.hpp -- Working Index Entry                                          */
/*============================================================================*/
#pragma once

//! entry work is used to form an entry which is then inserted into an index

namespace Entries
{
  class EntWrk : public Object
  {
  private:

    Ent    m_xEnt; // the index entry being formed
  
    byte   m_yaSpace[C_MAX_KEY]; // the bytes of the key
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWrk*);

    EntWrk(void);
   ~EntWrk(void) noexcept(false);
    
    static eKC eCompare(Ent* aEnt1, Ent* aEnt2);
  
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
    static bool bEqualTo(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterThan(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterEqual(Ent* aEnt1, Ent* aEnt2);
    static bool bLessEqual(Ent* aEnt1, Ent* aEnt2);
    static bool bLessThan(Ent* aEnt1, Ent* aEnt2);
    static bool bNotEqual(Ent* aEnt1, Ent* aEnt2);

    void  vAppendElement(Row* pRow, Column* pCol);
    void  vAppendMinusInfinity(void);
    void  vAppendPlusInfinity(void);
  
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);

    byte* ayElmNext(void);
    Ent*  aEnt(void);
    
    void  vFormEntry(Ent* pEntry);
  
    i8    iHashKey(void);
  
    void  vIncElmSeqSize(i8 nElementSize);
    i8    iItem(void);
    void  vItem(i8 iItem);
  
    byte* ayElmSeq(void);
    void  vElmSeq(i8 nElmSeqSize, byte* ayElmSeq);
    i8    nElmSeqSize(void);
    void  vElmSeqSize(i8 nElmSeqSize);

    i8    nLevels(void);
    void  vLevels(i8 nLevels);

    i8    nNEOsize(void);
  
    void  vRandomLevels(void);
    Ref&   xRef(void);
    void  vRef(Ref& xRef);

    void  vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry);
    void  vShow(IdxColVec* pIdxColVec);
    i8    nSize_EntKey(void);
    i8    nSize_EntKeyNeo(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
