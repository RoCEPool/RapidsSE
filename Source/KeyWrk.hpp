/*============================================================================*/
/* KeyWrk.hpp -- Working Key                                                  */
/*============================================================================*/
#pragma once

namespace Data
{
  class Key;
  
  class KeyWrk : public Object
  {
  friend class Ent;
  friend class Table;
  
  static const i8 C_WSP_ENTWRK = C_17KB;
  
  private:
    Id    m_xId;       // id of row, or referenced entry in another buffer
  
    char* m_aEmbedded; // address of embedded object. this may be different
                       // from m_xWsp.aArea() to allow for the object
                       // not to have to be copied into the wsp
  
    Wsp_t<C_WSP_ENTWRK> m_xWsp; // key workspace
  
    Footer m_xFooter;
  
  public:
     void* operator new(size_t);
     void* operator new(size_t, KeyWrk*);
  
     KeyWrk(void);
    ~KeyWrk(void) noexcept(false);
  
     void  vAddDatum(Datum& oDatum, Column* pCol);
     void  vAddElement(Row* pRow, Column* pCol);
     void  vAddDatum(Stz* aSrc, Column* pCol, Formatters* pFormatters);
     void  vAddMinusInfinity(void);
     void  vAddPlusInfinity(void);
     void  vAddWord(w8 wWord);
  
    i8     iBufferNo(void);
    void   vBufferNo(i8 iBufferNo);
  
    char*  aEmbedded(void);
    void   vEmbedded(char* aEmbedded);
    i8     nEntrySize(void);
    void   vExtractKey(RowBuffer* pRowBuffer, i8 i_Row, IdxColVec* pIdxColVec);

    i8     iHashKey(void);
  
    Id&    xId(void);
    void   vId(Id& xId);
    i8     iItemNo(void);
    void   vItemNo(i8 iItemNo);
  
    Key*   aKey(void);
    i8     nKeySize(void);
    void   vKeySize(i8 nKeySize);
  
    char*  aLatter(void);
    i8     nLevels(void);
    void   vLevels(i8 nLevels);

    i8     nOffsetSize(void);
  
    void   vRandomLevels(void);
    EntBuffer* pReferencedEntBuffer(void);
    void   vReset(void);

    void   vSetEntry(Ent* pEntry);
    void   vSetKey(Ent* pEntry, i8 iBufferNo, i8 iItem, IdxColVec* pIdxColVec);
    void   vShow(IdxColVec* pIdxColVec);
  
    void   vTake(i8 iSize);

    Footer* aFooter(void);
  
  protected:
    i8     nDataSize(void);
    void   vDataSize(i8 nDataBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
