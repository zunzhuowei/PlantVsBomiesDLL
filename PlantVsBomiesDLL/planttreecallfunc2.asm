.386P
.MODEL FLAT
option casemap:none

; 标记 planttreecallfunc 为导出函数
PUBLIC _planttreecallfunc

.code
; 定义 planttreecallfunc 函数，使用 cdecl 调用约定
_planttreecallfunc proc near
    push    ebp                ; 保存ebp寄存器，为后面的局部变量分配空间做准备
    mov     ebp, esp           ; 将esp寄存器的值赋给ebp寄存器，形成新的堆栈帧
    push    esi                 ; 保存其他可能使用的寄存器

    ; 获取参数
    ;在 cdecl 调用约定中，第一个参数在 [ebp + 8h] 处，第二个参数在 [ebp + 0Ch] 处，第三个参数在 [ebp + 10h] 处。
    mov ebx, [ebp + 8h]  ; 第一个参数 (x 坐标)
    mov eax, [ebp + 0Ch] ; 第二个参数 (y 坐标)
    mov ecx, [ebp + 10h] ; 第三个参数 (植物下标)

    mov esi, [7794F8h]
    mov esi, [esi + 868h]
    push eax          ; y 坐标
    push -1           ; 固定值 -1
    push ecx          ; 植物下标
    push ebx          ; x 坐标
    push esi          ; 可变
    mov edx, 00422610h
    call edx

    popad
    pop     esi                 ; 恢复保存的寄存器
    mov     esp, ebp           ; 将ebp寄存器的值赋给esp寄存器，恢复esp到调用MyFunction前的状态
    pop     ebp                ; 恢复ebp寄存器的值
    ret                        ; 函数返回           
_planttreecallfunc endp
end

;无参调用
;_planttreecallfunc proc near
;pushad
;mov esi,[7794F8h]
;mov esi,[esi+868h]
;push -1 ;ecx值固定值
;push 2  ;植物下标
;mov eax,4 ;y坐标
;push 6 ;x坐标
;push esi ;可变
;mov edx, 00422610h
;call edx
;popad
;retn
;_planttreecallfunc endp
;end