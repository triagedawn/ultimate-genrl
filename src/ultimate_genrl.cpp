#include "ultimate_genrl.hpp"
#include <bass.h>
#include "misc/misc.hpp"
#include <imgui_internal.h>

#define VEC_TO_BASSVEC(vec) BASS_3DVECTOR(vec->x, vec->y, vec->z)


std::map<std::uint32_t, std::string> g_weapons_names {

	std::make_pair(22u, "Colt45"),
	std::make_pair(23u, "Silenced"),
	std::make_pair(24u, "Deagle"),
	std::make_pair(25u, "Shotgun"),
	std::make_pair(26u, "Sawnoff"),
	std::make_pair(27u, "Spas"),
	std::make_pair(28u, "Uzi"),
	std::make_pair(29u, "Mp5"),
	std::make_pair(30u, "Ak47"),
	std::make_pair(31u, "M4"),
	std::make_pair(32u, "Tec9"),
	std::make_pair(33u, "Cuntgun"),
	std::make_pair(34u, "Sniper"),
	std::make_pair(60u, "Hit"),
	std::make_pair(61u, "Pain")
};

UltimateGenrl::UltimateGenrl(Config &config) :
	config_(config), sound_dir_path_(misc::current_path() / "ultimate-genrl") {
	// Set 'nosound' according to config
	toggle_no_sound(config->no_sound);
	toggle_nopostfx(config->bNoPostFx);

	// Reload BASS with 3D
	BASS_Free();
	BASS_Init(-1, 44100, BASS_DEVICE_3D | BASS_DEVICE_FREQ, nullptr, nullptr);

	// Set BASS volume as SFX
	auto sfx_vol = *reinterpret_cast<std::uint8_t *>(0xBA6797);
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, sfx_vol * 7000u);
	// Set hook on update SFX volume
	update_sfx_vol_hook_.on_before += [&](lemon::hook_cpu &cpu) {
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, cpu.AL * 7000u);
	};
	update_sfx_vol_hook_.install();

	// Setup bullet impact hook
	fire_hook_.on_before += std::make_tuple(this, &UltimateGenrl::on_fire);
	fire_hook_.install(4);

	// Setup received hook
	if (misc::IsUsingR3()) {
		send_rpc_hook_R3.on_before += std::make_tuple(this, &UltimateGenrl::on_send_rpc);
		send_rpc_hook_R3.install(4);
	} else {
		send_rpc_hook_R5.on_before += std::make_tuple(this, &UltimateGenrl::on_send_rpc);
		send_rpc_hook_R5.install(4);
	}
}

UltimateGenrl::~UltimateGenrl() {
	toggle_no_sound(false);
	toggle_nopostfx(false);
}

void UltimateGenrl::toggle_no_sound(bool status) {
	// Disable sounds (ret on CAEAudioHardware::PlaySound())

	static std::uint8_t orig_mem[3] = { 0 };
	if (status) {
		lemon::mem::safe_copy(orig_mem, reinterpret_cast<void *>(0x4D86B0), 3);
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x4D86B0), "\xC2\x1C\x00", 3);
	} else if (orig_mem[0] != 0) {
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x4D86B0), orig_mem, 3);
		std::memset(orig_mem, 0, 3);
	}
}

void UltimateGenrl::toggle_nopostfx(bool status) {

	//static std::uint8_t orig_mem[1] = { 0 };
	if (status) {

		//lemon::mem::safe_copy(orig_mem, reinterpret_cast<void *>(0x53E227), 1);
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x53E227), "\xC3", 1);
	} else {

		//lemon::mem::safe_copy(reinterpret_cast<void *>(0x53E227), orig_mem, 1);
		lemon::mem::safe_copy(reinterpret_cast<void *>(0x53E227), "\xE9", 1);
		//std::memset(orig_mem, 0, 1);
	}
}

