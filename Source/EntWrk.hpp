/*============================================================================*/
/* EntWrk.hpp -- Working Index Entry                                          */
/*============================================================================*/
#pragma once

//! entry work is used to form an entry which is then inserted into an index

namespace Indexes
{
  class Key;
  
  class EntWrk : public Object
  {
  friend class Ent;
  friend class Table;
  
  static const i8 C_WSP_ENTWRK = C_17KB;
  
  private:
    Id    m_xId;  // sub ref (child index buffer), or row ref (H0 leaf buffers)
  
    Key   m_xKey; // the key
  
    Wsp_t<C_WSP_ENTWRK> m_xWsp; // key workspace
  
    byte* m_ayElementCrs; // where to add next element
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWrk*);

    EntWrk(void);
   ~EntWrk(void) noexcept(false);
  
    static eKC eCompare(Ent* aEnt1, EntWrk& xEntWrk2);
    static eKC eCompare(EntWrk& xEntWrk1, EntWrk& xEntWrk2);
    static eKC eCompare(EntWrk& xEntWrk1, Ent* aEnt2);
    static eKC eCompare(Ent* aEnt1, Ent* aEnt2);
  
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
    static bool bEqualTo(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterThan(Ent* aEnt1, Ent* aEnt2);
    static bool bGreaterEqual(Ent* aEnt1, EntWrk& xEntWrk2);
    static bool bGreaterEqual(Ent* aEnt1, Ent* aEnt2);
    static bool bLessEqual(EntWrk& xEntWrk1, Ent* aEnt2);
    static bool bLessThan(EntWrk& xEntWrk1, Ent* aEnt2);
    static bool bNotEqual(Ent* aEnt1, Ent* aEnt2);

    void  vAddElement(Row* pRow, Column* pCol);
    void  vAddMinusInfinity(void);
    void  vAddPlusInfinity(void);
  
    i8    iBufferNo(void);
    void  vBufferNo(i8 iBufferNo);
  
    byte* ayElementCrs(void);
    i8    nEntKeySize(void);
    i8    nEntKeyNeoSize(void);

    void  vFormEntry(Ent* pEntry);
  
    i8    iHashKey(void);
  
    Id&   xId(void);
    void  vId(Id& xId);
    void  vIncKeyBytes(i8 nElementBytes);
    i8    iItemNo(void);
    void  vItemNo(i8 i_Item);
  
    byte* ayKeyBytes(void);
    void  vKeyBytes(i8 nKeySize, byte* ayKeyBytes);

    i8    nKeySize(void);
    void  vKeySize(i8 nKeySize);
  
    i8    nLevels(void);
    void  vLevels(i8 nLevels);

    i8    nOffsetsSize(void);
  
    void  vRandomLevels(void);

    void  vSetWrk(i8 iBufferNo, i8 i_Item, Ent* pEntry);
    void  vShow(IdxColVec* pIdxColVec);
  
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
