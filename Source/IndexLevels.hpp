/*============================================================================*/
/* IndexLevels.hpp - set of index buffers by level for an index partition     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  // allow for maximum 16 levels per index
  static const i8 C_LEVELS_PER_INDEX = 16;
  static const i8 C_L1_ID            = 0xFFFF;
  
  class IndexLevels : public Object
  {
  private:
    Lock         m_zLock;          // lock covers all levels
    IndexBuffer* m_pIndexBuffer[C_LEVELS_PER_INDEX];
  
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexLevels(void);
    ~IndexLevels(void);
    
    IndexBuffer* pFirstBuffer(void);
    
    void   vInsertEntry(KeyWsp& xKeyWsp, Id xId, IdxColVec* pColVec);
    Entry* pNextEntry(Entry* pEntry);
    Entry* aPosition(Key& xpKey, RelOpAry* pOpAryLower, IdxColVec* pColumnVec);
    void   vTruncate(void);
    
  private:
    void vSplitBuffer(IndexBuffer* pIndexBuffer, IdxColVec* pColVec);
    void vInsertEntryL1(IndexBuffer* pIndexBuffer, IdxColVec* pColVec);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
