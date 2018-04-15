#pragma once

#include <Core/Types.h>
#include <Core/Platform/Platform.h>

#include <cstdlib>
#include <cmath>

namespace Columbus
{

	struct Math
	{
		static inline int32  TruncToInt(const float Value)     noexcept { return (int32)Value; }
		static inline float  TruncToFloat(const double Value)  noexcept { return (float)Value; }
		static inline int32  FloorToInt(const float Value)     noexcept { return (int32)floorf(Value); }
		static inline float  FloorToFloat(const float Value)   noexcept { return floorf(Value); }
		static inline double FloorToDouble(const double Value) noexcept { return floor(Value); }
		static inline int32  RoundToInt(const float Value)     noexcept { return FloorToInt(Value + 0.5f); }
		static inline float  RoundToFloat(const float Value)   noexcept { return FloorToFloat(Value + 0.5f); }
		static inline double RoundToDouble(const double Value) noexcept { return FloorToDouble(Value + 0.5); }
		static inline int32  CeilToInt(const float Value)      noexcept { return TruncToInt(ceilf(Value)); }
		static inline float  CeilToFloat(const float Value)    noexcept { return ceilf(Value); }
		static inline double CeilToDouble(const double Value)  noexcept { return ceil(Value); }

		/** e ^ Value */
		static inline float Exp(const float Value) noexcept { return expf(Value); }
		/** 2 ^ value */
		static inline float Exp2(const float Value) noexcept { return powf(2.0f, Value); }

		static inline float Sin(const float Value)  noexcept { return sinf(Value); }
		static inline float Cos(const float Value)  noexcept { return cosf(Value); }
		static inline float Tan(const float Value)  noexcept { return tanf(Value); }
		static inline float Sqrt(const float Value) noexcept { return sqrtf(Value); }
		static inline float Sqr(const float Value)  noexcept { return Value * Value; }
		static inline float Pow(const float A, const float B) noexcept { return powf(A, B); };

		/*
		* Find sinus and cosine of Value
		* @param const float Value: Value for finding sine and cosine
		* @param float& S: Sine value reference
		* @param float& C: Cosine value reference
		*/
		static inline void SinCos(const float Value, float& S, float& C) noexcept
		{
			S = Sin(Value);
			C = Cos(Value);
		}
		/*
		* Return true if Value is power of 2
		*/
		static inline bool IsPowerOf2(const int32 Value) noexcept
		{
			return (Value & (Value - 1)) == 0;
		}
		/*
		* Finding upper power of 2
		*/
		static inline int64 UpperPowerOf2(int64 Value) noexcept
		{
			Value--;
			Value |= Value >> 1;
			Value |= Value >> 2;
			Value |= Value >> 4;
			Value |= Value >> 8;
			Value |= Value >> 16;
			Value++;

			return Value;
		}
		/*
		* Return true if Value is Nan (not a number)
		*/
		static constexpr inline bool IsNan(float Value) noexcept
		{
			return ((*(uint32*)&Value) & 0x7FFFFFFF) > 0x7F800000;
		}
		/*
		* Return true if Value if finite (not NaN and not infinity)
		*/
		static constexpr inline bool IsFinite(float Value) noexcept
		{
			return ((*(uint32*)&Value) & 0x7F800000) != 0x7F800000;
		}
		/*
		* Return absolute value in a generic way
		*/
		template <class T>
		static constexpr inline T Abs(const T Value) noexcept
		{
			return (Value >= (T)0) ? Value : -Value;
		}
		/*
		* Returns 1, 0, or -1 depending on relation of T to 0
		*/
		template <class T>
		static constexpr inline T Sign(const T A) noexcept
		{
			return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
		}
		/*
		* Return lower value in a generic way
		*/
		template <class T>
		static constexpr inline T Min(const T A, const T B) noexcept
		{
			return A < B ? A : B;
		}
		/*
		* Return heigher value in a generic way
		*/
		template <class T>
		static constexpr inline T Max(const T A, const T B) noexcept
		{
			return A > B ? A : B;
		}
		/*
		* Constrain a value to lie between two further values
		*/
		template <class T>
		static constexpr inline T Clamp(const T A, const T MinValue, const T MaxValue) noexcept
		{
			return Min(Max(A, MinValue), MaxValue);
		}
		/*
		* Converts Degrees to radians
		*/
		template <class T>
		static constexpr inline T Radians(const T Degrees) noexcept
		{
			//return Degrees * 3.141592653f / 180.0f;
			//0.017453293 = 3.141592653 / 180.0
			return Degrees * 0.017453293f;
		}

		/*
		* Converts Radians to degrees
		*/
		template <class T>
		static constexpr inline T Degrees(const T Radians) noexcept
		{
			//return Radians * 180.0f / 3.141592653f;
			//57.295779524 = 180.0 / 3.141592653
			return Radians * 57.295779524f;
		}
	};

}



