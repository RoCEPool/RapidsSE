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
    void* operator new(size_t);
  
    EntryData(void);
    ~EntryData(void);
  
    RightVec* aRightVec(i8 nOffsets);
  
  };
  
  EntryData* _aEntryData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
