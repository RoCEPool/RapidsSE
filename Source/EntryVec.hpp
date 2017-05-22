/*============================================================================*/
/* EntryVec.hpp -- index entry vector                                         */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBuffer;
  
  class EntryVec : public Object
  {
  private:
    Entry* m_pEntry[C_FLAT_LEVELS];
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    EntryVec(void);
    ~EntryVec(void);

    Entry* operator [](u8 iLevelNo);
    
    Footer* pFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
