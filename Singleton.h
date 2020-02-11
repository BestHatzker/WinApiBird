#pragma once

#define SINGLT_DELETE_CONSTRUCTORS(T) \
T(T const&) = delete;\
T& operator=(T const&) = delete;

//////////////////////////////////////////////////////////////
#define SINGLT_DEF_FUNCS(T) \
static T* GetInstance() { if (!m_pSingleton) { m_pSingleton = new T(); } return m_pSingleton; } \
static void DelInstance() { if (m_pSingleton) { delete m_pSingleton; m_pSingleton = nullptr; } }

//////////////////////////////////////////////////////////////
#define SINGL_MEMBERS(T) \
protected:\
static T* m_pSingleton;\

//////////////////////////////////////////////////////////////
#define SINGLTONE_BASE_DEFAULT(T)\
public:\
SINGLT_DEF_FUNCS(T)\
SINGL_MEMBERS(T)\
private:\
SINGLT_DELETE_CONSTRUCTORS(T)
//////////////////////////////////////////////////////////////
