/*============================================================================*/
/* SkipList.hpp -- Skip List                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{
  typedef bool (*cmpFun)(void* pSearchEntry, void* pListEntry, bool *bMatch);
  
  class SkipList : public Object
  {
  public:
    SkipNode* m_pHead;      // head node of the list
    bool      m_bUniqueKey; // is the key unique ?
    cmpFun    m_fCompare;   // compare function
    i8        m_nObjects;   // number of objects in the list
    i8        m_iHeight;    // current max level in the list
    
    // must be last
    Footer m_xFooter;
    
  public:

    void* operator new(size_t);
    void* operator new(size_t, SkipList* pSkipList);
    void  operator delete(void*);
    
    SkipList(bool bUniqueKey, cmpFun fCompare);
   ~SkipList(void);
    
    void  vCheck(void);
    void  vContents(void (*fvDump)(void* pObject));
    bool  bDeleteAll(void);
    void* pEach(SkipCursor* pCursor);
    bool  bExists(void* pObject);
    void* pFind(void* pObject);
    SkipNode* pFirst(i8 iLevel);
    bool  bInsert(void* pObject);
    SkipNode* pNext(SkipNode* pPrev, i8 iLevel);
    i8    nObjects(void);
    bool  bRemove(void* pObject);
  
  private:
    i8 iRandomLevel(void);
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
