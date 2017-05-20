/*============================================================================*/
/* IndexData.hpp - index portion of table partition                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{

  static const i8 C_HIGHER_LEVEL_ID = 0xFFFF;
  
  class Table;
  class Partition;
  class Index;
  
  class IndexData : public Object
  {
  private:
    Lock         m_zLock;
    Index*       m_pIndex;
    IndexBuffer* m_pRoot;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexData* pIndexData);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    IndexData(Index* pIndex);
    ~IndexData(void);
  
    Str*         pAnalyze(void);
    void         vDump(void);
    IndexBuffer* pFirstBuffer();
    Index*       pIndex(void);
    i8           nIndexBuffers(void);
    void         vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo);
    IndexBuffer* aPosition(KeyWsp& xKeyWsp, eJC eJuxaCompare, i8 iLevel);
    IndexBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    
    Footer*      pFooter(void);
    
  private:
    void         vFixHigherBuffer(IndexBuffer* pIndexBuffer, i8 iLevel);
    void         vInsertEntry(IndexBuffer* pRoot,KeyWsp& xKeyWsp, Id xId, i8 iLevel);
    void         vInsertFirstEntry(IndexBuffer* pToBuffer, IndexBuffer* pFromBuffer);
    IndexBuffer* pSplitBuffer(IndexBuffer* pIndexBuffer);
  };
  
  IndexData* _pIndexData(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
