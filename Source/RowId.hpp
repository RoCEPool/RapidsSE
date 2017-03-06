/*============================================================================*/
/* RowId.hpp -- row id part of an index key                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RowId
  {
  private:
  struct {
    u8 m_uBuffer : 48; // which buffer
    u8 m_uRow    : 16; // which row in buffer
  };
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowId*);
    
    RowId(void);
    RowId(RowId& xRowId);
    ~RowId(void);
    
    void operator =(RowId& xRowId); // assignment
  
    void  vContentDump(void);
    
    u8    uBuffer(void);
    void  vBuffer(u8 uBuffer);
    
    u8    uRow(void);
    void  vRow(u8 uRow);

    void  vRowId(RowId& xRowId);
    
    Row*  pRow(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
