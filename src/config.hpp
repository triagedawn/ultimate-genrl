#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Windows.h>
#include <filesystem>
#include <nlohmann/json.hpp>

class Config {
public:
	struct Data {

		int menu_key		= VK_F3;

		bool enabled  = true;
		bool no_sound = true;

		bool weapon		  = true;
		bool enemy_weapon = true;

		bool hit  = true;
		bool pain = true;

		int colt45_sound	= 1;
		int silenced_sound	= 1;
		int deagle_sound	= 1;
		int shotgun_sound	= 1;
		int sawnoff_sound	= 1;
		int shotgspa_sound	= 1;
		int uzi_sound		= 1;
		int mp5_sound		= 1;
		int ak47_sound		= 1;
		int m4_sound		= 1;
		int tec9_sound		= 1;
		int cuntgun_sound	= 1;
		int sniper_sound	= 1;

		int hit_sound		= 1;
		int pain_sound		= 1;

		float weapon_vol	= 50.0f;
		float hit_vol		= 50.0f;
		float pain_vol		= 50.0f;

		bool bNoPostFx		= false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, menu_key, enabled, no_sound, weapon, enemy_weapon, hit, pain, 
			colt45_sound,
			silenced_sound,
			deagle_sound,
			shotgun_sound,
			sawnoff_sound,
			shotgspa_sound,
			uzi_sound,
			mp5_sound,
			ak47_sound,
			m4_sound,
			tec9_sound,
			cuntgun_sound,
			sniper_sound,
			hit_sound, pain_sound, weapon_vol, hit_vol, pain_vol, bNoPostFx);
	};

	Config(std::filesystem::path path);
	~Config();

	bool load();
	void save() const;

	struct Data		  *operator->();
	const struct Data *operator->() const;

private:
	std::filesystem::path path_;
	struct Data			  data_;
};

#endif // CONFIG_HPP
