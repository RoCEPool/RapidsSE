/*============================================================================*/
/* Loader.hpp -- Parallel Table Loader                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    Table*   m_pTable;   // table being load
    s256     m_sPath;    // file being loaded
    Tx*      m_pTx;      // tx
    LoadMap* m_pLoadMap; // load map
    
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

    bool     bFindChunk(void);
    LoadMap* pLoadMap(void);
    StrVal&  sPath(void);
    void     vPath(StrVal& sPath);
    Table*   pTable(void);
    void     vTable(Table* pTable);
    void     vTidyUp(void);
    Tx*      pTx(void);
    void     vTx(Tx* pTx);
    
    Footer* aFooter(void);
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
