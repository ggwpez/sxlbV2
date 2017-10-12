#pragma once

#define BIT(x) (uint64_t(1) << x)

#define B(x) ( \
	(0##x >> 0 & 0001) | \
	(0##x >> 2 & 0002) | \
	(0##x >> 4 & 0004) | \
	(0##x >> 6 & 0010) | \
	(0##x >> 8 & 0020) | \
	(0##x >> 10 & 0040) | \
	(0##x >> 12 & 0100) | \
	(0##x >> 14 & 0200))

// TODO when not using 4.8, I can do this properly
template<typename T, typename S>
constexpr inline auto ROUND_UP(T const& v, S const& m)
	-> decltype(v -v %m)
{
	return ((v %m) == 0) ?
		v
	:
		v +(m -(v %m));
}

template<typename T, typename S>
constexpr inline auto ROUND_DW(T const& v, S const& m)
	-> decltype(v -v %m)
{
	return ((v %m) == 0) ?
		v
	:
		v -(v %m);
}
