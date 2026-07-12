#include "Defines.h"

#define FE_DEFINE_ENUM_FLAGS(EnumType, UnderlyingType)                \
	inline EnumType operator|(EnumType a, EnumType b)                 \
	{                                                                 \
		return static_cast<EnumType>(static_cast<UnderlyingType>(a) | \
		                             static_cast<UnderlyingType>(b)); \
	}                                                                 \
	inline EnumType operator&(EnumType a, EnumType b)                 \
	{                                                                 \
		return static_cast<EnumType>(static_cast<UnderlyingType>(a) & \
		                             static_cast<UnderlyingType>(b)); \
	}                                                                 \
	inline bool operator!(EnumType a)                                 \
	{                                                                 \
		return static_cast<UnderlyingType>(a) == 0;                   \
	}