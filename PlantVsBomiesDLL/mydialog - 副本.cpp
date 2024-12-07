// mydialog.cpp: 实现文件
//

#include "pch.h"
#include "PlantVsBomiesDLL.h"
#include "afxdialogex.h"
#include "mydialog.h"
#include <vector>
#include <iostream>

// mydialog 对话框

IMPLEMENT_DYNAMIC(mydialog, CDialogEx)

mydialog::mydialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

mydialog::~mydialog()
{

}

void mydialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editSunNum);
	DDX_Control(pDX, IDC_EDIT2, debugLogEdit);
}


BEGIN_MESSAGE_MAP(mydialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &mydialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &mydialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &mydialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &mydialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &mydialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &mydialog::OnBnClickedButton6)
END_MESSAGE_MAP()


// mydialog 消息处理程序

/// <summary>
/// 查询阳光值
/// </summary>
void mydialog::OnBnClickedButton1()
{
	/*
	--无限阳光
007794F8 + 868 + 5578
*/
//由于游戏是32位程序，所以地址位4个字节长度，所以用DWORD接收；

	DWORD baseAddr = *((DWORD*)0x00779618);
	baseAddr = *((DWORD*)(baseAddr + 0x868));
	int sunNum = *((int*)(baseAddr + 0x5578));
	CString str;
	str.Format(L"%d", sunNum);
	editSunNum.SetWindowTextW(str);
}

/// <summary>
/// 设置阳光值
/// </summary>
void mydialog::OnBnClickedButton2()
{
	DWORD baseAddr = *((DWORD*)0x00779618);
	baseAddr = *((DWORD*)(baseAddr + 0x868));
	int* sunNumAddr = (int*)(baseAddr + 0x5578);
	*sunNumAddr = 9999;
	mydialog::OnBnClickedButton1();
}

