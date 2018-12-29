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
    s256 m_sFilePath;    // file to be mapped
    i8   m_i_StartChunk; // seek location for the start of next chunk
    i8   m_i_EofOffset;  // eof location of file
    bool m_bMappingDone; // is mapping of chunks in file complete?
  
    Lock m_zChunksLock;  // lock over the chunks information
    struct {
      i8   iaOffset[C_LOADMAP_ENTRIES]; // array of ending '\n' offsets
      i8   nEntries;     // logical limit of iaOffset[]
      i8   i_Entry;      // which read-chunk to be taken by next subordinate
    } m_xChunks;
  
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadMap(Stz* aFilePath);
    ~LoadMap(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAddOffset(i8 i_Offset);
    void vChunkTest(void);
    void vCloseFile(void);
    void vDumpMap(void);
    bool bMapChunk(void);
    bool bMappingDone(void);
    void vMappingDone(bool bMappingDone);
    bool bNextChunk(i8& iSeek, i8& nBytes, i8& iChunk, i8& nChunks);
    bool bNextChunkInfo(i8& i_Seek, i8& nBytes);
    bool bOpenFile(void);

    void vStats(void);
    
    Footer* aFooter(void);
  };
  
  LoadMap* _pLoadMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
