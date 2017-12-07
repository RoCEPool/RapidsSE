/*============================================================================*/
/* Loader.hpp -- Parallel Table Loader                                        */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SEitf;
}

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    Interface::SEitf* m_pSEitf; // SE interface
  
    Table*     m_pTable;       // table being load
    s256       m_sPath;        // file being loaded
    LoadMap*   m_pLoadMap;     // load map
    Atomic     m_ziInvalidRecords;
    Atomic     m_ziValidRecords;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader* pLoader);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);

    void     vAddInvalidRecords(i8 iInvalidRecords);
    void     vAddValidRecords(i8 iValidRecords);
    bool     bFindChunk(void);
    i8       iInvalidRecords(void);
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
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
