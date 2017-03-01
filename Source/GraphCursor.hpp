/*============================================================================*/
/* GraphCursor.hpp -- Graph Cursor                                            */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class GraphCursor : public Object
  {
  private:
    Graph*     m_pGraph;     // graph
    GraphNode* m_pGraphNode; // graph node
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, GraphCursor* pGraphCursor);
    void  operator delete(void* pGraphCursorAddress);
 
    GraphCursor(void);
    ~GraphCursor(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
  private:
    
    Footer* pFooter(void);
  };
  
  GraphCursor* _pGraphCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
