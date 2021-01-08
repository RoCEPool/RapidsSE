/*============================================================================*/
/* StzList.hpp -- a list of null terminated strings                           */
/*============================================================================*/
#pragma once

//#include "ClassNumbers.hpp"
#include "ListExo_t.hpp"
#include "Stz.hpp"

namespace BASE
{
  typedef ListExo_t<CN_STZLIST, Stz> StzList;
  typedef ListExoIter_t<Stz> StzIter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
