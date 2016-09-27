/*============================================================================*/
/* LoaderSub.hpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class LoaderSub : public Object
  {
  private:
    Loader*  m_pLoader;  // the reference information
    char*    m_pRecord;  // record from file
    i8       m_iLength;  // length of record
    i8       m_iHash;          // hash of record
    i8       m_iPartition;     // which partition
    bool     m_bMoreChunks;    // more chunks
    LoadBuffer* m_pLoadBuffer; // loading buffer
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, LoaderSub* pLoaderSub);
    void  operator delete(void* pLoaderAddress);
 
    LoaderSub(void);
    ~LoaderSub(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vBegin(void);
    bool bCompleted(void);
    void vEnd(void);
    bool bFetchRecord(void);
    bool bFetchChunk(void);
    void vGetRecord(StrVal* pStrVal);
    void vHashRecord(void);
    bool bInsertRow(void);
    void vLoader(Loader* pLoader);
    
    Footer* pFooter(void);
  };
  
  LoaderSub* _pLoaderSub(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/