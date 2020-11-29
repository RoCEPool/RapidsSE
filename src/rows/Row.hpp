/*============================================================================*/
/* Row.hpp -- Computable form of Row                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Row : public Object
  {
  friend class RowSingle;
  friend class InfoData::InfoSetBase;
  friend class Steps::Step;
  friend class DataBuffers::RowBuffer;

  private:
    Dtm m_waColVal[1];
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(void);
    ~Row(void) noexcept(false);
  
    static i8 nSizeBytes(StreamDef* pStreamDef);
    void vDump(StreamDef* pStreamDef);
  
    Dtm* aColDtm(i8 i_Col);
    Dtm  wColDtm(i8 i_Col);
  
    char* azColVal_str(i8 i_Col);
    i8   iColVal_int(i8 i_Col);
    d8   dColVal_flt(i8 i_Col);
    Stz* szColVal_ofs(i8 i_Col);

    bool bIsColNull(i8 i_Col);
  
  private:
    void vColVal_int(i8 i_Col, i8 iVal);
    void vColVal_flt(i8 i_Col, d8 dVal);
    void vColVal_ofs(i8 i_Col, i8 iOfs);
  
    void vSetColNull(i8 i_Col);
  
  public:

  /*----------------------------------------------------------------------------*/
  /* get cell0 address                                                          */
  /*----------------------------------------------------------------------------*/
  inline m8* amCell0_rsb(void)
  {
    return (m8*) & m_waColVal[0];
  }

    
  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
//  inline bool bIsNull_rsb(i8 i_Col)
//  {
//    ASSERT(i_Col >= 0);
//
//    bool bIsNull = bIsColNull(i_Col);
//
//    return bIsNull;
//  }

  /*----------------------------------------------------------------------------*/
  /* get d8 float                                                               */
  /*----------------------------------------------------------------------------*/
//  inline d8 dVal_rsb(i8 i_Col)
//  {
//    ASSERT(i_Col >= 0);
//
//    d8 dVal = dColVal_flt(i_Col);
//
//    return dVal;
//  }

  /*----------------------------------------------------------------------------*/
  /* set d8 float                                                               */
  /*----------------------------------------------------------------------------*/
//  inline void vVal_rsb(i8 i_Col, d8 dVal)
//  {
//    ASSERT(i_Col >= 0);
//    
//  vColVal_flt(i_Col,dVal);
//   }
    
  /*----------------------------------------------------------------------------*/
  /* get i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline i8 iVal_rsb(i8 i_Col)
  {
    ASSERT(i_Col >= 0);
    
    i8 iVal = iColVal_int(i_Col);

    return iVal;
  }

  /*----------------------------------------------------------------------------*/
  /* set i8 integer                                                             */
  /*----------------------------------------------------------------------------*/
  inline void vVal_rsb(i8 i_Col, i8 iVal)
  {
    ASSERT(i_Col >= 0);
    
    vColVal_int(i_Col,iVal);
  }

  /*----------------------------------------------------------------------------*/
  /* get Stz* string                                                            */
  /*----------------------------------------------------------------------------*/
  inline Stz* szVal_rsb(i8 i_Col)
  {
    ASSERT(i_Col >= 0);

    i8 i_Ofs = iColVal_int(i_Col);
      
    Stz* szStz = aAdd<Stz*>(&m_waColVal[i_Col],i_Ofs);
        
    return szStz;
  }

  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
