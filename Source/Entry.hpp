/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

#pragma pack(push)
#pragma pack(1)

namespace Data
{
  class Entry : public Item
  {
  protected:

  public:
    // ..embedded object..

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void) noexcept(false);
    
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Entry* _aEntry(void* aAddr)
  {
    Entry* aEntry = reinterpret_cast<Entry*>(aAddr);
    
    //RASSERT_TYPE(aEntry,CN_ENTRY);
    
    return aEntry;
  }
  
}
#pragma pack(pop)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
