#pragma once
#include "main.h"

#ifndef KEY_CODE
#define KEY_CODE

// �ڡڡڡ� Ű���� �� ���� �ڡڡڡ� //
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4 // �����ϴ� ��

#endif

int MenuDraw();        // ���ӽ��� ��� ��ư ���� �Լ�
int Login_MenuDraw();
int keyControl();       // Ű���� �Է� �� ��Ʈ�� �Լ�
void MainMenu();
void FindID_Menu();
void FindPW_Menu();
void Join_Menu();
void edit_Menu();
void login_Menu();
void friend_register_Menu();
void conversation_Menu();