#include "menu.h"

struct tab
{
    const char* tab_name;
    int tab_id;
};

std::vector<tab> tabs{ {"aimbot", 0}, {"player esp", 1}, {"loot esp", 2}, {"miscellaneous", 3}, {"configs", 4} };
int current_tab = 0;

namespace ImGui
{
    bool Keybind(CKeybind* keybind, const ImVec2& size_arg = ImVec2(0, 0), bool clicked = false, ImGuiButtonFlags flags = 0)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        //SetCursorPosX(window->Size.x - 14 - size_arg.x);
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(keybind->get_name().c_str());
        const ImVec2 label_size = ImGui::CalcTextSize(keybind->get_name().c_str(), NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        bool value_changed = false;
        int key = keybind->key;

        auto io = ImGui::GetIO();

        std::string name = keybind->get_key_name();

        if (keybind->waiting_for_input)
            name = "waiting";

        if (ImGui::GetIO().MouseClicked[0] && hovered)
        {

            if (g.ActiveId == id)
            {

                keybind->waiting_for_input = true;

            }

        }
        else if (ImGui::GetIO().MouseClicked[1] && hovered) {
            OpenPopup(keybind->get_name().c_str());
        }
        else if (ImGui::GetIO().MouseClicked[0] && !hovered) {
            if (g.ActiveId == id)
                ImGui::ClearActiveID();
        }

        if (keybind->waiting_for_input)
            if (keybind->set_key())
            {
                ImGui::ClearActiveID();
                keybind->waiting_for_input = false;
            }

        // Render
        ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.f), 1.f);

        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(33 / 255.0f, 33 / 255.0f, 33 / 255.0f, 1.f));
        window->DrawList->AddRect(bb.Min, bb.Max, ImColor(0.f, 0.f, 0.f, 1.f));

        window->DrawList->AddText(bb.Min + ImVec2(size_arg.x / 2 - CalcTextSize(name.c_str()).x / 2, size_arg.y / 2 - CalcTextSize(name.c_str()).y / 2), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), name.c_str());

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
        SetNextWindowPos(pos + ImVec2(0, size_arg.y - 1));
        SetNextWindowSize(ImVec2(size_arg.x, 47 * 1.f));

        {

            if (BeginPopup(keybind->get_name().c_str(), window_flags)) {
                PushStyleVar(ImGuiStyleVar_Alpha, 1.f); {
                    SetCursorPos(ImVec2(7, 2)); {
                        BeginGroup(); {
                            if (Selectable("always on", keybind->type == CKeybind::ALWAYS)) keybind->type = CKeybind::ALWAYS;
                            if (Selectable("hold", keybind->type == CKeybind::HOLD)) keybind->type = CKeybind::HOLD;
                            if (Selectable("toggle", keybind->type == CKeybind::TOGGLE)) keybind->type = CKeybind::TOGGLE;
                        } EndGroup();
                    }
                } PopStyleVar();
                EndPopup();
            }
        }

        return pressed;
    }
    bool Colorpicker(const char* label, color* real_color, ImGuiColorEditFlags flags = NULL)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const float square_sz = GetFrameHeight();
        const float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
        const float w_inputs = w_full - w_button;
        const char* label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        auto col = &real_color->r;

        BeginGroup();
        PushID(label);

        // If we're not showing any slider there's no point in doing any HSV conversions
        const ImGuiColorEditFlags flags_untouched = flags;
        if (flags & ImGuiColorEditFlags_NoInputs)
            flags = (flags & (~ImGuiColorEditFlags__DisplayMask)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

        // Context menu: display and modify options (before defaults are applied)
        //if (!(flags & ImGuiColorEditFlags_NoOptions))
        //    ColorEditOptionsPopup(col, flags);

        flags |= ImGuiColorEditFlags_NoLabel;

        // Read stored options
        if (!(flags & ImGuiColorEditFlags__DisplayMask))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DisplayMask);
        if (!(flags & ImGuiColorEditFlags__DataTypeMask))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
        if (!(flags & ImGuiColorEditFlags__PickerMask))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
        if (!(flags & ImGuiColorEditFlags__InputMask))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputMask);
        flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__DisplayMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags__InputMask));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags__DisplayMask)); // Check that only 1 is selected
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags__InputMask));   // Check that only 1 is selected

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        // Convert to the formats we need
        float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
            // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            if (memcmp(g.ColorEditLastColor, col, sizeof(float) * 3) == 0)
            {
                if (f[1] == 0)
                    f[0] = g.ColorEditLastHue;
                if (f[2] == 0)
                    f[1] = g.ColorEditLastSat;
            }
        }
        int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

        bool value_changed = false;
        bool value_changed_as_float = false;

        const ImVec2 pos = window->DC.CursorPos;
        const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
        window->DC.CursorPos.x = pos.x + inputs_offset_x;

        flags |= ImGuiColorEditFlags_NoInputs;

        ImGui::PushFontShadow(IM_COL32_BLACK);

        if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            // RGB/HSV 0..255 Sliders
            const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
            const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
            static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
            static const char* fmt_table_int[3][4] =
            {
                {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
                { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
                { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
            };
            static const char* fmt_table_float[3][4] =
            {
                {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
                { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
                { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
            };
            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

            for (int n = 0; n < components; n++)
            {
                if (n > 0)
                    SameLine(0, style.ItemInnerSpacing.x);
                SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

                // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
                if (flags & ImGuiColorEditFlags_Float)
                {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                }
                else
                {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions))
                    OpenPopupOnItemClick("context");
            }
        }
        else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0)
        {
            // RGB Hexadecimal Input
            char buf[64];
            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
            SetNextItemWidth(w_inputs);
            if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed = true;
                char* p = buf;
                while (*p == '#' || ImCharIsBlankA(*p))
                    p++;
                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
                int r;
                if (alpha)
                    r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
                else
                    r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
                IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");
        }

        ImGuiWindow* picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
        {
            const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags))
            {
                if (!(flags & ImGuiColorEditFlags_NoPicker))
                {
                    // Store current color and open a picker
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
                }
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");

            if (BeginPopup("picker"))
            {
                picker_active_window = g.CurrentWindow;
                if (label != label_display_end)
                {
                    TextEx(label, label_display_end);
                    Spacing();
                }
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags__InputMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__DisplayMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
                value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
                ImGui::Checkbox("enable rainbow", &real_color->rainbow);
                ImGui::SliderFloat("rainbow speed", &real_color->rainbow_speed, 0.1f, 10.f);
                EndPopup();
            }
        }

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
        {
            const float text_offset_x = (flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + text_offset_x, pos.y + style.FramePadding.y);
            TextEx(label, label_display_end);
        }

        // Convert back
        if (value_changed && picker_active_window == NULL)
        {
            if (!value_changed_as_float)
                for (int n = 0; n < 4; n++)
                    f[n] = i[n] / 255.0f;
            if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
            {
                g.ColorEditLastHue = f[0];
                g.ColorEditLastSat = f[1];
                ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
                memcpy(g.ColorEditLastColor, f, sizeof(float) * 3);
            }
            if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }

        PopID();
        EndGroup();

        // Drag and Drop Target
        // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
        if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
        {
            bool accepted_drag_drop = false;
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512
                value_changed = accepted_drag_drop = true;
            }
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * components);
                value_changed = accepted_drag_drop = true;
            }

            // Drag-drop payloads are always RGB
            if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
            EndDragDropTarget();
        }

        // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
        if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
            window->DC.LastItemId = g.ActiveId;

        if (value_changed)
            MarkItemEdited(window->DC.LastItemId);

        ImGui::PopFontShadow();

        return value_changed;
    }
}

void AlignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

#include "../c_config.h"



void c_menu::render()
{

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGui::PushStyleColor(ImGuiCol_CheckMark, AccentColor.x());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, AccentColor.x());
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, AccentColor.x());

    ImGui::Begin("rellant", NULL, window_flags);
    {

        auto draw = ImGui::GetForegroundDrawList();
        auto size = ImGui::GetWindowSize();
        auto pos = ImGui::GetWindowPos();

        draw->AddRectFilled(pos + ImVec2(2, 2), pos + ImVec2(size.x - 2, 1), IM_COL32(74, 74, 74, 255));
        draw->AddRectFilled(pos + ImVec2(2, 3), pos + ImVec2(size.x - 2, 2), AccentColor.c());
        draw->AddRectFilled(pos + ImVec2(2, 4), pos + ImVec2(size.x - 2, 3), AccentColor.c());
        draw->AddRectFilled(pos + ImVec2(2, 5), pos + ImVec2(size.x - 2, 4), IM_COL32(74, 74, 74, 255));

        ImGui::BeginChild("tabs", ImVec2(85, size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::PushFontShadow(IM_COL32(0, 0, 0, 255));

        ImGui::PushStyleColor(ImGuiCol_Text, AccentColor.x());
        AlignForWidth(ImGui::CalcTextSize("// rellant").x);
        ImGui::Text("// rellant");
        ImGui::PopStyleColor();

        ImGui::Spacing();

        for (const auto& tab : tabs)
        {
            bool tmp = current_tab == tab.tab_id;
            if (ImGui::Selectable(tab.tab_name, &tmp)) current_tab = tab.tab_id;
        }

        ImGui::SetCursorPos(ImVec2(5, ImGui::GetWindowSize().y - 16));

        ImGui::Text("debug");

        ImGui::PopFontShadow();

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("content", ImVec2((size.x - 85) - 26, size.y - 15), true);

        switch (current_tab)
        {

        case 0: // aim
        {

            ImGui::Checkbox("enable aimbot", &config.enable_aimbot);
            ImGui::SameLine();
            ImGui::Keybind(&config.aimbot_bind, ImVec2(60, 16));
            ImGui::Combo("aimbot type", &config.aimbot_type, "memory\0mouse");
            ImGui::SliderFloat("aimbot fov", &config.aimbot_fov, 1.f, 600.f, "%.2f");
            ImGui::Checkbox("enable smoothing", &config.enable_smoothing);
            ImGui::SliderFloat("aimbot smoothness", &config.aimbot_smoothing, 0.f, 100.f, "%.2f");
            ImGui::Checkbox("draw aimbot fov", &config.draw_fov);
            ImGui::SameLine();
            ImGui::Colorpicker("fov color", &config.fov_color);
            ImGui::Checkbox("keep aimbot target", &config.aimbot_keep_target);
            break;

        }
        case 1: // player esp
        {

            ImGui::Checkbox("enable box esp", &config.enable_box_esp);
            ImGui::SameLine();
            ImGui::Colorpicker("box esp color", &config.box_esp_color);
            ImGui::Checkbox("enable health esp", &config.enable_health_esp);
            ImGui::SameLine();
            ImGui::Colorpicker("health esp color", &config.health_esp_color);
            ImGui::Checkbox("dynamic health colors", &config.enable_auto_healthbar_color);
            ImGui::Checkbox("enable weapon esp", &config.enable_weapon_esp);
            ImGui::SameLine();
            ImGui::Colorpicker("weapon esp color", &config.weapon_esp_color);
            ImGui::Checkbox("dynamic weapon colors", &config.enable_auto_weapon_color);
            ImGui::Checkbox("enable distance esp", &config.enable_distance_esp);
            ImGui::Checkbox("enable name esp", &config.enable_name_esp);
            ImGui::SameLine();
            ImGui::Colorpicker("name esp color", &config.name_esp_color);
            ImGui::SliderFloat("max esp distance", &config.max_esp_distance, 10.f, 5000.f);
            break;

        }
        case 2: // loot esp
        {
            break;
        }
        case 3: // misc
        {

            ImGui::Text("menu accent color");
            ImGui::SameLine();
            ImGui::Colorpicker("menu accent", &AccentColor, NULL);
            ImGui::Text("menu bind");
            ImGui::SameLine();
            ImGui::Keybind(&config.menu_bind, ImVec2(60, 16));
            if (ImGui::Button("exit", ImVec2(150, 30)))
                exit(0);

            break;

        }
        case 4: // configs
        {
            break;
        }

        }

        ImGui::EndChild();

    }
    ImGui::End();

    ImGui::PopStyleColor(3);

}

bool c_menu::initalize()
{

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImFontConfig cfg;
    cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 11.f, &cfg);

	return true;

}