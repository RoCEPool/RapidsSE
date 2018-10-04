/*============================================================================*/
/* SEitf.hpp - se interface                                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  class SEitf : public Object
  {
  public:
    i8       m_iPid;             // process which created this seitf
    i8       m_iPartition;       // which partition this seitf connects to
    DicStack m_xPathStack;       // dictionary stack
    Job*     m_pJob;             // job seitf currently executing
    s48K     m_sResponse;        // response
    Tx*      m_pTx;              // current transaction
    bool     m_bSeitfExiting;    // is seitf exiting ?
    
    PerformanceTrace* m_pPerformanceTrace;  // performance Trace
    CommandProcessor* m_pCommandProcessor;  // command processor
  
    // these should go
    RowOnce*   m_pRowOnce;       // has row been seen before?

    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool      bBeginTx(void);
    void      vCommitTx(void);
    void      vContentDump(void);
    void      vEndTx(void);
    void      vInit(Stz* pCatalogName, Stz* pSchemaName);
    i8        iPartition(void);
    DicStack* pPathStack(void);
    
    PerformanceTrace* pPerformanceTrace(void);
    void vPerformanceTrace(PerformanceTrace* pPerformanceTrace);
    
    i8        iSlot(void);
    Tx*       pTx(void);
    void      vTx(Tx* pTx);
    
    //--------------------------------------------------------------------------
    // to be deleted
    void   vCreateRowOnce(void);
    bool   bRowSeen(Id xRowId);
    //--------------------------------------------------------------------------
  
    Footer* aFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
