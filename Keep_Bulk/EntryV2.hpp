/*============================================================================*/
/* Entryv2.hpp -- buffer entry                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// there must be a
#pragma pack(push)
#pragma pack(1)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


namespace DBobjects
{
  class EntryV2 : public Header
  {
  protected:
    Id   m_xId;       // id
    u2   m_nBytes;    // number of bytes that follow
    byte m_aBytes[1]; // array of bytes
    // .. bytes ..
    // .. [NEO] ..    // next entry offset(s)

  public:
    void* operator new(size_t);
    void* operator new(size_t, EntryV2*);
  
    EntryV2(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~EntryV2(void) noexcept(false);
  
    byte* aBytes(void);
    i8    nBytes(void);
    i8    iSize(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline EntryV2* _aEntryV2(void* aAddr)
  {
    EntryV2* aEntry = reinterpret_cast<EntryV2*>(aAddr);
    
    //RASSERT_TYPE(aEntry,CN_ENTRY);
    
    return aEntry;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// matches pragma push() at beginning of file.  Do not remove
#pragma pack(pop)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
