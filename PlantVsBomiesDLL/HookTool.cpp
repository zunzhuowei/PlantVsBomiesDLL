#include "pch.h"
#include "HookTool.h"


bool HookTool::replaceInstructionFun(unsigned char*& orign, unsigned char* target, size_t size) {
	DWORD oldProtect;
	// 更改内存保护属性，使其可写
	if (!VirtualProtect(orign, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		return false;
	}
	// 修改指令的值
	for (size_t i = 0; i < size; i++)
	{
		orign[i] = target[i];
	}
	// 恢复内存保护属性
	if (!VirtualProtect(orign, size, oldProtect, &oldProtect)) {
		return false;
	}
	return true;
}

void HookTool::hook(DWORD& hookAddr, DWORD& hookAfterRetAddr, 
	unsigned char*& memoryForCustomAssembly, unsigned char customAssembly[],size_t codeSize) {
	size_t jumpInstructionLen = 5;
	//将指令拷贝到内存中
	memcpy(memoryForCustomAssembly, customAssembly, codeSize);
	long jmpReturnOffset = hookAfterRetAddr - ((long)memoryForCustomAssembly + codeSize);
	unsigned char jmpReturnInstruction[] = {
		0xE9,						 // jmp
		jmpReturnOffset & 0xFF,      // 偏移量低字节
		(jmpReturnOffset >> 8) & 0xFF,
		(jmpReturnOffset >> 16) & 0xFF,
		(jmpReturnOffset >> 24) & 0xFF
	};
	memoryForCustomAssembly[codeSize - 5] = jmpReturnInstruction[0];
	memoryForCustomAssembly[codeSize - 4] = jmpReturnInstruction[1];
	memoryForCustomAssembly[codeSize - 3] = jmpReturnInstruction[2];
	memoryForCustomAssembly[codeSize - 2] = jmpReturnInstruction[3];
	memoryForCustomAssembly[codeSize - 1] = jmpReturnInstruction[4];

	// 计算 跳转函数到执行指令的偏移量
	DWORD jmpOffset = (DWORD)memoryForCustomAssembly - (hookAddr + jumpInstructionLen);
	// 构造 替换 指令
	unsigned char replaceInstruction[] = {
		0xE9,                 // jmp [四个字节的地址]
		jmpOffset & 0xFF,       //偏移量低字节
		(jmpOffset >> 8) & 0xFF,
		(jmpOffset >> 16) & 0xFF,
		(jmpOffset >> 24) & 0xFF,
	};

	// 修改原始指令为 替换 指令
	unsigned char* pTarget = (unsigned char*) hookAddr;
	bool ok = replaceInstructionFun(pTarget, replaceInstruction, jumpInstructionLen);
	if (!ok) {
		return;
	}

}