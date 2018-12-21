/*============================================================================*/
/* IoChunk.hpp -- I/O Chunk                                                   */
/*============================================================================*/
#pragma once

namespace Loader
{
  static const i8 C_IO_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IoChunk : public DataChunk
  {
  private:
    File m_xFile; // file being used
  
    bool  m_bStatistics;    // report statistics
    s32   m_sReportPrefix;  // report prefix
    i8    m_iReportQuantum; // how often to report at
  
    Timer m_xElapsedTimer;  // timer
    i8    m_nBytesSoFar;    // number of bytes read/written so far
    i8    m_nNextReport;    // next byte quantity to report at
  
    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IoChunk(void);
    ~IoChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    void  vExtrapolatedReport(void);
    void  vOpenRead(s256& sPath);
    void  vOpenWrite(s256& sPath);
    void  vPossiblyReport(void);
    i8    nReadBytes(i8 nBytes);
    void  vReportStatistics(void);
    void  vSetReporting(Stz* sPrefix, i8 iReportQuantum);
    i8    nWriteBytes(i8 nBytes);
    void  vWriteData(void);
    Stz*  aStr(void);
    
  private:
    Footer* aFooter(void);
  };
  
  IoChunk* _pIoChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
