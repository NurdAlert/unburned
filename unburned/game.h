#pragma once

#include "c_memory.h"
#include "includes.h"
#include "mono.h"

#define cast_this reinterpret_cast<uintptr_t>(this)
#define get_member(type, name, offset) type name() { return memory.read<type>(cast_this + offset); }
#define set_member(type, name, offset) void name( type val ) { memory.write<type>(cast_this + offset, val); }
#define member(type, name, offset) get_member(type, name, offset) set_member(type, name, offset)
#define offset(klass, varname, name) varname = klass->find_field(name)->offset()

namespace unity_classes
{
	inline mono_class_t* playermovement;
	inline mono_class_t* playerlife;
	inline mono_class_t* playerlook;
	inline mono_class_t* provider;
	inline mono_class_t* steamplayer;
	inline mono_class_t* player;
	inline mono_class_t* maincamera;
	inline mono_class_t* steamplayerid;
	inline mono_class_t* playerequipment;
	inline mono_class_t* itemasset;
	inline mono_class_t* itemmanager;
	inline mono_class_t* interactableitem;
	inline mono_class_t* playerclothing;
	inline mono_class_t* humanclothes;
	inline mono_class_t* itemregion;
	inline mono_class_t* itemdrop;
	inline mono_class_t* itemdata;

	void init()
	{
		playermovement = mono::find_class("Assembly-CSharp", "SDG.Unturned.PlayerMovement");
		playerlife = mono::find_class("Assembly-CSharp", "SDG.Unturned.PlayerLife");
		playerlook = mono::find_class("Assembly-CSharp", "SDG.Unturned.PlayerLook");
		provider = mono::find_class("Assembly-CSharp", "SDG.Unturned.Provider");
		steamplayer = mono::find_class("Assembly-CSharp", "SDG.Unturned.SteamPlayer");
		player = mono::find_class("Assembly-CSharp", "SDG.Unturned.Player");
		maincamera = mono::find_class("Assembly-CSharp", "SDG.Unturned.MainCamera");
		steamplayerid = mono::find_class("Assembly-CSharp", "SDG.Unturned.SteamPlayerID");
		playerequipment = mono::find_class("Assembly-CSharp", "SDG.Unturned.PlayerEquipment");
		itemasset = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemAsset");
		itemmanager = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemManager");
		interactableitem = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemManager");
		playerclothing = mono::find_class("Assembly-CSharp", "SDG.Unturned.PlayerClothing");
		humanclothes = mono::find_class("Assembly-CSharp", "SDG.Unturned.HumanClothes");
		itemregion = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemRegion");
		itemdrop = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemDrop");
		itemdata = mono::find_class("Assembly-CSharp", "SDG.Unturned.ItemData");
	}
}

namespace class_offsets
{
	inline int max_players;
	inline int clients;
	inline int player;
	inline int life;
	inline int health;
	inline int look;
	inline int movement;
	inline int stamina;
	inline int water;
	inline int food;
	inline int _player;
	inline int orbit_yaw;
	inline int orbit_pitch;
	inline int yaw;
	inline int pitch;
	inline int character_height;
	inline int character_yaw;
	inline int is_connected;
	inline int character_camera;
	inline int camera_instance;
	inline int clothing;
	inline int lastupdatepos;
	inline int player_id;
	inline int model;
	inline int character_name;
	inline int velocity;
	inline int jump;
	inline int item_name;
	inline int asset;
	inline int equipment;
	inline int manager;
	inline int clampeditems;
	inline int interactable_item_asset;
	inline int rarity;
	inline int type;
	inline int item;
	inline int characterclothes;
	inline int skull;
	inline int itemregions;
	inline int drops;
	inline int itemmodel;
	inline int height;

