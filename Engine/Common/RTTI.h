#pragma once

#include "Common/Common.h"

// RunTime-Type-Information.
namespace Wanted
{
	class WANTED_API RTTI
	{
	public:
		virtual const size_t& GetType() const = 0;

		virtual bool Is(RTTI* const rtti) const
		{
			return false;
		}

		virtual bool Is(const size_t id) const
		{
			return false;
		}

		// 타입 질문 함수
		template<typename T>
		bool IsTypeOf()
		{
			return Is(T::TypeIdClass());
		}

		template<typename T>
		T* As()
		{
			if (Is(T::TypeIdClass()))
			{
				return (T*)this;
			}

			return nullptr;
		}

		template<typename T>
		const T* As() const
		{
			if (Is(T::TypeIdClass()))
			{
				return (T*)this;
			}

			return nullptr;
		}
	};
}

// RTTI를 선언할 클래스에 추가할 매크로.
// 아래 코드에서 Type, ParentType이 실제 타입으로 변환되어 복사/붙여넣기 됨.
// runTimeTypeId의 정보를 어떤 타입으로 저장할까 -> 문자열(실행시간 성능문제)
// -> 숫자(클래스 당 정보가 한 개여야한다.) : 인스턴스 기반으로 타입이 같아야한다.
// -> 1. 열거형 2. 전역에서 고유번호를 부여함 / 유일한 값을 시스템적으로 만드는게 너무 어렵다
// -> 시스템에게 고유값을 맡김 static 멤버변수 초기화시스템을 이용해서 전역변수처럼 사용해서
// -> 지역변수의 주소 값을 숫자처럼 사용해서 type들이 고유한 주소값을 Id처럼 가진다
#define RTTI_DECLARATIONS(Type, ParentType)												\
friend class RTTI;																		\
protected:																				\
	static const size_t TypeIdClass()													\
	{																					\
		static int runTimeTypeId = 0;													\
		return reinterpret_cast<size_t>(&runTimeTypeId);								\
	}																					\
public:																					\
	virtual const size_t& GetType() const override { return Type::TypeIdClass(); }		\
	using super = ParentType;															\
	virtual bool Is(const size_t id) const override										\
	{																					\
		if (id == TypeIdClass())														\
		{																				\
			return true;																\
		}																				\
		else																			\
		{																				\
			return ParentType::Is(id);													\
		}																				\
	}																					\
	virtual bool Is(RTTI* const rtti) const override									\
	{																					\
		return Is(rtti->GetType());														\
	}