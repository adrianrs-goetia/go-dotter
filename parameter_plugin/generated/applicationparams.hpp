#pragma once
#include <configHandler.h>

// CODE IS GENERATED FROM A CONFIG JSON -> file: project/application.json
// CHANGES WILL BE OVERWRITTEN
//
//
// EXAMPLE USE CASE:
//
//    param.myvalue();
//    param.nestedobject.mynestedvalue();
//
//    const MyParam::NestedObject& param = param.nestedobject;
//    param.mynestedvalue();


struct ApplicationParam {
struct Input{
auto overwriteMode() const { return GETPARAMGLOBAL_I("input", "overwriteMode"); }
auto mouselock() const { return GETPARAMGLOBAL_I("input", "mouselock"); }
}
//[]
input;

auto hardResetTreeOnRestart() const { return GETPARAMGLOBAL_I("hardResetTreeOnRestart"); }

}
 applicationparam;