	void init()
	{
		offset(unity_classes::provider, max_players, "_maxPlayers");
		offset(unity_classes::provider, is_connected, "_isConnected");
		offset(unity_classes::provider, clients, "_clients");
		offset(unity_classes::steamplayer, player, "_player");
		offset(unity_classes::player, life, "_life");
		offset(unity_classes::player, look, "_look");
		offset(unity_classes::player, movement, "_movement");
		offset(unity_classes::playerlife, stamina, "_stamina");
		offset(unity_classes::playerlife, water, "_water");
		offset(unity_classes::playerlife, food, "_food");
		offset(unity_classes::player, _player, "_player");
		offset(unity_classes::playerlook, orbit_yaw, "_orbitYaw");
		offset(unity_classes::playerlook, orbit_pitch, "_orbitPitch");
		offset(unity_classes::playerlook, yaw, "_yaw");
		offset(unity_classes::playerlook, pitch, "_pitch");
		offset(unity_classes::playerlook, character_height, "characterHeight");
		offset(unity_classes::playerlook, character_yaw, "characterYaw");
		offset(unity_classes::playerlook, character_camera, "_characterCamera");
		offset(unity_classes::maincamera, camera_instance, "_instance");
		offset(unity_classes::playermovement, lastupdatepos, "lastUpdatePos");
		offset(unity_classes::steamplayer, player_id, "_playerID");
		offset(unity_classes::steamplayerid, character_name, "_characterName");
		offset(unity_classes::playerlife, health, "_health");
		offset(unity_classes::playermovement, velocity, "velocity");
		offset(unity_classes::playermovement, jump, "JUMP");
		offset(unity_classes::player, equipment, "_equipment");
		offset(unity_classes::playerequipment, asset, "_asset");
		offset(unity_classes::itemasset, item_name, "_itemName");
		offset(unity_classes::itemasset, rarity, "rarity");
		offset(unity_classes::itemasset, type, "type");
		offset(unity_classes::itemmanager, manager, "manager");
		offset(unity_classes::itemmanager, itemregions, "<regions>k__BackingField");
		offset(unity_classes::itemmanager, clampeditems, "clampedItems");
		offset(unity_classes::interactableitem, interactable_item_asset, "asset");
		offset(unity_classes::interactableitem, item, "_item");
		offset(unity_classes::player, clothing, "_clothing");
		offset(unity_classes::playerclothing, characterclothes, "<thirdClothes>k__BackingField");
		offset(unity_classes::humanclothes, skull, "skull");
		offset(unity_classes::steamplayer, model, "_model");
		offset(unity_classes::itemregion, drops, "_drops");
		offset(unity_classes::itemdrop, itemmodel, "_model");
		offset(unity_classes::playermovement, height, "height");
	}
}

namespace unity
{
	struct vec3
	{
		float x, y, z;
		std::string print()
		{
			std::stringstream ss;
			ss << "x: " << x << " y: " << y << " z: " << z;
			return ss.str();
		}
		float dot(const vec3& b) const
		{
			return (x * b.x + y * b.y + z * b.z);
		}
		float distance(const vec3& b)
		{
			float num = this->x - b.x;
			float num2 = this->y - b.y;
			float num3 = this->z - b.z;
			return (float)sqrt((double)(num * num + num2 * num2 + num3 * num3));
		}
		float length2d() const
		{
			return sqrt(x * x + y * y);
		}
		vec3 operator-(const vec3& v) const
		{
			return vec3(x - v.x, y - v.y, z - v.z);
		}
		vec3 operator*(float v) const
		{
			return vec3(x * v, y * v, z * v);
		}
		vec3 clamp()
		{
			if (x < -89.0f)
				x = -89.0f;

			if (x > 89.0f)
				x = 89.0f;

			while (y < -180.0f)
				y += 360.0f;

			while (y > 180.0f)
				y -= 360.0f;

			z = 0.0f;
			return *this;
		}
		float length() const
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}
	};

	struct vmatrix_t
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};

	struct camera_t
	{
		vec3 position;
		vmatrix_t matrix;
	}; inline camera_t camera;

	bool world_to_screen(const vec3& entity_position, ImVec2& screen_position)
	{

		vec3 transform{ camera.matrix._14, camera.matrix._24, camera.matrix._34 };
		vec3 right{ camera.matrix._11, camera.matrix._21, camera.matrix._31 };
		vec3 up{ camera.matrix._12, camera.matrix._22, camera.matrix._32 };

		float w = transform.dot(entity_position) + camera.matrix._44;

		ImVec2 pos{ right.dot(entity_position) + camera.matrix._41, up.dot(entity_position) + camera.matrix._42 };

		if (w < 0.098f)
			return false;

		static float screen_center_x = GetSystemMetrics(SM_CXSCREEN) / 2;
		static float screen_center_y = GetSystemMetrics(SM_CYSCREEN) / 2;

		screen_position = ImVec2(screen_center_x * (1 + pos.x / w), screen_center_y * (1 - pos.y / w));

		return true;
	}
	vec3 calcangle(const vec3& src, const vec3& dst) 
	{
		#define M_RADPI 57.295779513082f
		vec3 vecDiff = dst - src;
		return vec3(std::atan2(-vecDiff.z, vecDiff.length2d()) * M_RADPI, std::atan2(vecDiff.y, vecDiff.x) * M_RADPI, 0.0f);
	}
}

