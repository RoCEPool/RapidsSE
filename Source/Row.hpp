/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class KeyWsp;
  
  class Row : public BufferItem
  {
  private:
  //i8  m_iTxId;       // transaction id
    // ..data..
    // ..footer..

  public:
    void* operator new(size_t);
    void* operator new(size_t, Row*);
    
    Row(i8 iDataBytes);
    ~Row(void);
    
    void  vContentDump(void);
    char* aRowData(void);
    void  vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pColVec);
    i8    nSize(void);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
    
    
    Footer* pFooter(void);
  };
  
  Row* _aRow(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
