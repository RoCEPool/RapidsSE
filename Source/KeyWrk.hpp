/*============================================================================*/
/* KeyWrk.hpp -- Working Ent (contains an embedded object)                  */
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
  
  protected:
    Id    m_xId;       // id of row, or referenced entry in another buffer
  
    char* m_aEmbedded; // address of embedded object. this may be different
                       // from m_xWsp.aArea() to allow for the object
                       // not to have to be copied into the wsp
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key and row workspace
    Wsp_t<C_WSP_ENTWRK> m_xWsp; // embedded object workspace
  
    Footer m_xFooter;
  
  public:
     void* operator new(size_t);
     void* operator new(size_t, KeyWrk*);
  
     KeyWrk(void);
    ~KeyWrk(void) noexcept(false);
  
    i8     iBufferNo(void);
    void   vBufferNo(i8 iBufferNo);
    char*  aEmbedded(void);
    void   vEmbedded(char* aEmbedded);
    i8     nEntrySize(void);
    Id&    xId(void);
    void   vId(Id& xId);
    i8     iItemNo(void);
    void   vItemNo(i8 iItemNo);
    char*  aLatter(void);
    i8     nLevels(void);
    i8     nOffsetSize(void);
    void   vLevels(i8 nLevels);
    void   vRandomLevels(void);
    void   vTake(i8 iSize);

  
  void vAddDatum(Datum& oDatum, Column* pCol);
  void vAddElement(Row* pRow, Column* pCol);
  void vAddDatum(Stz* aSrc, Column* pCol, Formatters* pFormatters);
  void vAddMinusInfinity(void);
  void vAddPlusInfinity(void);
  void vAddWord(w8 wWord);
  
  void vExtractKey(RowBuffer* pRowBuffer, i8 i_Row, IdxColVec* pIdxColVec);
  
  i8   iHashKey(void);
  Key* aKey(void);
  i8   nKeySize(void);
  void vKeySize(i8 nKeySize);
  EntBuffer* pReferencedEntBuffer(void);
  void vReset(void);
  void vSetEntry(Ent* pEntry);
  void vSetKey(Ent* pEntry, i8 iBufferNo, i8 iItem, IdxColVec* pIdxColVec);
  void vShow(IdxColVec* pIdxColVec);

  
  
  
    Footer* aFooter(void);
  
  protected:
    i8     nDataSize(void);
    void   vDataSize(i8 nDataBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
