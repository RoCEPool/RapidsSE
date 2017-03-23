/*============================================================================*/
/* SkipList.hpp -- Skip List                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{
  typedef bool (*compare)(void* pSearchEntry, void* pListEntry, bool *bMatch);
  
  class SkipList : public Object
  {
  public:
    
    SkipNode* m_pHead;     // head of the list
    SkipNode* m_pTail;     // tail of the list

    bool      m_bIsUniqueKey; // is the key unique ?
    compare   m_fCompare;     // compare function
    
    i8        m_iObjects;  // number of objects in the list
    i4        m_iHeight;   // current max level in the list
    
    // must be last
    Footer m_xFooter;
    
  public:

    void* operator new(size_t);
    void* operator new(size_t, SkipList* pSkipList);
    void  operator delete(void*);
    
    SkipList(bool bIsUniqueKey, compare fCompare);
   ~SkipList();
    
    bool  bExists (void* pObject);
    bool  bInsert (void* pObject);

    i8    iObjects();
    void* pPop    (bool*    bWasRemoved, i8 iFrom);
    bool  bRemove (void*    pObject);
    
    // iteration functions
    void* pFindFirst   (void*    pObject);
    
    void   vItemDump(void (*fvDump)(void* pObject));
    void   vCheck();
    void*  pEach   (SkipCursor* pCursor);
    
    SkipNode* pFirst(i8 iLevel);
    SkipNode* pNext(SkipNode* pPrev, i8 iLevel);
    SkipNode* pHead();
    SkipNode* pTail();
    
    bool   bDeleteAll(void);
    
  private:
    i4 iLevel();
    SkipNode* pPosition(void* Ent, bool* bEqualTo);
    SkipNode* pPosition(void* Ent, bool* bEqualTo, SkipNode** pRevise);
    
    Footer* pFooter(void);
    
  };
  
  SkipList* _pSkipList(void* aAddr);
  
  void vSkipListTest(void);
  void vSkipListDump(SkipList* pSkipList);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
