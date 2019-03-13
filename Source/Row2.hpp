/*============================================================================*/
/* Row2.hpp -- Row2                                                           */
/*============================================================================*/
#pragma once

namespace Data
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowData2
  {
  private:
    union {
      struct {
        u8 m_uExtention : 16 ; // 0..65535
        u8 m_uCol0      : 3 ;  // 0..7
        u8 m_uCol1      : 3 ;  // 0..7
        u8 m_uCol2      : 3 ;  // 0..7
        u8 m_uCol3      : 3 ;  // 0..7
        u8 m_uCol4      : 3 ;  // 0..7
        u8 m_uCol5      : 3 ;  // 0..7
        u8 m_uCol6      : 3 ;  // 0..7
        u8 m_uCol7      : 3 ;  // 0..7
        u8 m_uCol8      : 3 ;  // 0..7
        u8 m_uCol9      : 3 ;  // 0..7
        u8 m_uCol10     : 3 ;  // 0..7
        u8 m_uCol11     : 3 ;  // 0..7
        u8 m_uCol12     : 3 ;  // 0..7
        u8 m_uCol13     : 3 ;  // 0..7
        u8 m_uCol14     : 3 ;  // 0..7
        u8 m_uCol15     : 3 ;  // 0..7
        //            ---
        //             64
        //            ===
      } x;
      u8 m_uMetaData;
    };
    // ...data...

  public:
    void* operator new(size_t);
  
    RowData2(void);
    ~RowData2(void) noexcept(false);
  
    
  private:
   
  };
  
  RowData2* _aRowData2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
