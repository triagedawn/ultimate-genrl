#ifndef MISC_SAMP_HPP
#define MISC_SAMP_HPP

#include <cstdint>
#include <filesystem>

namespace misc {
	std::uintptr_t samp();
	bool		   samp_loaded();
	bool		   IsUsingR3();
	bool		   IsUsingR5();

	std::filesystem::path current_path();
} // namespace misc

#endif // MISC_SAMP_HPP
