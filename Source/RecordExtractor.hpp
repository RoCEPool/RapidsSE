/*============================================================================*/
/* RecordExtractor.hpp -- Record Extractor                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordExtractor : public FileChunk
  {
  protected:
    Formatters* m_pFormatters; // using these formatters
  
    Stz* m_aEndChunk;          // last byte of where chunk ends
    i8   m_nRemainingBytes;    // bytes remaining

    Stz* m_aNextRecord;        // first byte of next record
    i8   m_nRecordBytes;       // size of record
  
  public:    
    RecordExtractor(eCN eClassNo, Formatters* pFormatters);
    ~RecordExtractor(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);
    bool  bOpenFile(s256& sPath);
    bool  bReadChunk(void);
    void  vReadChunk(i8 iSeek, i8 nBytes);

  protected:
    void  vReset(void);

  private:
    void  vCopyDown(void);
    bool  bFindRecordEnd(char** aaRecordEnd);
  };
  
  RecordExtractor* _pRecordExtractor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
