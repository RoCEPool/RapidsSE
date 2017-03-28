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
    
  };
  
  EntryData* _aEntryData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
