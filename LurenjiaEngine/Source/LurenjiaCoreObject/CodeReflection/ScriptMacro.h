#pragma once

class CCoreMinimalObject;
struct FFrame;

#define FUNCTION_DEFINITION(Func) static void Func(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
#define FUNCTION_IMPLEMENTATION(Func) void Func(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)