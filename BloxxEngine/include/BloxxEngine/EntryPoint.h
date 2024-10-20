/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#include "BloxxEngine/Engine.h"



static BE::Engine *s_App = nullptr;

int main(int argc, char **argv)
{
    BE::CreateEngine();
    s_App->Run();
    return 0;
}