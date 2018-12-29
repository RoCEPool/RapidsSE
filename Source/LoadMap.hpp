/*============================================================================*/
/* LoadMap.hpp -- Load Map                                                    */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class LoadMap : public Object
  {
    static const i8 C_LOADMAP_BLOCK   = 2048;
    static const i8 C_LOADMAP_ENTRIES = (C_1MB * 15) / sizeof(i8);
 
  private:
    i8   m_iEntries;   // logical limit of m_iEnd
    bool m_bDone;      // is map complete?
    
    s256 m_sFilePath;  // file path
    File m_oChunkFile; // from which file
    i8   m_iChunkNext; // seek before next read
    char m_xBlockTest[C_LOADMAP_BLOCK];
    i8   m_iChunkTell; //
    
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
