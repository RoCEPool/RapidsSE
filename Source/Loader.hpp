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
  
    Table*   m_pTable;   // table being load
    s256     m_sPath;    // file being loaded
    s32      m_sEnclosedBy;      // enclosed by
    s32      m_sEscapeChar;       // escape char
    s32      m_sFieldSeparator;   // field separator
    s32      m_sRecordTerminator; // record terminator
    LoadMap* m_pLoadMap;          // load map
    
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

    StrVal&  sEnclosedBy(void);
    void     vEnclosedBy(StrVal& sPath);
    StrVal&  sEscapeChar(void);
    void     vEscapeChar(StrVal& sPath);
    StrVal&  sFieldSeparator(void);
    void     vFieldSeparator(StrVal& sPath);
    bool     bFindChunk(void);
    LoadMap* pLoadMap(void);
    StrVal&  sPath(void);
    void     vPath(StrVal& sPath);
    StrVal&  sRecordTerminator(void);
    void     vRecordTerminator(StrVal& sPath);
    Interface::SEitf* pSEitf(void);
    void     vSEitf(Interface::SEitf* pSEitf);
    Table*   pTable(void);
    void     vTable(Table* pTable);
    void     vTidyUp(void);
    
    Footer* aFooter(void);
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
