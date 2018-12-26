/*============================================================================*/
/* RecordLoader.hpp -- Record Loader                                          */
/*============================================================================*/
#pragma once

namespace Chunks
{
  static const i8 C_LOAD_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordLoader : public Object
  {
  private:
    File m_oFile;        // file being loaded
    Stz* m_aEndChunk;    // last byte of where chunk ends
    Stz* m_aNextRecord;  // first byte of next record
    i8   m_iRecordBytes; // size of record
    bool m_bHaveRecord;  // record in work space?
  
    WspFxd<C_LOAD_CHUNK> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RecordLoader(void);
    ~RecordLoader(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    i8    iNextRecord(void);
    Stz*  aNextRecord(i8& iRecordBytes, Formatters* pFormatters);
    void  vOpenFile(s256& sPath);
    void  vPushBack(void);
    void  vReadChunk(i8 iSeek, i8 nBytes);
    
  private:
    void  vAdvanceRecordEnd(char** aSrc, Formatters* pFormatters);
    void  vReset(void);
  
    Footer* aFooter(void);
  };
  
  RecordLoader* _pRecordLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
