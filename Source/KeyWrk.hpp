/*============================================================================*/
/* KeyWrk.hpp -- Key Working Entry                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntryBuffer;
  class Key;
  
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
    void vAddKey(Key* aKey, IdxColVec* pColVec);
    void vAddWord(w8 wWord);
    Key* aKey(void);
    i8   nKeySize(void);
    void vKeySize(i8 nKeySize);
    EntryBuffer* pReferencedEntryBuffer(void);
    void vReset(void);
    void vShow(IdxColVec* pIdxColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
