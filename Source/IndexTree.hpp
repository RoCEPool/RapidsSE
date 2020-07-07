/*============================================================================*/
/* IndexTree.hpp -- Index Partition                                           */
/*============================================================================*/
#pragma once

namespace Indexing
{
  static const i8 C_HIGHER_LEVEL_ID = 0xFFFF;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
  class IndexTree : public Object
  {
  private:
    Lock      m_okLock;        // lock on the buffer tree
    Index*    m_pIndex;        // which index
    IndexDef* m_pIndexDef;     // index definition
    i8        m_iPartitionNo;  // which partition no
    
    EntBuffer* m_pRootEntBuffer;    // the root ent buffer

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexTree* pIndexTree);
    void  operator delete(void*);
    
    IndexTree(Index* pIndex, IndexDef* pIndexDef, i8 iPartitionNo);
   ~IndexTree(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorPortrait(s48K& sPortrait);
  
    Stz*       pAnalyze(void);
    void       vCompact(void);
    i8         iCountH0Entries(void);
    void       vDelete(EntBuffer* pEntryBuffer, Ent* pEnt);
    void       vDump(bool bSameHeight);
    i8         nEntBuffers(void);
    EntBuffer* pFirstH0Buffer(void);
    Ref        xwGetRef(Key* aFindKey);
    Index*     pIndex(void);
    IndexDef*  pIndexDef(void);
    void       vInsertEntries(CrwBuffer* pCrwBuffer);
    bool       bInsertEntry(Ent* aEnt);
    bool       bInsertEntry(Crw* aCrw, Ref& xRowRef);
    bool       bInsertEntry(CrwBuffer* pCrwBuffer, i8 i_Row, Ref& xRowRef);
    Ent*       aInsertPayloadEntry(EntWrk& xEntWrk, i8 nPayloadBytes);
    void       vLazyBuild(void);
    EntBuffer* pNewRootBuffer(void);
    EntBuffer* pPositionH0Buffer(Key* aFindKey);
    EntBuffer* pRootBuffer(void);
    void       vRootBuffer(EntBuffer* pRootBuffer);
    void       vTruncate(void);
    bool       bVerify(void);
    bool       bVerifyIndex(void);
    void       vVisualize(void);

    void       vExtractKey(EntWrk* pEntWrk, Crw* aCrw);
    void       vExtractKey(EntWrk* pEntWrk, Rsb* pRsb);
    void       vExtractKey(EntWrk* pEntWrk, CrwBuffer* pCrwBuffer, i8 i_Row);

    Footer*   aFooter(void);
    
  private:
    void       vDumpLevel(EntBuffer* pEntBuffer);
    EntBuffer* pFirstH1Buffer(void);
    EntBuffer* pPopSubList(EntBuffer** ppSubList);
  };
  
  IndexTree* _pIndexTree(void* pAddr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
