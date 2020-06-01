/*============================================================================*/
/* Loader.hpp -- Loader                                                       */
/*============================================================================*/
#pragma once

namespace Load
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Loader : public Object
  {
  public:
 
    Table*        m_pTable;         // table
    WriteHash*    m_pWriteHash;

    LoadCursor*   m_pLoadCursor;
    RowSingle*    m_pRow;           // row

    Format        m_oFormatter;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Loader(Table* pTable, WriteHash* pWriteHash);
   ~Loader(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bReadRecords(Job* pPipelineJob, Stz* aDataArea, i8 nDataBytes);
    bool bSetFormat(TagSet* pFmtTagSet);
    bool bTerminate(void);

  private:
    eEC  eAppendRow(void);
    bool bExtractRow(Stz* szRecStr, bool &bBlankLine);
    eEC  eProcessRecord(Stz* aRecord,  i8 nBytes);

    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
