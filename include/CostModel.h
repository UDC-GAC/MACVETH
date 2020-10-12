#ifndef COSTMODEL_H
#define COSTMODEL_H

#include "include/StmtWrapper.h"
#include <map>

namespace macveth {

class CostModel {
public:
  enum VectorizationType { NONE, PARTIAL, FULL };

  using MVCost = std::map<std::string, long>;

  static long computeSequentialCostStmtWrapper(std::list<StmtWrapper *> SL);

private:
  CostModel *_instance;
};

} // namespace macveth

#endif