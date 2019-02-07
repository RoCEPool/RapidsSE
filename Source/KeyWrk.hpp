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
    void vAddDatum(GenVar& xVariable, Column* pCol, Formatters* pFormatters);
    void vAddKey(KeyData* aKey, IdxColVec* pColVec);
    void vAddWord(w8 wWord);
    void vExtractKey(Item* aRowEntry, IdxColVec* pIdxColVec);
    KeyData* aKey(void);
    i8   nKeySize(void);
    void vKeySize(i8 nKeySize);
    EntryBuffer* pReferencedEntryBuffer(void);
    void vReset(void);
    void vSetEntry(Item* pEntry);
    void vShow(IdxColVec* pIdxColVec, bool bCompressed);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
