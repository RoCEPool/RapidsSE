/*============================================================================*/
/* RowOnce.hpp -- Row Once                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RowOnce : public Object
  {
  private:
  
    Lock      m_xLock;
    SkipList* m_pSkipList; // skip list containing seen rowids
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowOnce(void);
    ~RowOnce(void);
  
    bool bRowSeen(Id xRowId);
    bool bRowSeen2(Id xRowId);
  
    Footer* aFooter(void);  
  };
  
  RowOnce* _pRowOnce(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
