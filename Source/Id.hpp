/*============================================================================*/
/* RowId.hpp -- row id part of an index key                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Id
  {
  private:
  struct {
    u8 m_uBuffer : 48; // which buffer
    u8 m_uRow    : 16; // which row in buffer
  };
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Id*);
    
    Id(void);
    Id(Id& xId);
    ~Id(void);
    
    void operator =(Id& xId); // assignment
  
    void  vContentDump(void);
    
    u8    uBuffer(void);
    void  vBuffer(u8 uBuffer);
  
    void  vForm(u8 uBuffer, u8 uRow);
  
    u8    uRow(void);
    void  vRow(u8 uRow);

    void  vId(Id& xId);
    
    Row*  pRow(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
