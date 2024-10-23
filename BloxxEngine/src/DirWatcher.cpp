/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "DirWatcher.h"

#ifdef __linux__
#include <kqueue/sys/event.h>
#else
#include <event2/event.h>
#endif

#include <sys/stat.h>
#include <sys/utime.h>
#include <unistd>

namespace BloxxEngine {
} // BloxxEngine