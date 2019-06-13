/*============================================================================*/
/* EntWrk.hpp -- Working Index Entry                                          */
/*============================================================================*/
#pragma once

//! entry work is used to form an entry which is then inserted into an index

namespace Indexes
{
  class EntWrk : public Object
  {
  private:

    Ent    m_xEnt; // the entry being formed
  
    byte   m__KeyBytes[C_MAX_KEY]; // the bytes of the key
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWrk*);

    EntWrk(void);
   ~EntWrk(void) noexcept(false);
  
    //static eKC eCompare(Ent* aEnt1, EntWrk& xEntWrk2);
    //static eKC eCompare(EntWrk& xEntWrk1, EntWrk& xEntWrk2);
    //static eKC eCompare(EntWrk& xEntWrk1, Ent* aEnt2);
    
    static eKC eCompare(Ent* aEnt1, Ent* aEnt2);
  
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
    static bool bEqualTo(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterThan(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterEqual(Ent* aEnt1, EntWrk& xEntWrk2);
    static bool bGreaterEqual(Ent* aEnt1, Ent* aEnt2);
    static bool bLessEqual(EntWrk& xEntWrk1, Ent* aEnt2);
    static bool bLessThan(EntWrk& xEntWrk1, Ent* aEnt2);
    static bool bNotEqual(Ent* aEnt1, Ent* aEnt2);

    void  vAppendElement(Row* pRow, Column* pCol);
    void  vAppendMinusInfinity(void);
    void  vAppendPlusInfinity(void);
  
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
    byte* ayByteSeq(void);
    void  vByteSeq(i8 nKeyBytes, byte* ayByteSeq);

    byte* ayElementCrs(void);
    Ent*  aEnt(void);
    
    void  vFormEntry(Ent* pEntry);
  
    i8    iHashKey(void);
  
    void  vIncKeyBytes(i8 nElementBytes);
    i8    iItem(void);
    void  vItem(i8 iItem);
  
    i8    nKeyBytes(void);
    void  vKeyBytes(i8 nKeyBytes);
  
    i8    nLevels(void);
    void  vLevels(i8 nLevels);

    i8    nNEOsize(void);
  
    void  vRandomLevels(void);
    Id&   xRef(void);
    void  vRef(Id& xRef);

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
