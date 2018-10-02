/*============================================================================*/
/* LoadTable.hpp -- Parallel Table LoadTable                                  */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SEitf;
}

namespace Utilities
{
  class LoadTable : public Object
  {
  private:
    Job_LoadTable* m_pJob_LoadTable; // job
  
    Table*    m_pTable;       // table being load
    LoadMap*  m_pLoadMap;     // load map
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, LoadTable* pLoadTable);
    void  operator delete(void* pLoaderAddress);
 
    LoadTable(Job_LoadTable* pJob_LoadTable);
    ~LoadTable(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);

    void     vDumpMap(void);
    bool     bFindChunk(void);
    Job_LoadTable* pJob_LoadTable(void);
    LoadMap* pLoadMap(void);
    Table*   pTable(void);
    void     vTable(Table* pTable);
    void     vTidyUp(void);
    
    Footer* aFooter(void);
  
    static void vAdvanceRecordEnd(char** aSrc, Formatters* pFormatters);
  
  };
  
  LoadTable* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
