/*============================================================================*/
/* Loader.hpp -- Parallel Table Loader                                        */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    Table*   m_pTable;   // table being load
    s256     m_sPath;    // file being loaded
    TxRef*   m_pTxRef;   // tx
    LoadMap* m_pLoadMap; // load map
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader* pLoader);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    void     vFinalize(void);
    bool     bFindChunk(void);
    LoadMap* pLoadMap(void);
    StrVal&       sPath(void);
    void     vPath(StrVal& sPath);
    Table*   pTable(void);
    void     vTable(Table* pTable);
    TxRef*   pTxRef(void);
    void     vTxRef(TxRef* pTxRef);
    
    Footer* pFooter(void);
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
