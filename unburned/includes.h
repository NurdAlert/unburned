#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>
#include <codecvt>
#include <unordered_map>
#include <sstream>
#include <mutex>
#include <intrin.h>

#include <windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "ImGui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_freetype.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGuiRender/ImGuiRender.h"
#include "Overlay/Overlay.h"

inline std::vector<struct color*> registered_colors{};

struct color
{
    float r, g, b, a;
    bool rainbow = false;
    float rainbow_speed = 1.f;
    color()
    {
        r = 1.f, g = 1.f, b = 1.f, a = 1.f;
        registered_colors.push_back(this);
    }
    color(float _r, float _g, float _b, float _a)
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
        this->a = _a;
        registered_colors.push_back(this);
    }
    ImVec4 x()
    {
        return ImVec4(r, g, b, a);
    }
    ImU32 c()
    {
        return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
    }
    ImU32 c(int m)
    {
        return IM_COL32((r * 255.f) - m, (g * 255.f) - m, (b * 255.f) - m, (a * 255.f) - m);
    }
};