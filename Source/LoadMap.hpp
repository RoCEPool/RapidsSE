/*============================================================================*/
/* LoadMap.hpp -- Load Map                                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  //
  // Important Note: CN_LOADMAP is allocated out of the CN_JUMBOTRANSIENT heap
  //
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class LoadMap : public DataChunk
  {
    static const i8 C_LOADMAP_BLOCK   = 2048;
    static const i8 C_LOADMAP_ENTRIES = (C_1MB * 1);
 
  private:
    i8   m_iEntries;    // logical limit of m_iEnd
    bool m_bDone;       // is map complete?
    
    s256 m_sFilePath;   // file path
    File m_oChunkFile;  // from which file
    i8   m_iNextChunk;  // seek before next read
    i8   m_iSeekEof;    //
    
    Lock m_zLock;      // subordinate lock
    i8   m_iNext;      // which read-chunk to be taken by next subordinate
    i8   m_iEolOffset[C_LOADMAP_ENTRIES]; // array of ending '\n' offsets
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadMap(Stz* aFilePath);
    ~LoadMap(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vDumpMap(void);
     
    void vCloseFile(void);
    bool bFindChunk(void);
    bool bOpenFile(void);

    void vEolOffset(i8 iEolOffset);
    bool bDone(void);
    void vDone(bool bDone);
    bool bNextChunk(i8& iSeek, i8& nBytes, i8& iChunk, i8& nChunks);
    bool bNextChunkInfo(i8& i_Seek, i8& nBytes);
    void vChunkTest(void);
    
    void vStats(void);
    
    Footer* aFooter(void);
  };
  
  LoadMap* _pLoadMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
