#pragma once


namespace Parasite
{
	template<typename T>
	struct TVector
	{
		static_assert(std::is_floating_point_v<T>, "Given templated type must be a floating point value.");

	public:
		union
		{
			struct
			{
				T X;
				T Y;
				T Z;
			};
		};

		static const TVector<T> ZeroVector;

		/** Returns the vector (0,0,0) */
		static inline TVector<T> Zero() { return TVector<T>(static_cast<T>(0)); }

		/** Returns the vector (1,1,1) */
		static inline TVector<T> One() { return TVector<T>(static_cast<T>(1)); }

		/** Returns string as "(X, Y, Z)" */
		inline const std::string ToString() const 
		{ 
			return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";
		}

		TVector() = default;
		
		/**
		 *	Constructor used to init all components individually
		 */
		inline TVector(T InValueX, T InValueY, T InValueZ);

		/**
		 *	Constructor used to init all components to a single value
		 */
		explicit inline TVector(T InValue);

		const bool operator==(const TVector<T>& InOther) const;
		const bool operator!=(const TVector<T>& InOther) const;
		TVector<T> operator*=(const TVector<T>& InOther);
		
		template<typename ScalarType >
		requires std::is_arithmetic_v<ScalarType>
		TVector<T> operator*=(const ScalarType& InScale)
		{
			X *= InScale;
			Y *= InScale;
			Z *= InScale;
			return *this;
		}
	};


	template<typename T>
	TVector<T>::TVector(T InValueX, T InValueY, T InValueZ)
		: X(InValueX), Y(InValueY), Z(InValueZ)
	{
	}

	template<typename T>
	TVector<T>::TVector(T InValue)
		: X(InValue), Y(InValue), Z(InValue)
	{
	}

	template<typename T>
	const bool TVector<T>::operator==(const TVector<T>& InOther) const
	{
		return X == InOther.X && Y == InOther.Y && Z == InOther.Z;
	}

	template<typename T>
	const bool TVector<T>::operator!=(const TVector<T>& InOther) const
	{
		return X != InOther.X || Y != InOther.Y || Z != InOther.Z;
	}

	template<typename T>
	TVector<T> TVector<T>::operator*=(const TVector<T>& InOther)
	{
		X *= InOther.X;
		Y *= InOther.Y;
		Z *= InOther.Z;
		return *this;
	}

	template<typename T>
	const TVector<T> TVector<T>::ZeroVector = TVector<T>(static_cast<T>(0));
};