enum EItemRarity
{
	// Token: 0x04000692 RID: 1682
	COMMON,
	// Token: 0x04000693 RID: 1683
	UNCOMMON,
	// Token: 0x04000694 RID: 1684
	RARE,
	// Token: 0x04000695 RID: 1685
	EPIC,
	// Token: 0x04000696 RID: 1686
	LEGENDARY,
	// Token: 0x04000697 RID: 1687
	MYTHICAL
};

enum EPlayerHeight
{
	// Token: 0x04001C0B RID: 7179
	STAND,
	// Token: 0x04001C0C RID: 7180
	CROUCH,
	// Token: 0x04001C0D RID: 7181
	PRONE
};

enum EItemType
{
	// Token: 0x04000699 RID: 1689
	HAT,
	// Token: 0x0400069A RID: 1690
	PANTS,
	// Token: 0x0400069B RID: 1691
	SHIRT,
	// Token: 0x0400069C RID: 1692
	MASK,
	// Token: 0x0400069D RID: 1693
	BACKPACK,
	// Token: 0x0400069E RID: 1694
	VEST,
	// Token: 0x0400069F RID: 1695
	GLASSES,
	// Token: 0x040006A0 RID: 1696
	GUN,
	// Token: 0x040006A1 RID: 1697
	SIGHT,
	// Token: 0x040006A2 RID: 1698
	TACTICAL,
	// Token: 0x040006A3 RID: 1699
	GRIP,
	// Token: 0x040006A4 RID: 1700
	BARREL,
	// Token: 0x040006A5 RID: 1701
	MAGAZINE,
	// Token: 0x040006A6 RID: 1702
	FOOD,
	// Token: 0x040006A7 RID: 1703
	WATER,
	// Token: 0x040006A8 RID: 1704
	MEDICAL,
	// Token: 0x040006A9 RID: 1705
	MELEE,
	// Token: 0x040006AA RID: 1706
	FUEL,
	// Token: 0x040006AB RID: 1707
	TOOL,
	// Token: 0x040006AC RID: 1708
	BARRICADE,
	// Token: 0x040006AD RID: 1709
	STORAGE,
	// Token: 0x040006AE RID: 1710
	BEACON,
	// Token: 0x040006AF RID: 1711
	FARM,
	// Token: 0x040006B0 RID: 1712
	TRAP,
	// Token: 0x040006B1 RID: 1713
	STRUCTURE,
	// Token: 0x040006B2 RID: 1714
	SUPPLY,
	// Token: 0x040006B3 RID: 1715
	THROWABLE,
	// Token: 0x040006B4 RID: 1716
	GROWER,
	// Token: 0x040006B5 RID: 1717
	OPTIC,
	// Token: 0x040006B6 RID: 1718
	REFILL,
	// Token: 0x040006B7 RID: 1719
	FISHER,
	// Token: 0x040006B8 RID: 1720
	CLOUD,
	// Token: 0x040006B9 RID: 1721
	MAP,
	// Token: 0x040006BA RID: 1722
	KEY,
	// Token: 0x040006BB RID: 1723
	BOX,
	// Token: 0x040006BC RID: 1724
	ARREST_START,
	// Token: 0x040006BD RID: 1725
	ARREST_END,
	// Token: 0x040006BE RID: 1726
	TANK,
	// Token: 0x040006BF RID: 1727
	GENERATOR,
	// Token: 0x040006C0 RID: 1728
	DETONATOR,
	// Token: 0x040006C1 RID: 1729
	CHARGE,
	// Token: 0x040006C2 RID: 1730
	LIBRARY,
	// Token: 0x040006C3 RID: 1731
	FILTER,
	// Token: 0x040006C4 RID: 1732
	SENTRY,
	// Token: 0x040006C5 RID: 1733
	VEHICLE_REPAIR_TOOL,
	// Token: 0x040006C6 RID: 1734
	TIRE,
	// Token: 0x040006C7 RID: 1735
	COMPASS,
	// Token: 0x040006C8 RID: 1736
	OIL_PUMP
};

