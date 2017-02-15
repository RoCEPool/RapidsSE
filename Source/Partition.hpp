/*============================================================================*/
/* Partition.hpp - table partition                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Partition : public Object
  {
  private:
    i8         m_iPartitionId;   // partition number
    Data*      m_pData;
  
//Indexes:
//  Index*     m_pIndex[C_INDEX];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partition(void);
    ~Partition(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    static i8   iPartitionToNode(i8 iPartition);
    static i8   iPartitionToSlot(i8 iPartition);
    static i8   iSlotToPartition(i8 iSlot);

    i8          iPartitionId(void);
    void        vPartitionId(i8 iPartitionId);
  
    Data*       pData(void);
  
  private:
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
