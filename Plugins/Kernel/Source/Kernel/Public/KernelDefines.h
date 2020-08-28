#pragma once

#define BIT_ADD(Target, Flag) (Target) |= (Flag)
#define BIT_REMOVE(Target, Flag) (Target) &= ~(Flag)
#define BIT_CHECK(Target, Flag) (((Target) & (Flag)) != 0)


#define HIDE_CTOR_DTOR(Class) \
private: \
	Class(); \
	~Class(); 

#define DISABLE_COPY_CTOR_AND_ASSIGNMENT(Class) \
private: \
	Class(const Class&) = delete; \
	Class(Class&&) = delete; \
	Class& operator= (const Class&) = delete; \
	Class& operator= (Class&&) = delete;

#define DECLARE_SINGLETON(SingletonClass) \
public: \
    static SingletonClass& GetMe() { \
        static SingletonClass Me; \
        return Me; \
    } \
    HIDE_CTOR_DTOR(SingletonClass) \
    DISABLE_COPY_CTOR_AND_ASSIGNMENT(SingletonClass)

#define DECLARE_SINGLETON_WITH_EXPLICIT_CTOR_AND_DTOR(SingletonClass) \
public: \
	static SingletonClass& GetMe() { \
		checkSlow(S_Me != nullptr); \
		return *S_Me; \
	} \
	static void Initialize() { S_Me = new SingletonClass; } \
	static void Uninitialize() { checkSlow(S_Me != nullptr); delete S_Me; } \
private: \
	static SingletonClass *S_Me;

#define DEFINE_SINGLETON_WITH_EXPLICIT_CTOR_AND_DTOR(SingletonClass) \
	SingletonClass* SingletonClass::S_Me = nullptr;