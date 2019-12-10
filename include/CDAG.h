/**
 * File              : CDAG.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:51 MST
 * Last Modified Date: Lun 09 Dec 2019 15:14:58 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include <list>
#include <stdio.h>

namespace macveth {

class Node {};

typedef std::list<Node> NodeList;

class CDAG {
private:
  NodeList *NL;
};

} // namespace macveth
