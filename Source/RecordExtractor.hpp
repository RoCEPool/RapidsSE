/*============================================================================*/
/* RecordExtractor.hpp -- Record Extractor                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordExtractor_NEW : public DataChunk
  {
  public:
    Formatters* m_pFormatters; // using these formatters
  
    Stz* m_aNextRecord;        // first byte of next record
    i8   m_nRecordBytes;       // size of record
  
  public:
    RecordExtractor_NEW(eCN eClassNo, Formatters* pFormatters);
    ~RecordExtractor_NEW(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);
  
    protected:
    bool  bFindRecordEnd(char** aaRecordEnd);
  
  private:
  };
  
  RecordExtractor_NEW* _pRecordExtractor_NEW(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
