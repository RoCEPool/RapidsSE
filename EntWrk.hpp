/*============================================================================*/
/* EntWrk.hpp -- Working Index Entry                                          */
/*============================================================================*/
#pragma once

//! entry work is used to form an entry which is then inserted into an index

namespace Data
{
  class Key;
  
  class EntWrk : public Object
  {
  friend class Ent;
  friend class Table;
  
  static const i8 C_WSP_ENTWRK = C_17KB;
  
  private:
    Id    m_xId; // id of row, or referenced entry in another buffer
  
    byte* m_ayEmbedded; // address of embedded object. this may be different
                        // from m_xWsp.aArea() to allow for the object
                        // not to have to be copied into the wsp
  
    Wsp_t<C_WSP_ENTWRK> m_xWsp; // key workspace
  
    Footer m_xFooter;
  
  public:
     void* operator new(size_t);
     void* operator new(size_t, EntWrk*);
  
     EntWrk(void);
    ~EntWrk(void) noexcept(false);
  
     void  vAddElement(Row* pRow, Column* pCol);
     void  vAddElement(Stz* aSrc, Column* pCol, Formatters* pFormatters);
     void  vAddElement(w8 wWord);
     void  vAddMinusInfinity(void);
     void  vAddPlusInfinity(void);
  
    i8     iBufferNo(void);
    void   vBufferNo(i8 iBufferNo);
  
    byte*  ayEmbedded(void);
    void   vEmbedded(byte* ayEmbedded);
    i8     nEntrySize(void);
 
    void   vFormEntry(Ent* pEntry);
  
    i8     iHashKey(void);
  
    Id&    xId(void);
    void   vId(Id& xId);
    i8     iItemNo(void);
    void   vItemNo(i8 iItemNo);
  
    Key*   aKey(void);
    i8     nKeySize(void);
    void   vKeySize(i8 nKeySize);
  
    i8     nLevels(void);
    void   vLevels(i8 nLevels);

    i8     nOffsetSize(void);
  
    void   vRandomLevels(void);
    EntBuffer* pReferencedEntBuffer(void);
    void   vReset(void);

    void   vSetDetails(Ent* pEntry, i8 iBufferNo, i8 i_Item);
    void   vShow(IdxColVec* pIdxColVec);
  
  private:
    void   vIncKeyBytes(i8 nElementBytes);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
