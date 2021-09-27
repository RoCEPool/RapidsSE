/*============================================================================*/
/* SkipListNL.hpp -- Skip List                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  typedef bool (*CompareFp)(void* aSearchEntry, void* pListEntry, bool& bMatch);
  typedef void (*PrintFp)(void* aItem);

  static const bool C_UNQIUE_KEY     = true;
  static const bool C_NON_UNQIUE_KEY = false;
  
  class SkipListNL : public Object
  {
  public:
    bool      m_bUniqueKey; // is the key unique ?
    CompareFp m_fpCompare;  // compare function
    PrintFp   m_fpPrint;    // print function

    i8        m_nItems;     // number of objects in the list
    i8        m_iHeight;    // current max level in the list
    SkipNode* m_pHead;      // head node of the list

    SkipNode* m_pNewly;     // most recently new'd skip node
  
    SlabAllocator* m_pSlabAllocator; // optional slab allocator
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SkipListNL* pSkipList);
    void  operator delete(void*);
    
    SkipListNL(bool bUniqueKey,
               CompareFp fpCompare,
               PrintFp fpPrint= nullptr,
               bool bSlabAllocator=false);
   ~SkipListNL(void) noexcept(false);
    
    void  vCheck(void);
  
    bool  bCompareAddr(void* aInsert, void* aScan, bool& bMatch);

    void  vContents(void (*fvDump)(void* pObject));
    bool  bDeleteAll(void);
    bool  bExists(void* pObject);
  
    void* pFind(void* pObject,
                bool bTrace=false,
                CompareFp fpCompare_alt = nullptr);
  
    void* pFindByKey(std::function<bool(void* aScan,
                                        bool& bFound
                                        )> bfMoveRightTest
                     );
    SkipNode* pFirst(i8 iLevel);
  
  
    bool bInsert(void* pObject, bool bTrace=false);
  
    bool bInsertByKey(void* pObject,
                      std::function<bool(void* aRightObject,
                                         bool& bFound
                                         )> bfMoveRightTest
                      );

  
    i8    nItems(void);
    SkipNode* pNewly(void);
    SkipNode* pNext(SkipNode* pPrev, i8 iLevel);
    void* pPop(void);
    i8    nRecount(void);
  
    bool  bRemove(void* pObject);

    bool  bRemoveByKey(void* pObject,
                       std::function<bool(void* aRightObject,
                                          bool& bFound
                                          )> bfMoveRightTest
                       );

  private:
    i8 iRandomLevel(void);
  
    SkipNode* pPosition(void* Ent,
                        bool& bEqualTo,
                        SkipNode** pRevise,
                        bool bTrace,
                        CompareFp fpCompare
                        );
  
    SkipNode* pLocateKey(std::function<bool(void* aRightObject,
                                            bool& bFoundKey
                                           )> bfMoveRightTest
                         );

    SkipNode* pPositionPrior(std::function<bool(void* aRightObject,
                                                bool& bFoundKey
                                                )> bfMoveRightTest,
                             SkipNode** paPriorVec,
                             bool& bFoundInList
                             );

    void vValidate(void);
  
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
