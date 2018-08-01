/*============================================================================*/
/* Loader.hpp -- Parallel Table Loader                                        */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SEitf;
}

namespace Utilities
{
  class Loader : public Object
  {
  private:
    Job_LoadTable* m_pJob_LoadTable; // job
  
    LoadMap*   m_pLoadMap;     // load map
    Atomic     m_ziInvalidRecords;
    Atomic     m_ziValidRecords;
  
    Interface::SEitf* m_pSEitf; // SE interface
    Table*     m_pTable;       // table being load
    s256       m_sPath;        // file being loaded
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader* pLoader);
    void  operator delete(void* pLoaderAddress);
 
    Loader(Job_LoadTable* pJob_LoadTable);
    ~Loader(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);

    void     vAddInvalidRecords(i8 iInvalidRecords);
    void     vAddValidRecords(i8 iValidRecords);
    void     vDumpMap(void);
    bool     bFindChunk(void);
    i8       iInvalidRecords(void);
    Job_LoadTable* pJob_LoadTable(void);
    LoadMap* pLoadMap(void);
    StrVal&  sPath(void);
    void     vPath(StrVal& sPath);
    Interface::SEitf* pSEitf(void);
    void     vSEitf(Interface::SEitf* pSEitf);
    Table*   pTable(void);
    void     vTable(Table* pTable);
    void     vTidyUp(void);
    i8       iValidRecords(void);
    
    Footer* aFooter(void);
  
    static void vAdvanceRecordEnd(char** aSrc, Formatters* pFormatters);
  
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
