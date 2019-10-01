/*============================================================================*/
/* OpCatalog.hpp -- the catalog of op functions                               */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class OpCatalog
  {
  public:
    i8      m_nOpDefns;     // number of op definitions
    OpDefn  m_xOpDefn[128]; // array of op definitions
  
  public:
    OpDefn* aFindOp(Stz* szCommand);
    void    vHelp(eCN eClass);
    void    vPrintCatalog(void);
    void    vPrintDefn(OpDefn* aOpDefn);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
