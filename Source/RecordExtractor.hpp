/*============================================================================*/
/* RecordExtractor.hpp -- Record Extractor                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordExtractor : public FileChunk
  {
  public:
    Formatters* m_pFormatters; // using these formatters
  
    Stz* m_aNextRecord;        // first byte of next record
    i8   m_nRecordBytes;       // size of record
  
  public:    
    RecordExtractor(eCN eClassNo, Formatters* pFormatters);
    ~RecordExtractor(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);
    bool  bOpenFile(s256& sPath);
    bool  bReadNextChunk(void);
    void  vReadSpecificChunk(i8 iSeek, i8 nBytes);

    void  vCopyDown(void);
  protected:
    bool  bFindRecordEnd(char** aaRecordEnd);
    void  vReset(void);

  private:
  };
  
  RecordExtractor* _pRecordExtractor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
