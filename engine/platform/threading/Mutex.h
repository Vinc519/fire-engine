#pragma once

#include "Defines.h"

struct MutexHandle;

/// @brief 
/// @param handle 
/// @return 
bool Mutex_Create(MutexHandle* handle);

/// @brief 
/// @param handle 
void Mutex_Destroy(MutexHandle* handle);

/// @brief 
/// @param handle 
void Mutex_Lock(MutexHandle* handle);

/// @brief 
/// @param handle 
void Mutex_Unlock(MutexHandle* handle);

/// @brief 
/// @param handle
/// @return 
bool Mutex_TryLock(MutexHandle* handle);