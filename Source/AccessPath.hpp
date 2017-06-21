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
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    s32        m_sTenantName;    // name of tenant
    s32        m_sCatalogName;   // name of catalog
    s32        m_sSchemaName;    // name of schema
    s32        m_sTableName;     // name of table
    s32        m_sIndexName;     // name of index
  
    s32        m_sColumnName;    // name of column index
    Column*    m_pColumn;        // column
    eRO        m_eRelationalOp;  // operator
    ValReg     m_xValReg;        // value
  
    s48K       m_sWhereString;   // where string
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    AccessPath(void);
    ~AccessPath(void);
  
    void vInit(Interface::SEitf* pSEitf);
  
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
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    s32& sTenantName(void);
    void vTenantName(s32& sTenantName);

    s32& sCatalogName(void);
    void vCatalogName(s32& sCatalogName);
  
    s32& sSchemaName(void);
    void vSchemaName(s32& sSchemaName);
  
    s32& sTableName(void);
    void vTableName(s32& sTableName);

    s32& sIndexName(void);
    void vIndexName(s32& sIndexName);
  
    s32& sColumnName(void);
    void vColumnName(s32& sColumnName);
  
    Column* pColumn(void);
    void vColumn(Column* pColumn);
  
    eRO eRelationalOp(void);
    void vRelationalOp(eRO eRelationalOp);
  
    ValReg& xValReg(void);
    void vValReg(ValReg& xValReg);
  
    void vParse(void);
  
    void vGenPaths(void);
  
    Footer* aFooter(void);
  };
  
  AccessPath* _pAccessPath(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
