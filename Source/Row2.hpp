/*============================================================================*/
/* Row2.hpp -- Computable form of Row2                                        */
/*============================================================================*/
#pragma once

namespace Rows
{
  
  class Row2 : public Header
  {
  public:
    Dtm m_waColVal[1];
    
    // - - - - - - - - - - - - - - - - - -
    // footer -- must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row2(void);
    ~Row2(void) noexcept(false);
  
    static i8 nSizeBytes(StreamDef* pStreamDef);
    void vDump(StreamDef* pStreamDef);
    
//    void vCopy(RowSingle* pRow);
    
//    void vAdjustAddresses(RowSingle* pCopiedFromRow);
//    void vCopy(Row2* pRow);
    
//    void vCopy(Row2* pRow, i8 iFromCol, i8 iIntoCol);
//    i8   iHashColumn(Column* pCol);
//    void vSetValue(Datum* pDatum, Column* pCol);
    

  private:
    //Footer* aFooter(void);
  };
  
  Row2* _pRow2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
