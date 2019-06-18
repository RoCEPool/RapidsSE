/*============================================================================*/
/* RecordExtractor.hpp -- Record RowExtractor                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordExtractor : public DataChunk
  {
  public:
    Formatters* m_pFormatters; // using these formatters
  
    Stz* m_aNextRecord; // first byte of next record
  
  public:
    RecordExtractor(eCN eClassNo, Formatters* pFormatters);
    ~RecordExtractor(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);
  
  protected:
    bool  bFindRecordEnd(char** aaRecordEnd);
  
  private:
  };
  
  RecordExtractor* _pRecordExtractor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
