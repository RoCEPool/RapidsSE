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
  class LoadMap : public FileChunk
  {
    static const i8 C_LOADMAP_BLOCK   = 2048;
    static const i8 C_LOADMAP_ENTRIES = (C_1MB * 1);
 
  private:
    s256 m_sFilePath;    // file path
    i8   m_iChunkOffset; // seek location of next chunk
    i8   m_iEofOffset;   // eof location of eof
    bool m_bMappingDone; // is mapping of chunks in file complete?
  
    Lock m_zEolLock;    // lock over the offset information
    
    i8   m_iEolOffset[C_LOADMAP_ENTRIES]; // array of ending '\n' offsets
    i8   m_nEntries;    // logical limit of m_iEnd
    i8   m_iNext;       // which read-chunk to be taken by next subordinate

    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadMap(Stz* aFilePath);
    ~LoadMap(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vDumpMap(void);
     
    void vCloseFile(void);
    bool bMapChunk(void);
    bool bOpenFile(void);

    void vEolOffset(i8 iEolOffset);
    bool bMappingDone(void);
    void vMappingDone(bool bMappingDone);
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
