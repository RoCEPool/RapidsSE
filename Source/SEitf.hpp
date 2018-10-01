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
    i8     m_iPid;             // process which created this seitf
    i8     m_iPartition;       // which partition this seitf connects to
    
    DicStack m_xPathStack;     // dictionary stack
    Job*   m_pJob;             // job seitf currently executing
    s48K   m_sResponse;        // response
    
    Table*   m_pCurrentTable;
  
    Tx*    m_pTx;
  
    bool   m_bDisplay;
  
    bool   m_bSeitfExiting;
    
    RowOnce* m_pRowOnce; // has row been seen before?
  
    Formatters m_xFormatters; // record formatters
  
    PerformanceTrace* m_pPerformanceTrace;
  
    CommandProcessor* m_pCommandProcessor;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void   vCurrentTable(Table* pTable);
    Table* pCurrentTable(void);
    
    void   vContentDump(void);
    
    void   vInit(Stz* pCatalogName, Stz* pSchemaName);
    
    i8     iPartition(void);
    i8     iSlot(void);
  
    DicStack* pPathStack(void);
  
    void   vCreateRowOnce(void);
    bool   bRowSeen(Id xRowId);
   
    Tx*    pTx(void);
    void   vTx(Tx* pTx);
  
    PerformanceTrace* pPerformanceTrace(void);
    void   vPerformanceTrace(PerformanceTrace* pPerformanceTrace);
  
    //--------------------------------------------------------------------------
    
    bool bBeginTx(void);
    void vCommitTx(void);
    void vEndTx(void);
    
    Footer* aFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
