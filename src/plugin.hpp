#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <bass.h>
#include <d3d9.h>
#include <lemon/hook.hpp>
#include "config.hpp"
#include "ultimate_genrl.hpp"
#include "misc/misc.hpp"
#include "ui/menu.hpp"

class Plugin {
public:
	Plugin();
	~Plugin();

private:
	Config		  config_{ misc::current_path() / "ultimate-genrl.json" };
	UltimateGenrl ugenrl_{ config_ };
	ui::Menu	  menu_{ config_, ugenrl_ };

	//std::uint32_t bass_reset_orig_[31] = { 0 };
};

#endif // PLUGIN_HPP
