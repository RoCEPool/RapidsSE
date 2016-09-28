/*============================================================================*/
/* SkipMap.hpp -- Skip Map                                                    */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class SkipMap : public Buffer
  {
    static const i8 C_SKIPMAP_SIZE = C_1KB * 64;
 
  private:
    i8   m_iLimit;  // limit
    
    Lock m_zLock;   // lock
    i8   m_iNext;   // next
    i8   m_iSeek[C_SKIPMAP_SIZE];
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SkipMap(void);
    ~SkipMap(void);

    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vAdd(i8 iSeek);
    bool vNext(i8& iStart, i8& iEnd);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/