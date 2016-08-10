/*============================================================================*/
/*  Skip.hpp - a list                                                         */
/*============================================================================*/
#pragma once

#include "DataStructures.h"

namespace DataStructures {
  
  typedef bool (*compare)(void* pSearchEntry, void* pListEntry, bool *bMatch);
  
  class Skip : public mn
  {
  protected:
    static const i4 MAX_LEVELS = 16;
    
    SkipNode* m_pHead;     // head of the list
    SkipNode* m_pTail;     // tail of the list

    bool      m_bIsUniqueKey; // is the key unique ?
    compare   m_fCompare;     // compare function
    
    i8        m_iObjects;  // number of objects in the list
    i4        m_iHeight;   // current max level in the list
    
  public:

    Skip(bool bIsUniqueKey, compare fCompare);
   ~Skip();
    
    bool  bExists (void* pObject);
    bool  bInsert (void* pObject);

    i8    iObjects();
    void* pPop    (bool*    bWasRemoved, i8 iFrom);
    bool  bRemove (void*    pObject);
    
    // iteration functions
    void* pFindFirst   (void*    pObject);
    
    void   vDump(void (*fvDump)(void* pObject));
    void   vCheck();
    void*  pEach   (SkipCursor* pCursor);
    
    SkipNode* pFirst();
    SkipNode* pNext(SkipNode* pPrev);
    SkipNode* pHead();
    SkipNode* pTail();
    
    bool   bDeleteAll(void);
    
  private:
    i4 iLevel();
    SkipNode* pPosition(void* Ent, bool* bEqualTo);
    SkipNode* pPosition(void* Ent, bool* bEqualTo, SkipNode** pRevise);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/