/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class KeyWsp;
  
  class Row : public Object
  {
  private:
    i8  m_iTxId;       // transaction id
    i8  m_iDataBytes;  // number of data bytes (incl null teminator)
    // ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Row*);
    
    Row(i8 iDataBytes);
    ~Row(void);
    
    void  vContentDump(void);
    char* pData(void);
    i8    iDataBytes(void);
    void  vDataBytes(i8 iDataBytes);
    void  vGetKey(KeyWsp& xKeyWsp, IdxColVec* pColVec);
    i8    nSize(void);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
    
    static i8 iBytesRequired(Str* w);
    static i8 iBytesRequired(i8 iDataBytes);
    
    Footer* pFooter(void);
  };
  
  Row* _aRow(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
