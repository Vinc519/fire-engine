#pragma once

#include "Defines.h"

struct MutexHandle;
struct CondVarHandle;

/// @brief 
/// @param handle 
/// @return 
bool CondVar_Create(CondVarHandle* handle);

/// @brief 
/// @param handle 
void CondVar_Destroy(CondVarHandle* handle);

/// @brief 
/// @param handle 
/// @param mutex 
void CondVar_Wait(CondVarHandle* handle, MutexHandle* mutex);

/// @brief 
/// @param handle 
void CondVar_Signal(CondVarHandle* handle);

/// @brief 
/// @param handle 
void CondVar_Broadcast(CondVarHandle* handle);