#pragma once
#include <configHandler.h>

// CODE IS GENERATED FROM A CONFIG JSON
// CHANGES WILL BE OVERWRITTEN

struct Param {
struct {
auto overwriteMode() const { return GETPARAMGLOBAL_I("input", "overwriteMode"); }
auto mouselock() const { return GETPARAMGLOBAL_I("input", "mouselock"); }
}
//[]
input;

auto hardResetTreeOnRestart() const { return GETPARAMGLOBAL_I("hardResetTreeOnRestart"); }

}
 param;

