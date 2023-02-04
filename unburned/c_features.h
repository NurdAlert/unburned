#pragma once

#include "game.h"

namespace features
{

	inline std::vector<struct c_feature*> features{};

	class c_feature
	{
	public:
		bool enabled = false;
		std::string name;
		double last_interact_time = 0.f;
		bool* logged_value = nullptr;

		c_feature()
		{

		}
		
		c_feature(std::string feature_name)
		{
			name = feature_name;
			enabled = true;
			last_interact_time = GetTickCount64();
			features.push_back(this);
		}

		void on_enable()
		{
			last_interact_time = GetTickCount64();
			enabled = true;
		}

		void on_disable()
		{
			last_interact_time = GetTickCount64();
			enabled = false;
		}

		void on_interact()
		{
			last_interact_time = GetTickCount64();
		}

	};

	class c_logged_feature : public c_feature
	{
	public:

		c_logged_feature(std::string _name, bool* value)
		{
			logged_value = value;
			name = _name;
			enabled = false;
			last_interact_time = GetTickCount64();
			features.push_back(this);
		}

	};

	class c_no_recoil : public c_feature
	{
	private:
		std::vector<std::tuple<std::string, unity::vec4>> saved_recoil{};
	public:

		using c_feature::c_feature;

		void on_enable()
		{

			enabled = true;
			last_interact_time = GetTickCount64();

			auto gun = (item_gun_asset_t*)cheat::local_player.equipment->asset();
			if (!gun)
				return;

			saved_recoil.emplace_back<std::tuple<std::string, unity::vec4>>({ gun->item_name(), gun->recoil() });
			gun->recoil(unity::vec4{ 0.f, 0.f, 0.f, 0.f });

		}

		void on_disable()
		{

			enabled = false;
			last_interact_time = GetTickCount64();

			auto gun = (item_gun_asset_t*)cheat::local_player.equipment->asset();
			if (!gun)
				return;

			for (const auto& saved_data : saved_recoil)
				if (std::get<0>(saved_data) == gun->item_name())
					gun->recoil(std::get<1>(saved_data));

		}

	}; inline c_no_recoil no_recoil("no recoil");

	class c_no_spread : public c_feature
	{
	private:
		std::vector<std::tuple<std::string, float>> saved_spread{};
	public:

		using c_feature::c_feature;

		void on_enable()
		{

			enabled = true;
			last_interact_time = GetTickCount64();

			auto gun = (item_gun_asset_t*)cheat::local_player.equipment->asset();
			if (!gun)
				return;

			saved_spread.emplace_back<std::tuple<std::string, float>>({ gun->item_name(), gun->base_spread() });
			gun->base_spread(0.f);

		}

		void on_disable()
		{

			enabled = false;
			last_interact_time = GetTickCount64();

			auto gun = (item_gun_asset_t*)cheat::local_player.equipment->asset();
			if (!gun)
				return;

			for (const auto& saved_data : saved_spread)
				if (std::get<0>(saved_data) == gun->item_name())
					gun->base_spread(std::get<1>(saved_data));

		}

	}; inline c_no_spread no_spread("no spread");

}