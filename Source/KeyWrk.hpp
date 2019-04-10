/*============================================================================*/
/* KeyWrk.hpp -- KeyData Working Item                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class EntryBuffer;
  class KeyData;
  
  class KeyWrk : public EntWrk
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWrk*);
    
    KeyWrk(void);
    ~KeyWrk(void) noexcept(false);

    void vAddDatum(Datum& oDatum, Column* pCol);
    void vAddDatum(Stz* aSrc, Column* pCol, Formatters* pFormatters);
    void vAddMinusInfinity(void);
    void vAddMinusInfinity_Flt(void);
    void vAddPlusInfinity(void);
    void vAddPlusInfinity_Flt(void);
    void vAddWord(w8 wWord);

    void vExtractKey(RowNub* pRowNub, i8 i_Row, IdxColVec* pIdxColVec);
    
    i8   iHashKey(void);
    KeyData* aKey(void);
    i8   nKeySize(void);
    void vKeySize(i8 nKeySize);
    EntryBuffer* pReferencedEntryBuffer(void);
    void vReset(void);
    void vSetEntry(Item* pEntry);
    void vSetKey(Item* pEntry, i8 iBufferNo, i8 iItem, IdxColVec* pIdxColVec);
    void vShow(IdxColVec* pIdxColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
