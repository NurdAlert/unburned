#include "c_memory.h"
#include "mono.h"
#include "game.h"
#include "menu/menu.h"
#include "c_config.h"
#include "c_features.h"

//#define MENU_TESTING

COverlay* c_overlay = new COverlay();
bool clickable = false;

void change_click(bool canclick)
{
	long style = ::GetWindowLong(c_overlay->GetLocalHwnd(), GWL_EXSTYLE);
	if (canclick)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLong(c_overlay->GetLocalHwnd(), GWL_EXSTYLE, style);
		SetFocus(c_overlay->GetLocalHwnd());
		clickable = true;
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLong(c_overlay->GetLocalHwnd(), GWL_EXSTYLE, style);
		SetFocus(c_overlay->GetLocalHwnd());
		clickable = false;
	}
}

void entity_thread()
{

	while (true)
	{

		auto provider = provider_t::get_instance();

		if (provider && provider->is_connected())
		{

			auto local_player = player_t::get_local();

			if (local_player)
			{

				auto clients = provider->clients();
				auto client_list = memory.read_vec<steam_player_t*>(clients->list(), clients->size());

				if (!clients)
					continue;

				std::vector<cached_player_t> tmp_cache{};

				cached_player_t cached_local{};
				cached_local.player = local_player;
				cached_local.playerlife = local_player->life();
				cached_local.playerlook = local_player->look();
				cached_local.transform = local_player->game_object()->transform();
				cached_local.equipment = local_player->equipment();

				for (const auto& client : client_list)
				{

					if (!client)
						continue;

					auto player = client->player();

					if (!player)
						continue;

					if (player == local_player)
					{

						cached_local.steamplayer = client;
						continue;

					}

					auto equipment = player->equipment();

					tmp_cache.emplace_back<cached_player_t>({ client, player, player->life(), player->look(), nullptr, player->movement(), player->game_object()->transform(), equipment, client->player_id()->character_name(), equipment->asset()->item_name() });

				}

				cheat::sync.lock();
				cheat::players.clear();
				cheat::players = tmp_cache;
				cheat::local_player = cached_local;
				cheat::sync.unlock();

				tmp_cache.clear();

			}

		}
		else
		{
			cheat::sync.lock();
			cheat::players.clear();
			cheat::sync.unlock();
		}

		Sleep(1500);

	}

}

inline void DrawBox(ImVec2 pos, ImVec2 size, ImColor color)
{
	const ImRect rect_bb(pos, pos + size);

	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min - ImVec2(1, 1), rect_bb.Max + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, color.Value.w));
	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min + ImVec2(1, 1), rect_bb.Max - ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, color.Value.w));
	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min, rect_bb.Max, color);
}

inline void DrawHealth(int health, ImVec2 pos, ImVec2 size, float alpha, bool dormant)
{

	int max_health = 100;
	//health = std::clamp(health, 0, 100);
	float health_ratio = health / (float)max_health;

	int offset = size.x - 2;
	offset -= (offset * health_ratio);

	ImColor color = dormant ? ImColor(52, 204, 235, int(200 * alpha)) : ImColor(int(244 - (116 * health_ratio)), int(100 + (144 * health_ratio)), 66, int(220 * alpha));
	if (!config.enable_auto_healthbar_color)
		color = config.health_esp_color.c();

	ImRenderer->DrawFilledRect(ImVec2(pos.x - 5, pos.y), ImVec2(4, size.x), ImColor(0, 0, 0, int(130 * alpha)));
	ImRenderer->DrawFilledRect(ImVec2(pos.x - 4, pos.y + 1 + offset), ImVec2(2, size.x - 2 - offset), color);

}

void MoveMouse(int x, int y, int width, int height)
{

	float ScreenCenterX = c_overlay->m_pWidth / 2;
	float ScreenCenterY = c_overlay->m_pHeight / 2;

	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			if (config.enable_smoothing)
				TargetX /= config.aimbot_smoothing;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			if (config.enable_smoothing)
				TargetX /= config.aimbot_smoothing;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			if (config.enable_smoothing)
				TargetY /= config.aimbot_smoothing;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			if (config.enable_smoothing)
				TargetY /= config.aimbot_smoothing;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

}
//

#include <numbers>

void do_rainbow(color* col) noexcept
{
	if (!col->rainbow)
		return;
	constexpr float pi = std::numbers::pi_v<float>;
	auto r_rain = std::sin(col->rainbow_speed * ImGui::GetTime()) * 0.5f + 0.5f;
	auto g_rain = std::sin(col->rainbow_speed * ImGui::GetTime() + 2 * pi / 3) * 0.5f + 0.5f;
	auto b_rain = std::sin(col->rainbow_speed * ImGui::GetTime() + 4 * pi / 3) * 0.5f + 0.5f;
	col->r = r_rain;
	col->g = g_rain;
	col->b = b_rain;
}

