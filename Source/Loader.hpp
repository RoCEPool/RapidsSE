/*============================================================================*/
/* Loader.hpp -- Parallel Table Loader                                        */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    static const i8 C_BLOCKTEST = 4096;
    
    Table* m_pTable;   // table being load
    File   m_xFile;    // from which file
    i8     m_iSeek;    // seek before next read
    TxRef* m_pTxRef;   // tx
    
    Lock   m_xLock;    // lock
    
    i8     m_nSubs;    // number of subordinates remaining

    //--

    SeekMap* m_pChunkMap;  // chunk map
    File     m_xChunkFile; // from which file
    i8       m_iChunkNext; // seek before next read
    char     m_xBlockTest[C_BLOCKTEST];
    i8       m_iChunkTell; //
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader* pLoader);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void   vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer);
    void   vCloseFile(void);
    File*  pFile(void);
    bool   bOpenFile(void);
    void   vPath(s& sPath);
    void   vSubs(i8 nSubs);
    bool   bSubCompleted(void);
    Table* pTable(void);
    void   vTable(Table* pTable);
    TxRef* pTxRef(void);
    void   vTxRef(TxRef* pTxRef);
    
    bool   bFetchChunk(LoadChunk* pLoadBuffer);
    bool   bInsertRow(i8 iPartn, char* pRecord, i8 iLength);
    bool   bNextChunk(i8& iSeek, i8& nBytes);
    
    bool   bFindChunk(void);
    
    void   vChunkTest(void);
    
    Footer* pFooter(void);
  };
  
  Loader* _pLoader(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/