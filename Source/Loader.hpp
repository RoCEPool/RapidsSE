/*============================================================================*/
/* Loader.hpp -- Loader                                                       */
/*============================================================================*/
#pragma once

namespace Load
{
  class Loader : public Object
  {
  public:
 
    Table*      m_pTable;         // table
    StreamDef*  m_pStreamDef;

    CrwHash*    m_pCrwHash;

    LoadCursor* m_pLoadCursor;
    RowSingle*  m_pRow;           // row

    Format      m_oFormatter;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Loader(Table* pTable, CrwHash* pWriteHash);
   ~Loader(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bCsvToCrw(Stz* szCsvArea, i8 nCsvBytes, Job* pPipelineJob);
  
    bool bSetFormat(TagSet* pFmtTagSet);

  private:
    eEC  eAppendRow(void);
    eEC  eProcessRecord(Stz* szRecord,  i8 nBytes);
    bool bTerminateEarly(void);
    eEC  eValidRecord(Stz* szRecord, i8 nBytes);

    Footer* aFooter(void);
  };
  
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
