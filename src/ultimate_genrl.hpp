#ifndef ULTIMATE_GENRL_HPP
#define ULTIMATE_GENRL_HPP

#include <cstdint>
#include <BitStream.h>
#include <lemon/hook.hpp>
#include "config.hpp"
#include "misc/vec3.hpp"

class UltimateGenrl {
public:
	UltimateGenrl(Config &config);
	~UltimateGenrl();

	[[nodiscard]] inline const std::filesystem::path &sound_dir_path() const {
		return sound_dir_path_;
	}

	void toggle_no_sound(bool status);
	void toggle_nopostfx(bool status);
	void play_sound(std::uint32_t weapon_type, const misc::vec3 *pos = nullptr);

private:
	Config				 &config_;
	std::filesystem::path sound_dir_path_;

	lemon::hook<>					  update_sfx_vol_hook_{ 0x573560 };
	lemon::hook<void *, misc::vec3 *> fire_hook_{ 0x742300 };
	lemon::hook<int *, BitStream *>	  send_rpc_hook_R3{ 0x33EE0, "samp.dll" };
	lemon::hook<int *, BitStream *>	  send_rpc_hook_R5{ 0x34620, "samp.dll" };

	void on_fire(lemon::hook_cpu &cpu, void *&owner, misc::vec3 *&src);
	void on_send_rpc(int *id, BitStream *bit_stream);
};

#endif // ULTIMATE_GENRL_HPP
