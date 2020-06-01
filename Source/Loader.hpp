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
 
    Format        m_oFormatter;
    RecordData*   m_pRecordData;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Loader(Table* pTable);
   ~Loader(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bReadRecords(WriteHash* pWriteHash, Job* pPipelineJob,
                      Stz* aDataArea, i8 nDataBytes);
    bool bSetFormat(TagSet* pFmtTagSet);
    bool bTerminate(void);

  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
