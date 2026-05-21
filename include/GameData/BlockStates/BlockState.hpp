#pragma once
#include "../BlockIds.hpp"

struct BlockStateContainer;

struct BlockState {
private:
  bool outdated = false;

  friend class BlockStateContainer;

public:
  inline bool isOutdated() const { return outdated; }

  virtual ~BlockState() = default;
};
