.386P
.MODEL FLAT
option casemap:none

; ��� planttreecallfunc Ϊ��������
PUBLIC _planttreecallfunc

.code
; ���� planttreecallfunc ������ʹ�� cdecl ����Լ��
_planttreecallfunc proc near
    push    ebp                ; ����ebp�Ĵ�����Ϊ����ľֲ���������ռ���׼��
    mov     ebp, esp           ; ��esp�Ĵ�����ֵ����ebp�Ĵ������γ��µĶ�ջ֡
    push    esi                 ; ������������ʹ�õļĴ���

    ; ��ȡ����
    ;�� cdecl ����Լ���У���һ�������� [ebp + 8h] �����ڶ��������� [ebp + 0Ch] ���������������� [ebp + 10h] ����
    mov ebx, [ebp + 8h]  ; ��һ������ (x ����)
    mov eax, [ebp + 0Ch] ; �ڶ������� (y ����)
    mov ecx, [ebp + 10h] ; ���������� (ֲ���±�)

    mov esi, [7794F8h]
    mov esi, [esi + 868h]
    push eax          ; y ����
    push -1           ; �̶�ֵ -1
    push ecx          ; ֲ���±�
    push ebx          ; x ����
    push esi          ; �ɱ�
    mov edx, 00422610h
    call edx

    popad
    pop     esi                 ; �ָ�����ļĴ���
    mov     esp, ebp           ; ��ebp�Ĵ�����ֵ����esp�Ĵ������ָ�esp������MyFunctionǰ��״̬
    pop     ebp                ; �ָ�ebp�Ĵ�����ֵ
    ret                        ; ��������           
_planttreecallfunc endp
end

;�޲ε���
;_planttreecallfunc proc near
;pushad
;mov esi,[7794F8h]
;mov esi,[esi+868h]
;push -1 ;ecxֵ�̶�ֵ
;push 2  ;ֲ���±�
;mov eax,4 ;y����
;push 6 ;x����
;push esi ;�ɱ�
;mov edx, 00422610h
;call edx
;popad
;retn
;_planttreecallfunc endp
;end