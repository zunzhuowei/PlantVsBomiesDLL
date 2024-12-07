#pragma once
class HookTool
{
public:
	/// <summary>
	/// 替换进程原有指令
	/// </summary>
	/// <param name="orign">
	/// 原有指令字节码所在地址
	/// DWORD targetAddress = 0x004838DC;
	/// unsigned char* pTarget = (unsigned char*)targetAddress;
	/// </param>
	/// <param name="target">
	/// 替换的字节码数组
	/// unsigned char nopInstruction[] = { 0x90 };
	/// </param>
	/// <param name="size">
	/// 替换的字节个数（char），等于target数组的个数
	/// </param>
	/// <returns>成功或者失败</returns>
	bool replaceInstructionFun(unsigned char*& orign, unsigned char* target, size_t size);

	/// <summary>
	/// hook汇编指令
	/// </summary>
	/// <param name="hookAddr">
	/// 要hook的指令地址
	/// DWORD targetAddress = 0x004838DC;
	/// </param>
	/// <param name="hookAfterRetAddr">
	/// hook之后跳转到的地址
	/// DWORD returnAddress = 0x004838E2;
	/// </param>
	/// <param name="memoryForCustomAssembly">
	/// 为自定义的汇编指令开辟的空间大小
	/// unsigned char* pRemoteBuf = (unsigned char*)VirtualAlloc(NULL, sizeof(customAssembly), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	/// </param>
	/// <param name="customAssembly">
	/// 自定义的hook汇编指令（机器码）
	/// unsigned char customAssembly[] = {
	/// 0xC7, 0x43, 0x58, 0x30, 0x00, 0x00, 0x00, // mov [ebx+58], 30
	/// 0x83, 0xF8, 0x12,                         // cmp eax, 12
	/// 0xE9, 0xe2, 0x38, 0x48, 0x00           // jmp 004838E2；该行用于占位跳转回指定地址，函数中会计算重新其偏移量覆盖；
	/// };
	/// </param>
	/// <param name="codeSize">
	/// 机器码字节数
	/// </param>
	void hook(DWORD& hookAddr, DWORD& hookAfterRetAddr, unsigned char*& memoryForCustomAssembly, unsigned char customAssembly[], size_t codeSize);

};

