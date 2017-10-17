/*============================================================================*/
/* RowId.hpp -- row id part of an index key                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class Id
  {
  private:
  struct {
    u8 m_uBuffer : 48; // which buffer                  could be: 24
    u8 m_uRow    : 16; // which row in buffer                   : 16
  };                   //                                         --
                       //                                         40 bits
  public:
    void* operator new(size_t);
    void* operator new(size_t, Id*);
    
    Id(void);
    Id(Id& xId);
    Id(u8 uBuffer, u8 uRow);
    ~Id(void);
    
    void operator =(Id& xId); // assignment
  
    i8    iAsI8(void);
  
    void  vContentDump(void);
    
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
  
    void  vForm(i8 iBuffer, i8 iRow);
  
    i8    iRow(void);
    void  vRow(i8 iRow);

    void  vId(Id& xId);
    
    Entry*  pRow(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
