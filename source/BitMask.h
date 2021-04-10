#ifndef BitMask_h__
#define BitMask_h__

#include "type_traits"

template<typename Enum>
struct EnableBitMaskOperators
{
	static const bool enable = false;
};

#define ENABLE_BITMASK_OPERATORS(x)  \
template<>                           \
struct EnableBitMaskOperators<x>     \
{                                    \
    static const bool enable = true; \
};


template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (
		static_cast<underlying>(lhs) ^
		static_cast<underlying>(rhs)
		);
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ~(Enum rhs)
{

	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (
		~static_cast<underlying>(rhs)
		);
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type&
operator &=(Enum& lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	lhs = static_cast<Enum> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);
	return lhs;
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type&
operator ^=(Enum& lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	lhs = static_cast<Enum> (
		static_cast<underlying>(lhs) ^
		static_cast<underlying>(rhs)
		);
	return lhs;
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum&>::type
operator |=(Enum& lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	lhs = static_cast<Enum> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);
	return lhs;
}

template<typename Enum>
bool isBitActive(Enum& bitmask, Enum testBit)
{
	return (bitmask & testBit) == testBit;
}

#endif // BitMask_h__