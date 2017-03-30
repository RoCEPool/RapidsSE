/*============================================================================*/
/* EntryData.hpp -- entry data                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntryData
  {
  private:
    // ..data..
  
  public:
    static i8 nBytesRequired(i8 nLevels, i8 nKeyBytes);
  
    void* operator new(size_t);
  
    EntryData(void);
    ~EntryData(void);
  
    void vCopyBytes(void* aSrc, i8 nBytes);
    Key* aKey(i8 nLevels);
    RightVec* aRightVec(i8 nLevels);
  
  };
  
  EntryData* _aEntryData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