void UltimateGenrl::play_sound(std::uint32_t weapon_type, const misc::vec3 *pos) {
	if (!config_->enabled) return;
	if (!g_weapons_names.contains(weapon_type)) return;

	// Choose right sound path & volume
	auto sound_path = sound_dir_path_.string() + "/" + g_weapons_names.at(weapon_type);

	auto sound_vol = config_->weapon_vol;

	switch (weapon_type) 
	{
		case 22u: {
			sound_path += "." + std::to_string(config_->colt45_sound);
			break;
		}
		case 23u: {
			sound_path += "." + std::to_string(config_->silenced_sound);
			break;
		}
		case 24u: {
			sound_path += "." + std::to_string(config_->deagle_sound);
			break;
		}
		case 25u: {
			sound_path += "." + std::to_string(config_->shotgun_sound);
			break;
		}
		case 26u: {
			sound_path += "." + std::to_string(config_->sawnoff_sound);
			break;
		}
		case 27u: {
			sound_path += "." + std::to_string(config_->shotgspa_sound);
			break;
		}
		case 28u: {
			sound_path += "." + std::to_string(config_->uzi_sound);
			break;
		}
		case 29u: {
			sound_path += "." + std::to_string(config_->mp5_sound);
			break;
		}
		case 30u: {
			sound_path += "." + std::to_string(config_->ak47_sound);
			break;
		}
		case 31u: {
			sound_path += "." + std::to_string(config_->m4_sound);
			break;
		}
		case 32u: {
			sound_path += "." + std::to_string(config_->tec9_sound);
			break;
		}
		case 33u: {
			sound_path += "." + std::to_string(config_->cuntgun_sound);
			break;
		}
		case 34u: {
			sound_path += "." + std::to_string(config_->sniper_sound);
			break;
		}

		case 60u: {
			sound_path += "." + std::to_string(config_->hit_sound);
			sound_vol = config_->hit_vol;
			break;
		}
		case 61u: {
			sound_path += "." + std::to_string(config_->pain_sound);
			sound_vol = config_->pain_vol;
			break;
		}

		default:
			break;
	}

	// Check if the file exists with .wav or .mp3 extension
	std::string final_sound_path;
	if (std::filesystem::exists(sound_path + ".wav"))
	{
		sound_path += ".wav";
	}
	else if (std::filesystem::exists(sound_path + ".mp3"))
	{
		sound_path += ".mp3";
	}
	else
	{
		// Handle the case where neither file exists (e.g., use a default sound or log an error)
		// For this example, we'll default to ".wav"
		sound_path += ".wav"; // This may still fail if neither file exists
	}

	//sound_path += ".wav";
	sound_vol /= 100.0f;

	// Load sound & apply sound volume
	auto stream = BASS_StreamCreateFile(FALSE, sound_path.c_str(), 0, 0, pos ? (BASS_SAMPLE_3D | BASS_SAMPLE_MUTEMAX | BASS_STREAM_AUTOFREE) : 0);
	BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, sound_vol);

	if (pos != nullptr) {
		// Set listener position (if available); Matrix - 0x974
		auto cam_pos   = VEC_TO_BASSVEC(reinterpret_cast<misc::vec3 *>(0xB6F028 + 0x974 + 0x30));
		auto cam_front = VEC_TO_BASSVEC(reinterpret_cast<misc::vec3 *>(0xB6F028 + 0x974 + 0x20));
		auto cam_top   = VEC_TO_BASSVEC(reinterpret_cast<misc::vec3 *>(0xB6F028 + 0x974 + 0x10));
		BASS_Set3DPosition(&cam_pos, nullptr, &cam_front, &cam_top);

		// Set channel's 3D mode
		BASS_ChannelSet3DAttributes(stream, BASS_3DMODE_NORMAL, 0.0f, 125.0f, -1, -1, -1);

		// Set shot position & 3D attributes
		auto shot_pos = VEC_TO_BASSVEC(pos);
		BASS_ChannelSet3DPosition(stream, &shot_pos, nullptr, nullptr);

		// Apply 3D settings
		BASS_Apply3D();
	}

	// Play sound
	BASS_ChannelPlay(stream, true);
}

void UltimateGenrl::on_fire(lemon::hook_cpu &cpu, void *&owner, misc::vec3 *&src) {
	if (!config_->weapon && owner == *reinterpret_cast<void **>(0xB6F5F0)) return;
	if (!config_->enemy_weapon && owner != *reinterpret_cast<void **>(0xB6F5F0)) return;

	auto weapon_type = *reinterpret_cast<std::uint32_t *>(cpu.ECX);

	if (owner == *reinterpret_cast<void **>(0xB6F5F0))
		play_sound(weapon_type);
	else 
		play_sound(weapon_type, src);
}

void UltimateGenrl::on_send_rpc(int *id, BitStream *bit_stream) {

	if (!id || !bit_stream) return;

	/*
	if (*id == 16) {
		uint32_t soundid = 0;

		bit_stream->Read(soundid);

		if (soundid == 17802 && config_->hit) {
			play_sound(60u);
			bit_stream->SetWriteOffset(0);
			bit_stream->Write(0);
		}

		if (soundid == 1190 && config_->pain) {
			play_sound(61u);
			bit_stream->SetWriteOffset(0);
			bit_stream->Write(0);
		}
	}
	*/

	if (*id == 103) {
		uint8_t	 type	  = 0;
		uint32_t arg	  = 0;
		uint8_t	 response = 0;
		bit_stream->Read(type);
		bit_stream->Read(arg);
		bit_stream->Read(response);

		// Cheat Menu check bypass
		if (type == 69) {
			uint32_t someRaknetCheck = 214560;
			if (arg >= someRaknetCheck - 255 && arg <= someRaknetCheck) {
				bit_stream->SetWriteOffset(0);
				bit_stream->Write(type);
				bit_stream->Write(arg);
				bit_stream->Write(8);
			}
		}
	}

	if (*id == 115) {
		auto is_pain = false;
		bit_stream->Read(is_pain);

		if (!is_pain && config_->hit)
			return play_sound(60u);

		if (is_pain && config_->pain)
			return play_sound(61u);
	}
}