/// <summary>
/// 修改代码区的指令
/// </summary>
/// <typeparam name="T">指针类型</typeparam>
/// <param name="pointer">指针</param>
/// <param name="data">要替换的数据</param>
template<typename T>
void modifyCodeAreaDataByPointer(T* pointer,int data) {
	DWORD oldProtect;

	// 更改内存保护属性，使其可写
	if (!VirtualProtect(pointer, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) {
		return;
	}
	// 修改指令的值
	*pointer = data;
	// 恢复内存保护属性
	if (!VirtualProtect(pointer, sizeof(T), oldProtect, &oldProtect)) {
		return;
	}
}

/// <summary>
/// 道具无冷却
/// </summary>
void mydialog::OnBnClickedButton3()
{
	//地址里面存的是两个字节，所以需要用两个字节的指针接收
	WORD* baseAddr = (WORD*)0x004B2FF3;
	modifyCodeAreaDataByPointer(baseAddr, 0x127F);
}

/// <summary>
/// 食人花无冷却
/// </summary>
void mydialog::OnBnClickedButton4()
{
	//地址里面存的是两个字节，所以需要用两个字节的指针接收
	WORD* baseAddr = (WORD*)0x004855F5;
	modifyCodeAreaDataByPointer(baseAddr, 0xf574);
}

/// <summary>
/// 自动拾取阳光和金币
/// </summary>
void mydialog::OnBnClickedButton5()
{
	DWORD* baseAddr = (DWORD*)0x0044C5EE;
	modifyCodeAreaDataByPointer(baseAddr, 0x01507b80);
}



//FF5C38D3 源数
//1111 1111 0101 1100 0011 1000 1101 0011 源码
//1000 0000 1010 0011 1100 0111 0010 1100 +1 -00 A3 C7 2C 反码
//1000 0000 1010 0011 1100 0111 0010 1101    -00 A3 C7 2D 补码
//11 18 30 D8
//0001 0001 0001 1000 0011 0000 1100 1000
//0110 1110 1110 0111 1100 1111 0011 0111 +1
//                                        -1
//0001 0001 0001 1000 0011 0000 1100 0111
//0110 1110 1110 0111 1100 1111 0011 1000 //6E E7 CF 38

//116069BA -> 004838E1
//004838E1 - 116069BA = -1118 30D9（目标地址-起始地址）
//1001 0001 0001 1000 0011 0000 1101 1001 -1
//1001 0001 0001 1000 0011 0000 1101 1000 反码
//1001 0001 0001 1000 0011 0000 1101 1000 
//

// 自定义的汇编代码
unsigned char customAssembly[] = {
	0xC7, 0x43, 0x58, 0x30, 0x00, 0x00, 0x00, // mov [ebx+58], 30
	0x83, 0xF8, 0x12,                         // cmp eax, 12
	//0xC3                                      // ret
	0xE9, 0xe2, 0x38, 0x48, 0x00           // jmp 004838E2
};
// 分配可执行内存
unsigned char* pRemoteBuf = (unsigned char*)VirtualAlloc(NULL, sizeof(customAssembly), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
//替换进程原有指令
void replaceInstructionFun(unsigned char*& orign, unsigned char* target, CEdit& log) {
	CString str;
	DWORD oldProtect;
	unsigned int instructionSize = sizeof(orign) / sizeof(orign[0]);
	// 更改内存保护属性，使其可写
	if (!VirtualProtect(orign, instructionSize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		str.Format(L"%d", instructionSize);
		log.SetWindowTextW(str);
		return;
	}
	// 修改指令的值
	unsigned int size = sizeof(target) / sizeof(target[0]);
	for (size_t i = 0; i < size; i++)
	{
		orign[i] = target[i];
	}
	// 恢复内存保护属性
	if (!VirtualProtect(orign, instructionSize, oldProtect, &oldProtect)) {
		str.Format(L"%d", 22);
		log.SetWindowTextW(str);
		return;
	}
}

void mydialog::OnBnClickedButton6() {
	// 目标地址
	DWORD targetAddress = 0x004838DC;
	DWORD nopAddress = 0x004838E1;
	DWORD returnAddress = 0x004838E2;

	CString str;

	// 分配内存用于存放自定义汇编代码
	//unsigned char* pRemoteBuf = customAssembly;
	memcpy(pRemoteBuf, customAssembly, sizeof(customAssembly));

	//跳转返回源地址的偏移量计算
	unsigned long num = sizeof(customAssembly) / customAssembly[0];
	long jmpReturnOffset = returnAddress - ((long)pRemoteBuf + num);
	unsigned char jmpReturnInstruction[] = {
		0xE9,                 // jmp
		jmpReturnOffset & 0xFF,       // 偏移量低字节
		(jmpReturnOffset >> 8) & 0xFF,
		(jmpReturnOffset >> 16) & 0xFF,
		(jmpReturnOffset >> 24) & 0xFF
	};
	pRemoteBuf[10] = jmpReturnInstruction[0];
	pRemoteBuf[11] = jmpReturnInstruction[1];
	pRemoteBuf[12] = jmpReturnInstruction[2];
	pRemoteBuf[13] = jmpReturnInstruction[3];
	pRemoteBuf[14] = jmpReturnInstruction[4];



	// 计算 跳转函数到执行指令的偏移量
	DWORD jmpOffset = (DWORD)pRemoteBuf - (targetAddress + 5);
	// 构造 替换 指令
	unsigned char replaceInstruction[] = {
		0xE9,                 // jmp [四个字节的地址]
		jmpOffset & 0xFF,       //偏移量低字节
		(jmpOffset >> 8) & 0xFF,
		(jmpOffset >> 16) & 0xFF,
		(jmpOffset >> 24) & 0xFF,         
	};

	// 修改原始指令为 替换 指令
	unsigned char* pTarget = (unsigned char*)targetAddress;
	replaceInstructionFun(pTarget, replaceInstruction, debugLogEdit);
	// nop 指令用于占位
	unsigned char nopInstruction[] = { 0x90 };
	unsigned char* nopTarget = (unsigned char*)nopAddress;
	replaceInstructionFun(nopTarget, nopInstruction, debugLogEdit);

	//str.Format(L"%d", 5);
	//debugLogEdit.SetWindowTextW(str);
	std::cout << "Modification successful." << std::endl;

	//不是放内存是因为退出窗口时，进程还在请求该内存
	//VirtualFree(pRemoteBuf, 0, MEM_RELEASE);
}
