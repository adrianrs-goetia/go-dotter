#pragma once

#include <configHandler.h>

// CODE IS GENERATED FROM A CONFIG JSON -> file: project/application.json
// CHANGES WILL BE OVERWRITTEN
//
//
// EXAMPLE USE CASE:
//
//    ParamType::myvalue();
//    ParamType::nestedobject::mynestedvalue();
//
//    MyParam::NestedObject param;
//    param.mynestedvalue();


struct ApplicationParam {
struct Input{
inline static auto overwriteMode() { return GETPARAMGLOBAL_I("input", "overwriteMode"); }
inline static auto mouselock() { return GETPARAMGLOBAL_B("input", "mouselock"); }
}
//[]
input;

inline static auto hardResetTreeOnRestart() { return GETPARAMGLOBAL_B("hardResetTreeOnRestart"); }

};

