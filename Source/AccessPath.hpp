/*============================================================================*/
/* AccessPath.hpp - Access Path                                               */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SEitf;
}

namespace Plans
{
  
  class AccessPath : public Object
  {
  private:
    Table*     m_pTable;         // table
    Partition* m_pPartition;     // partition
    Index*     m_pIndex;         // index
  
    i8         m_iPathRating;    // rating
  
    eFT        m_eAccess;        // access technique
    PredicateVec*  m_pLowerPredicates;
    PredicateVec*  m_pUpperPredicates;
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    AccessPath(void);
    ~AccessPath(void);
  
    Table* pTable(void);
    void vTable(Table* pTable);
    
    Partition* pPartition(void);
    void vPartition(Partition* pPartition);
    
    Index* pIndex(void);
    void vIndex(Index* pIndex);
    
    i8 iPathRating(void);
    void vPathRating(i8 iPathRating);
  
    eFT eAccess(void);
    void vAccess(eFT eAccess);
  
    PredicateVec* pLowerPredicates(void);
    void vLowerPredicates(PredicateVec* pLowerPredicates);

    PredicateVec* pUpperPredicates(void);
    void vUpperPredicates(PredicateVec* pUpperPredicates);
    
    s48K& sWhereString(void);
    void vWhereString(s48K& sWhereString);
  
    Footer* aFooter(void);
  };
  
  AccessPath* _pAccessPath(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
