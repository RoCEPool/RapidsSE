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
  
    void vAddDatum(char* aDatum);
    void vAddDatum(GenVar& xVariable, Column* pCol);
    void vAddDatum(Datum* pDatum, GenVar& xVariable, Column* pCol, Formatters* pFormatters);
    void vAddKey(KeyData* aKey, IdxColVec* pColVec);
    void vAddMinusInfinity(void);
    void vAddMinusInfinity_Flt(void);
    void vAddPlusInfinity(void);
    void vAddPlusInfinity_Flt(void);
    void vAddWord(w8 wWord);

    void vExtractKey(CacheLine* aRowData, IdxColVec* pIdxColVec);
    
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
