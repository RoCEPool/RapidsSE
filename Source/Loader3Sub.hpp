/*============================================================================*/
/* Loader3Sub.hpp -- Subordinate Table Loader                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader3Sub : public Object
  {
  private:
    Loader3* m_pLoader3; // the reference information
    char*    m_pRecord;  // record from file
    i8       m_iLength;  // length of record
    LoadBuffer* m_pLoadBuffer; // loading buffer
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader3Sub* pLoader3Sub);
    void  operator delete(void* pLoader3Address);
 
    Loader3Sub(void);
    ~Loader3Sub(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void   vBegin(void);
    bool   bCompleted(void);
    void   vEnd(void);
    bool   bFetchRecord(void);
    bool   bFetchChunk(void);
    void   vGetRecord(StrVal* pStrVal);
    bool   bInsertRow(i8 iPartn);
    void   vLoader3(Loader3* pLoader3);
    
    Footer* pFooter(void);
  };
  
  Loader3Sub* _pLoader3Sub(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/