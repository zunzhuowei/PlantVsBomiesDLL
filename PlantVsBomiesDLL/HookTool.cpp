#include "pch.h"
#include "HookTool.h"


bool HookTool::replaceInstructionFun(unsigned char*& orign, unsigned char* target, size_t size) {
	DWORD oldProtect;
	// �����ڴ汣�����ԣ�ʹ���д
	if (!VirtualProtect(orign, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		return false;
	}
	// �޸�ָ���ֵ
	for (size_t i = 0; i < size; i++)
	{
		orign[i] = target[i];
	}
	// �ָ��ڴ汣������
	if (!VirtualProtect(orign, size, oldProtect, &oldProtect)) {
		return false;
	}
	return true;
}

void HookTool::hook(DWORD& hookAddr, DWORD& hookAfterRetAddr, 
	unsigned char*& memoryForCustomAssembly, unsigned char customAssembly[],size_t codeSize) {
	size_t jumpInstructionLen = 5;
	//��ָ������ڴ���
	memcpy(memoryForCustomAssembly, customAssembly, codeSize);
	long jmpReturnOffset = hookAfterRetAddr - ((long)memoryForCustomAssembly + codeSize);
	unsigned char jmpReturnInstruction[] = {
		0xE9,						 // jmp
		jmpReturnOffset & 0xFF,      // ƫ�������ֽ�
		(jmpReturnOffset >> 8) & 0xFF,
		(jmpReturnOffset >> 16) & 0xFF,
		(jmpReturnOffset >> 24) & 0xFF
	};
	memoryForCustomAssembly[codeSize - 5] = jmpReturnInstruction[0];
	memoryForCustomAssembly[codeSize - 4] = jmpReturnInstruction[1];
	memoryForCustomAssembly[codeSize - 3] = jmpReturnInstruction[2];
	memoryForCustomAssembly[codeSize - 2] = jmpReturnInstruction[3];
	memoryForCustomAssembly[codeSize - 1] = jmpReturnInstruction[4];

	// ���� ��ת������ִ��ָ���ƫ����
	DWORD jmpOffset = (DWORD)memoryForCustomAssembly - (hookAddr + jumpInstructionLen);
	// ���� �滻 ָ��
	unsigned char replaceInstruction[] = {
		0xE9,                 // jmp [�ĸ��ֽڵĵ�ַ]
		jmpOffset & 0xFF,       //ƫ�������ֽ�
		(jmpOffset >> 8) & 0xFF,
		(jmpOffset >> 16) & 0xFF,
		(jmpOffset >> 24) & 0xFF,
	};

	// �޸�ԭʼָ��Ϊ �滻 ָ��
	unsigned char* pTarget = (unsigned char*) hookAddr;
	bool ok = replaceInstructionFun(pTarget, replaceInstruction, jumpInstructionLen);
	if (!ok) {
		return;
	}

}