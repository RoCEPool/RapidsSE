/*============================================================================*/
/* SkipListNL.hpp -- Skip List                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  typedef bool (*CompareFp)(void* pSearchEntry, void* pListEntry, bool& bMatch);
  
  static const bool C_UNQIUE_KEY     = true;
  static const bool C_NON_UNQIUE_KEY = false;
  
  class SkipListNL : public Object
  {
  public:
    SkipNode* m_pHead;      // head node of the list
    bool      m_bUniqueKey; // is the key unique ?
    CompareFp m_fpCompare;  // compare function
    i8        m_nObjects;   // number of objects in the list
    i8        m_iHeight;    // current max level in the list
  
    SlabAllocator* m_pSlabAllocator; // optional slab allocator
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SkipListNL* pSkipList);
    void  operator delete(void*);
    
    SkipListNL(bool bUniqueKey, CompareFp fpCompare, bool bSlabAllocator=false);
   ~SkipListNL(void) noexcept(false);
    
    void  vCheck(void);
    void  vContents(void (*fvDump)(void* pObject));
    bool  bDeleteAll(void);
    bool  bExists(void* pObject);
    void* pFind(void* pObject);
    SkipNode* pFirst(i8 iLevel);
    bool  bInsert(void* pObject);
    SkipNode* pNext(SkipNode* pPrev, i8 iLevel);
    i8    nObjects(void);
    void* pPop(void);
    i8    nRecount(void);
    bool  bRemove(void* pObject);
  
  private:
    i8 iRandomLevel(void);
    SkipNode* pPosition(void* Ent, bool& bEqualTo, SkipNode** pRevise);
  
    Footer* aFooter(void);
  };
  
  SkipListNL* _pSkipList(void* aAddr);
  
  void vSkipListTest(void);
  void vSkipListDump(SkipListNL* pSkipList);
}
bool bI8Compare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
