/*============================================================================*/
/* RowNo.hpp -- row number part of an index key                               */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowNo : public Part
  {
  private:
  struct {
    u8 m_uBuffer : 48; // which buffer
    u8 m_uRow    : 16; // which row in buffer
  };
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowNo*);
    
    RowNo(void);
    RowNo(RowNo& xRowNo);
    ~RowNo(void);
    
    void operator =(RowNo& xRowNo); // assignment
  
    void  vContentDump(void);
    
    u8    uBuffer(void);
    void  vBuffer(u8 uBuffer);
    
    u8    uRow(void);
    void  vRow(u8 uRow);

    void  vRowNo(RowNo& xRowNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
