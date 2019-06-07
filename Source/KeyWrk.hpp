/*============================================================================*/
/* KeyWrk.hpp -- Key Working Ent                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class EntBuffer;
  class Key;
  class Row;
  
  class KeyWrk : public EntWrk
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWrk*);
    
    KeyWrk(void);
    ~KeyWrk(void) noexcept(false);

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
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
