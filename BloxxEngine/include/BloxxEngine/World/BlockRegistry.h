

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Block.h"

#include <map>
#include <string>

namespace BloxxEngine {

class BlockTypeRegistry {
    public:
      BlockTypeRegistry() = default;

private:
    std::map<std::string, Block*> blocks;

};

} // BloxxEngine
