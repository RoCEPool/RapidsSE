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
    i8   m_i_EofLoc;     // eof location of file
    i8   m_i_ChunkLoc;   // seek location for the start of next chunk
    bool m_bMappingDone; // is mapping of chunks in file complete?
  
    Lock m_zMapLock;     // lock over the following map information
    struct {
      i8   iaEndLoc[C_LOADMAP_ENTRIES]; // array of chunk end file locations
      i8   nEntries;     // logical limit of iaEndLoc[]
      i8   i_Entry;      // which read-chunk to be taken by next subLoader
    } m_xMap;
  
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadMap(Stz* aFilePath);
    ~LoadMap(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vCloseFile(void);
    void vDumpMap(void);
    bool bMapChunk(void);
    bool bMappingDone(void);
    void vMappingDone(bool bMappingDone);
    bool bNextChunk(i8& i_Seek, i8& nBytes);
    bool bOpenFile(void);
    void vStats(void);
  
  private:
    void vAddEntry(i8 i_ChunkEndLoc);
    bool bGetNextMapping(i8& i_Seek, i8& nBytes, i8& i_Entry);
    void vTestMap(void);

    Footer* aFooter(void);
  };
  
  LoadMap* _pLoadMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