enum ELimb
{
	// Token: 0x04000C76 RID: 3190
	LEFT_FOOT,
	// Token: 0x04000C77 RID: 3191
	LEFT_LEG,
	// Token: 0x04000C78 RID: 3192
	RIGHT_FOOT,
	// Token: 0x04000C79 RID: 3193
	RIGHT_LEG,
	// Token: 0x04000C7A RID: 3194
	LEFT_HAND,
	// Token: 0x04000C7B RID: 3195
	LEFT_ARM,
	// Token: 0x04000C7C RID: 3196
	RIGHT_HAND,
	// Token: 0x04000C7D RID: 3197
	RIGHT_ARM,
	// Token: 0x04000C7E RID: 3198
	LEFT_BACK,
	// Token: 0x04000C7F RID: 3199
	RIGHT_BACK,
	// Token: 0x04000C80 RID: 3200
	LEFT_FRONT,
	// Token: 0x04000C81 RID: 3201
	RIGHT_FRONT,
	// Token: 0x04000C82 RID: 3202
	SPINE,
	// Token: 0x04000C83 RID: 3203
	SKULL
};

struct unity_list_t
{
	get_member(uint32_t, size, 0x18)
	get_member(uintptr_t, internal_list, 0x10)
	uintptr_t list()
	{
		return internal_list() + 0x20;
	}
};

struct unity_transform_t
{
	unity::vec3 __fastcall position()
	{

		if (!this)
			return unity::vec3{};

		struct transform_access_read_only_t
		{
			uint64_t transform_data{};
		};

		struct transform_data_t
		{
			uint64_t transform_array{};
			uint64_t transform_indices{};
		};

		struct vec4
		{
			float x, y, z, w;
		};

		struct matrix34_t
		{
			vec4 vec0{};
			vec4 vec1{};
			vec4 vec2{};
		};

		__m128 result{};

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		uintptr_t pTransform = memory.read<uintptr_t>(cast_this + 0x10);

		transform_access_read_only_t transform_access_read_only = memory.read<transform_access_read_only_t>(pTransform + 0x38);
		unsigned int index = memory.read<unsigned int>(pTransform + 0x40);
		transform_data_t transform_data = memory.read<transform_data_t>(transform_access_read_only.transform_data + 0x18);

		if (transform_data.transform_array && transform_data.transform_indices)
		{
			result = memory.read<__m128>(transform_data.transform_array + 0x30 * index);
			int transform_index = memory.read<int>(transform_data.transform_indices + 0x4 * index);
			int safe = 0;
			while (transform_index >= 0 && safe++ < 1)
			{
				matrix34_t matrix = memory.read<matrix34_t>(transform_data.transform_array + 0x30 * transform_index);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

				result = _mm_add_ps(_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix.vec0));

				transform_index = memory.read<int>(transform_data.transform_indices + 0x4 * transform_index);
			}
		}
		return unity::vec3{ result.m128_f32[0], result.m128_f32[1], result.m128_f32[2] };
	}
};

