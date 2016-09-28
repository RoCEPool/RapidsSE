/*============================================================================*/
/* SeekMap.hpp -- Seek Map                                                    */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class SeekMap : public Buffer
  {
    static const i8 C_SEEKMAP_ENTRIES = C_1M / sizeof(i8);
 
  private:
    i8   m_iEntries;  // logical limit of m_iEnd
    bool m_bDone;     // is map complete?
    
    Lock m_zLock;     // subordinate lock
    i8   m_iNext;     // which read-chunk to be taken by next subordinate
    i8   m_iEolOffset[C_SEEKMAP_ENTRIES]; // array of ending '\n' offsets
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SeekMap(void);
    ~SeekMap(void);

    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vEolOffset(i8 iEolOffset);
    bool bDone(void);
    void vDone(bool bDone);
    bool bNext(i8& iSeek, i8& nBytes);
    
    Footer* pFooter(void);
  };
  
  SeekMap* _pSeekMap(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/