void AddRadialGradient(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out)
{
	if (((col_in | col_out) & IM_COL32_A_MASK) == 0 || radius < 0.5f)
		return;

	// Use arc with automatic segment count
	draw_list->_PathArcToFastEx(center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
	const int count = draw_list->_Path.Size - 1;

	unsigned int vtx_base = draw_list->_VtxCurrentIdx;
	draw_list->PrimReserve(count * 3, count + 1);

	// Submit vertices
	const ImVec2 uv = draw_list->_Data->TexUvWhitePixel;
	draw_list->PrimWriteVtx(center, uv, col_in);
	for (int n = 0; n < count; n++)
		draw_list->PrimWriteVtx(draw_list->_Path[n], uv, col_out);

	// Submit a fan of triangles
	for (int n = 0; n < count; n++)
	{
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base));
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + n));
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + ((n + 1) % count)));
	}
	draw_list->_Path.Size = 0;
}

unity::vec3 calculate_angle(const unity::vec3& origin, const unity::vec3& dest)
{
	unity::vec3 diff = origin - dest;
	unity::vec3 ret;

	float length = diff.length();
	ret.y = asinf(diff.y / length);
	ret.x = -atan2f(diff.x, -diff.z);

	return ret * 57.29578f;
}

int main_thread()
{

#ifndef MENU_TESTING
	if (!memory.init())
	{
		std::cout << "failed to init memory.\n";
		return 1;
	}
#endif

#ifdef MENU_TESTING
	memory.window = FindWindowA(NULL, "Unturned");
#endif

#ifndef MENU_TESTING
	memory.mono = memory.get_module(L"mono-2.0-bdwgc.dll");
	memory.unity = memory.get_module(L"UnityPlayer.dll");
#endif

	ImRenderer = new GRenderer();
	if (!c_overlay->InitWindows(memory.window))
		return 1;

	SetWindowLongA(c_overlay->GetLocalHwnd(), GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

#ifndef MENU_TESTING
	//mono::init();
	unity_classes::init();
	class_offsets::init();
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(entity_thread), NULL, NULL, NULL);
#endif

	RECT OldRc{};
	MSG msg{};

#ifdef _DEBUG
	std::cout << "Unturned base: " << std::hex << memory.base << "\nMono base: " << std::hex << memory.mono << "\n";
#endif

#define CREATELOG(x, y, z) auto x = features::c_logged_feature(y, z)

	CREATELOG(name_esp, "name esp", &config.enable_name_esp);
	CREATELOG(aimbot_log, "aimbot key", &config.aimbot_bind.enabled);
	CREATELOG(box_esp, "box esp", &config.enable_box_esp);
	CREATELOG(distance_esp, "distance esp", &config.enable_distance_esp);
	CREATELOG(weapon_esp, "weapon esp", &config.enable_weapon_esp);
	CREATELOG(no_sway, "no sway", &config.no_sway);
	CREATELOG(sprint_and_aim, "sprint and aim", &config.sprint_in_shoot);
	CREATELOG(instant_aim, "instant aim", &config.instant_aim);
	CREATELOG(no_bullet_drop, "no bullet drop", &config.no_bullet_drop);

	while (msg.message != WM_QUIT)
	{

#ifndef MENU_TESTING
		if (!FindWindowA(NULL, "Unturned"))
			exit(0);
#endif

		for (const auto& keybind : keybinds)
			keybind->update();

		menu.opened = config.menu_bind.enabled;

		if (PeekMessageA(&msg, c_overlay->GetLocalHwnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		HWND ForegaundWind = GetForegroundWindow();
		if (memory.window == ForegaundWind)
		{
			HWND GetWind = GetWindow(ForegaundWind, GW_HWNDPREV);
			SetWindowPos(c_overlay->GetLocalHwnd(), GetWind, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT Rect{};
		POINT Cord{};
		GetClientRect(memory.window, &Rect);
		ClientToScreen(memory.window, &Cord);
		Rect.left = Cord.x;
		Rect.top = Cord.y;

		if (Rect.left != OldRc.left || Rect.right != OldRc.right || Rect.top != OldRc.top || Rect.bottom != OldRc.bottom)
		{
			OldRc = Rect;
			c_overlay->m_pWidth = (float)Rect.right;
			c_overlay->m_pHeight = (float)Rect.bottom;
			SetWindowPos(c_overlay->GetLocalHwnd(), 0, Cord.x, Cord.y, int(Rect.right + 2.f), int(Rect.bottom + 2.f), SWP_NOREDRAW);
			ImGui_ImplDX9_InvalidateDeviceObjects();
			g_pd3dDevice->Reset(&g_d3dpp);
			ImGui_ImplDX9_CreateDeviceObjects();
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

#ifndef MENU_TESTING
		auto camera = main_camera_t::get_instance()->instance();

		if (camera)
		{
			auto camera_obj = memory.read<uintptr_t>(camera + 0x10);

			unity::camera.position = memory.read<unity::vec3>(camera_obj + 0x42C);
			unity::camera.matrix = memory.read<unity::vmatrix_t>(camera_obj + 0x2E4);
		}
#endif

		for (const auto& color : registered_colors)
			do_rainbow(color);

		if (menu.opened)
		{
			if (clickable != true)
				change_click(true);

			menu.render();

		}
		else
		{
			if (clickable != false)
				change_click(false);
		}

		std::stringstream ss;
		ss << "rellant.dev / fps: " << static_cast<int>(ImGui::GetIO().Framerate);
		ImGui::SetNextWindowPos(ImVec2(5, 5));
		ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(ss.str().c_str()).x + 15, 25));
		ImGui::Begin("watermark", NULL, menu.window_flags);
		ImGui::TextColored(menu.AccentColor.x(), ss.str().c_str());
		ImGui::End();

		if (config.draw_fov)
		{
			static auto center = ImVec2(c_overlay->m_pWidth / 2, c_overlay->m_pHeight / 2);
			ImRenderer->DrawCircle(center, config.aimbot_fov, config.fov_color.c());
		}

		std::vector<std::tuple<int, std::string, double>> enabled_features{};
		int hornyelephant = 0;

		for (int i = 0; i < features::features.size(); i++)
		{
			if (features::features[i]->enabled)
			{
				hornyelephant++;
				enabled_features.emplace_back<std::tuple<int, std::string, double>>({ hornyelephant, features::features[i]->name, features::features[i]->last_interact_time });
			}
			else if (!features::features[i]->enabled && features::features[i]->logged_value != nullptr)
			{
				if (!(*features::features[i]->logged_value))
					continue;
				hornyelephant++;
				enabled_features.emplace_back<std::tuple<int, std::string, double>>({ hornyelephant, features::features[i]->name, features::features[i]->last_interact_time });
			}
		}

		for (const auto& enabled_feature : enabled_features)
		{
			auto text_sz = ImRenderer->GetBoldFont()->CalcTextSizeA(22.f, FLT_MAX, 0.f, std::get<1>(enabled_feature).c_str());
			auto pos = ImVec2((c_overlay->m_pWidth - text_sz.x) - 10, 0 + (25 * std::get<0>(enabled_feature)));
			ImRenderer->DrawTextGui(std::get<1>(enabled_feature), pos, 22.f, ImColor(static_cast<int>(menu.AccentColor.r * 255.f), static_cast<int>(menu.AccentColor.g * 255.f), static_cast<int>(menu.AccentColor.b * 255), static_cast<int>((GetTickCount64() - std::get<2>(enabled_feature)) * 1.2f)), false, ImRenderer->GetBoldFont());
		}

#ifndef MENU_TESTING
		cheat::sync.lock();

		std::vector<aimbot_target_t> targets;

		for (const auto& player : cheat::players)
		{

			unity::vec3 base_pos = player.transform->position();

			if (base_pos.distance(unity::camera.position) > config.max_esp_distance)
				continue;

			unity::vec3 head_pos = base_pos;
			head_pos.y += player.movement->get_height();

			ImVec2 base_screen;
			ImVec2 head_screen;
			if (!unity::world_to_screen(base_pos, base_screen) || !unity::world_to_screen(head_pos, head_screen))
				continue;

			int box_x;
			int box_y;
			int box_w;
			int box_h;

			{
				int height = head_screen.y - base_screen.y;
				int width = height / 4;

				float Entity_x = base_screen.x - width;
				float Entity_y = base_screen.y;
				float Entity_w = height / 2;
				box_x = Entity_x;
				box_y = Entity_y;
				box_w = Entity_w;
				box_h = height;
			}

			std::string top_text = "";
			if (config.enable_name_esp)
				top_text += player.name;
			if (config.enable_distance_esp)
				top_text += " (" + std::to_string(static_cast<int>(unity::camera.position.distance(head_pos))) + "M)";

			if (config.enable_box_esp)
				DrawBox(ImVec2(box_x, box_y), ImVec2(box_w, box_h), config.box_esp_color.c());
			if (config.enable_weapon_esp)
				ImRenderer->DrawTextGui(player.weapon_name, ImVec2(head_screen.x, base_screen.y + 2.f), 11.f, config.weapon_esp_color.c(), true, nullptr);
			ImRenderer->DrawTextGui(top_text, ImVec2(head_screen.x, head_screen.y - 14.f), 11.f, config.name_esp_color.c(), true, nullptr);

			auto calc_fov = [&](ImVec2 pos)
			{
				static auto screen_center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);
				return pos.distance(screen_center);
			};

			aimbot_target_t target;
			target.player = player;
			target.fov = calc_fov(head_screen);
			target.world_pos = head_pos;
			targets.push_back(target);

		}

		if (config.aimbot_bind.enabled)
		{

			if (targets.size() > 0)
			{

				auto compare_target = [&](aimbot_target_t t1, aimbot_target_t t2)
				{
					return t1.fov < t2.fov;
				};

				std::sort(targets.begin(), targets.end(), compare_target);

				aimbot_target_t target = targets.at(0);

				if (!(target.fov > config.aimbot_fov))
				{

					unity::vec3 base_pos = target.player.transform->position();
					unity::vec3 head_pos = base_pos;
					head_pos.y += target.player.movement->get_height()-0.2f;

					switch (config.aimbot_type)
					{
						case 0: // mem
						{
							auto ang = calculate_angle(unity::camera.position, head_pos);
							auto look = cheat::local_player.playerlook;

							if (ang.y <= 90.f && ang.y <= 270.f) ang.y += 90.f;
							else if (ang.y >= 270.f && ang.y <= 360.f) ang.y -= 270.f;

							look->yaw(ang.x);
							look->pitch(ang.y);
							break;
						}
						case 1: // mouse
						{
							ImVec2 screen;
							if (unity::world_to_screen(head_pos, screen))
							{

								MoveMouse(screen.x, screen.y, 1920, 1080);

							}
							break;
						}
						case 2: // silent
						{
							auto usable = cheat::local_player.equipment->usable();
							if (usable)
							{
								auto usable_gun = (usable_gun_t*)usable;
								auto bullets = usable_gun->bullets();
								if (bullets)
								{

									for (const auto& bullet : memory.read_vec<bullet_info_t*>(bullets->list(), bullets->size()))
									{
										auto calc_dir = head_pos - bullet->origin();
										bullet->direction(calc_dir.normalized());
									}

									static auto center = ImVec2(c_overlay->m_pWidth / 2, c_overlay->m_pHeight / 2);
									ImVec2 screen;
									unity::world_to_screen(head_pos, screen);
									ImRenderer->DrawLineEx(center, screen, ImColor(255, 255, 255, 255));

								}

							}
							break;
						}
					}

				}

			}

		}

		if (config.no_sway)
		{
			if (cheat::local_player.player)
			{
				cheat::local_player.player->animator()->scope_sway(unity::vec3{ 0.f, 0.f, 0.f });
			}
		}

		if (config.no_recoil || config.no_spread || config.instant_aim || config.sprint_in_shoot || config.no_bullet_drop)
		{
			auto gun = (item_gun_asset_t*)cheat::local_player.equipment->asset();
			auto ballistics = gun->ballistic_information();
			if (gun && ballistics.valid())	
			{
				if (config.no_recoil) gun->recoil(unity::vec4{ 0.f, 0.f, 0.f, 0.f });
				if (config.no_spread) gun->base_spread(0.f);
				if (config.instant_aim) gun->aim_duration(0.f);
				if (config.sprint_in_shoot) gun->can_aim_during_sprint(true);
				if (config.no_bullet_drop) gun->ballistic_drop(0.f);
				gun->range(9999.f);
			}
		}

		cheat::sync.unlock();
#endif

		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			g_pd3dDevice->Reset(&g_d3dpp);
			ImGui_ImplDX9_CreateDeviceObjects();
		}

	}

	return 1;

}

struct injection_data
{

	char username[255];
	char expiry[255];
	uintptr_t key;
	unsigned int size;
	char process_name[255];

};

#ifndef _DEBUG
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{

	if (fdwReason == DLL_PROCESS_ATTACH)
	{

		injection_data* data = reinterpret_cast<injection_data*>(lpvReserved);

		if (!lpvReserved)
			reinterpret_cast<int>(nullptr)();

		if (!data)
			reinterpret_cast<int>(nullptr)();

		if (data->size != 0x4500)
			reinterpret_cast<int>(nullptr)();

		if (strcmp(data->process_name, "ctfmon.exe"))
			reinterpret_cast<int>(nullptr)();

		config.username = data->username;

		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(main_thread), NULL, NULL, NULL);

	}

	return TRUE;

}
#else
int main()
{
	main_thread();
}
#endif