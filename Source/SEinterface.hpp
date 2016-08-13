/*============================================================================*/
/* SEinterface.hpp - se interface                                             */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class SEinterface : public Object
  {
  public:
    i8         m_iPid;           // process which created this seitf
    Dictionary m_xDictionary;    // private dictionary

    Queue*     m_pRequestQueue;  // request queue
    Queue*     m_pResponseQueue; // reponse queue
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEinterface(void);
    ~SEinterface(void);
    
    Dictionary* pDictionary(void);
    
    void   vDump(const char* aTitle);
    void   vInit(void);
    void   vPrompt(void);
    bool   bDecode(s& sRequest);
    
    //--------------------------------------------------------------------------
    
    void   vAppendRequest(s& sRequest);
    bool   bPopResponse(s& sResponse);
    bool   bPopResponse(i8& iResult, s& sResponse);
    
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);
    
    //--------------------------------------------------------------------------
    
    bool bConnect();
    bool bDisconnect();

    bool bCreateTenant();
    bool bDropTenant();

    bool bCreateCatalog();
    bool bDropCatalog();
    
    bool bCreateSchema();
    bool bDropSchema();
    
    bool bCreateTable();
    bool bDroptable();
    
    bool bCopyTable();
    bool bExportTable();
    bool bFetchTable();
    bool bLoadTable();
    bool bTruncateTable();
    
    bool bPopDic();
    bool bPushDic();
    bool bGetDicPath();
    bool bSetDicPath();
    bool bGetDicContents();
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