struct player_movement_t
{
	static auto get_instance() -> player_movement_t*
	{
		static auto instance = (player_movement_t*)unity_classes::playermovement->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	member(unity::vec3, last_update_pos, class_offsets::lastupdatepos)
	member(unity::vec3, velocity, class_offsets::velocity)
	member(float, jump, class_offsets::jump)
	get_member(EPlayerHeight, height, class_offsets::height)
	float get_height()
	{
		switch (height())
		{
		case EPlayerHeight::STAND:
			return 2.f;
			break;
		case EPlayerHeight::CROUCH:
			return 1.2f;
			break;
		case EPlayerHeight::PRONE:
			return 0.8f;
			break;
		default:
			return 2.f;
			break;
		}
	}
};

struct player_look_t
{
	static auto get_instance() -> player_look_t*
	{
		static auto instance = (player_look_t*)unity_classes::playerlook->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	member(float, orbit_yaw, class_offsets::orbit_yaw)
	member(float, orbit_pitch, class_offsets::orbit_pitch)
	member(float, yaw, class_offsets::yaw)
	member(float, pitch, class_offsets::pitch)
	member(float, character_height, class_offsets::character_height)
	member(float, character_yaw, class_offsets::character_yaw)
	get_member(uintptr_t, character_camera, class_offsets::character_camera)
};

struct item_asset_t
{
	std::string item_name()
	{
		if (!this)
			return "None";
		uintptr_t managed_string = memory.read<uintptr_t>(cast_this + class_offsets::item_name);
		auto wide = memory.read_wstr(managed_string + 0x14);
		return std::string(wide.begin(), wide.end());
	}
	std::wstring item_name_wide()
	{
		if (!this)
			return L"None";
		uintptr_t managed_string = memory.read<uintptr_t>(cast_this + class_offsets::item_name);
		auto wide = memory.read_wstr(managed_string + 0x14);
		return wide;
	}
	unity::vec3 position()
	{
		return memory.read_chain<unity::vec3>(item(), { 0x30, 0x8, 0x38, 0x90 });
	}
	get_member(EItemRarity, rarity, class_offsets::rarity)
	get_member(EItemType, type, class_offsets::type)
	get_member(uintptr_t, item, class_offsets::item)
};

struct player_equipment_t
{
	get_member(item_asset_t*, asset, class_offsets::asset)
};

struct player_life_t
{
	get_member(byte, food, class_offsets::food);
	get_member(byte, water, class_offsets::water);
	get_member(byte, stamina, class_offsets::stamina);
	get_member(byte, health, class_offsets::health)
};

struct interactable_item_t
{
	get_member(item_asset_t*, asset, class_offsets::interactable_item_asset)
};

struct item_drop_t
{
	get_member(unity_transform_t*, model, class_offsets::itemmodel)
};

struct item_region_t
{
	get_member(unity_list_t*, drops, class_offsets::drops)
};

struct item_manager_t
{
	static auto get_instance() -> item_manager_t*
	{
		static auto instance = (item_manager_t*)unity_classes::itemmanager->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	get_member(item_manager_t*, manager, class_offsets::manager)
	get_member(unity_list_t*, clamped_items, class_offsets::clampeditems)
	get_member(unity_list_t*, item_regions, class_offsets::itemregions)
};

struct item_weapon_asset : item_asset_t
{

};

struct item_gun_asset : item_weapon_asset
{

};

struct human_clothes_t
{
	get_member(unity_transform_t*, skull, class_offsets::skull)
};

struct player_clothing_t
{
	get_member(human_clothes_t*, human_clothes, class_offsets::characterclothes)
};

struct player_t
{
	static auto get_instance() -> player_t*
	{
		static auto instance = (player_t*)unity_classes::player->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	static auto get_local()
	{
		return memory.read<player_t*>((uintptr_t)get_instance() + class_offsets::_player);
	}
	get_member(player_movement_t*, movement, class_offsets::movement)
	get_member(player_life_t*, life, class_offsets::life)
	get_member(player_look_t*, look, class_offsets::look)
	get_member(player_equipment_t*, equipment, class_offsets::equipment)
	get_member(player_clothing_t*, clothing, class_offsets::clothing)
};

struct steam_player_id_t
{
	std::string character_name()
	{
		uintptr_t managed_string = memory.read<uintptr_t>(cast_this + class_offsets::character_name);
		auto wide = memory.read_wstr(managed_string + 0x14);
		return std::string(wide.begin(), wide.end());
	}
};

struct steam_player_t
{
	get_member(player_t*, player, class_offsets::player)
	get_member(steam_player_id_t*, player_id, class_offsets::player_id)
	get_member(unity_transform_t*, model, class_offsets::model)
};

struct provider_t
{
	static auto get_instance() -> provider_t*
	{
		static auto instance = (provider_t*)unity_classes::provider->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	get_member(unity_list_t*, clients, class_offsets::clients)
	get_member(byte, max_clients, class_offsets::max_players)
	get_member(bool, is_connected, class_offsets::is_connected)
};

struct main_camera_t
{
	static auto get_instance() -> main_camera_t*
	{
		static auto instance = (main_camera_t*)unity_classes::maincamera->get_vtable(mono::get_root_domain())->get_static_field_data();
		return instance;
	}
	get_member(uintptr_t, instance, class_offsets::camera_instance)
};

struct cached_player_t
{
	steam_player_t* steamplayer;
	player_t* player;
	player_life_t* playerlife;
	player_look_t* playerlook;
	human_clothes_t* clothes;
	player_movement_t* movement;
	std::string name;
	std::string weapon_name;
};

struct aimbot_target_t
{
	float fov;
	cached_player_t player;
	ImVec2 pos;
	unity::vec3 world_pos;
};

namespace cheat
{
	std::vector<cached_player_t> players{};
	cached_player_t local_player;
	std::mutex sync{};
}