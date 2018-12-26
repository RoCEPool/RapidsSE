/*============================================================================*/
/* RecordIngestor.hpp -- Record Ingestor                                      */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordIngestor : public RecordExtractor
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RecordIngestor(Formatters* pFormatters);
    ~RecordIngestor(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
      
    Footer* aFooter(void);
  };
  
  RecordIngestor* _pRecordIngestor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
