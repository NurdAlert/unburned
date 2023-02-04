#pragma once

#include "includes.h"
#include "menu/keybind.h"

struct c_config
{

	bool enable_box_esp = true;
	color box_esp_color{};
	bool enable_distance_esp = true;
	bool enable_name_esp = true;
	color name_esp_color{};
	bool enable_weapon_esp = true;
	color weapon_esp_color{};
	bool enable_health_esp = true;
	color health_esp_color{};
	bool enable_auto_healthbar_color = true;
	float max_esp_distance = 300.f;

	bool enable_item_esp = true;
	bool enable_automatic_item_coloring = true;

	bool enable_aimbot = true;
	CKeybind aimbot_bind{ "aimbot" };
	CKeybind menu_bind{ "menu", VK_INSERT };
	float aimbot_fov = 120.f;
	float aimbot_smoothing = 20.f;
	bool enable_smoothing = true;
	int aimbot_type = 0; // memory->mouse
	int aimbot_target_selection = 0; // fov->distance->auto
	bool enable_prediction = false;
	bool draw_fov = true;
	color fov_color{};
	bool aimbot_keep_target = true;

	bool no_recoil = false;
	bool no_spread = false;
	bool no_sway = false;
	bool instant_reload = false;
	bool always_day = true;
	bool instant_aim = false;
	bool sprint_in_shoot = false;
	bool no_bullet_drop = false;

	std::string username = "";
	
}; inline c_config config;