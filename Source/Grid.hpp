/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/

#pragma once

#include "GridModel.h"

namespace GridModel
{
  class Grid : public Object,
               public List_t<Node>
  {
  private:
    bool   m_bHasStarted;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Grid* pGrid);
    void  operator delete(void*);
    
    Grid(void);
    ~Grid(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    void vHasStarted(bool bHasStarted);
    bool bHasStarted(void);
    void vInit(void);
    
    Footer* pFooter(void);
  };
    
  Grid* _pGrid(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
