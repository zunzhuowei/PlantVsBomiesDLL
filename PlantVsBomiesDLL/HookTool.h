#pragma once
class HookTool
{
public:
	/// <summary>
	/// �滻����ԭ��ָ��
	/// </summary>
	/// <param name="orign">
	/// ԭ��ָ���ֽ������ڵ�ַ
	/// DWORD targetAddress = 0x004838DC;
	/// unsigned char* pTarget = (unsigned char*)targetAddress;
	/// </param>
	/// <param name="target">
	/// �滻���ֽ�������
	/// unsigned char nopInstruction[] = { 0x90 };
	/// </param>
	/// <param name="size">
	/// �滻���ֽڸ�����char��������target����ĸ���
	/// </param>
	/// <returns>�ɹ�����ʧ��</returns>
	bool replaceInstructionFun(unsigned char*& orign, unsigned char* target, size_t size);

	/// <summary>
	/// hook���ָ��
	/// </summary>
	/// <param name="hookAddr">
	/// Ҫhook��ָ���ַ
	/// DWORD targetAddress = 0x004838DC;
	/// </param>
	/// <param name="hookAfterRetAddr">
	/// hook֮����ת���ĵ�ַ
	/// DWORD returnAddress = 0x004838E2;
	/// </param>
	/// <param name="memoryForCustomAssembly">
	/// Ϊ�Զ���Ļ��ָ��ٵĿռ��С
	/// unsigned char* pRemoteBuf = (unsigned char*)VirtualAlloc(NULL, sizeof(customAssembly), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	/// </param>
	/// <param name="customAssembly">
	/// �Զ����hook���ָ������룩
	/// unsigned char customAssembly[] = {
	/// 0xC7, 0x43, 0x58, 0x30, 0x00, 0x00, 0x00, // mov [ebx+58], 30
	/// 0x83, 0xF8, 0x12,                         // cmp eax, 12
	/// 0xE9, 0xe2, 0x38, 0x48, 0x00           // jmp 004838E2����������ռλ��ת��ָ����ַ�������л����������ƫ�������ǣ�
	/// };
	/// </param>
	/// <param name="codeSize">
	/// �������ֽ���
	/// </param>
	void hook(DWORD& hookAddr, DWORD& hookAfterRetAddr, unsigned char*& memoryForCustomAssembly, unsigned char customAssembly[], size_t codeSize);

};

