#include "menu.h"
#include "util.h"

#include<iostream>
#include<stdio.h> 
#include<vector>
#include<windows.h> //��ǥ �ڵ鷯 ���
#include<conio.h> //_getch�� ���ԵǾ��ִ� ���



int keyControl() {
    char temp = _getch(); //�Է��� ���� �ܼ�â�� �ȳ����� �ٷ� �޾���. ����߰� �ʼ�

    if (temp == 'w' || temp == 'W') {
        return UP;
    }
    else if (temp == 'a' || temp == 'A') {
        return LEFT;
    }
    else if (temp == 's' || temp == 'S') {
        return DOWN;
    }
    else if (temp == 'd' || temp == 'D') {
        return RIGHT;
    }
    else if (temp == ' ') {
        return SUBMIT;
    }

}

void MainMenu() {
    cout << "�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��\n";
    cout << "��                                                       ��\n";
    cout << "��       ****    *      *         *        *********     ��\n";
    cout << "��     *         *      *        * *           *         ��\n";
    cout << "��    *          *      *       *   *          *         ��\n";
    cout << "��    *          ********      *******         *         ��\n";
    cout << "��    *          *      *     *       *        *         ��\n";
    cout << "��     *         *      *    *         *       *         ��\n";
    cout << "��       ****    *      *   *           *      *         ��\n";
    cout << "��                                                       ��\n";
    cout << "��              *****      ****    *     *               ��\n";
    cout << "��              *    *    *    *    *   *                ��\n";
    cout << "��              *    *   *      *    * *                 ��\n";
    cout << "��              *  **    *      *     *                  ��\n";
    cout << "��              *    *   *      *    * *                 ��\n";
    cout << "��              *    *    *    *    *   *                ��\n";
    cout << "��              *****      ****    *     *               ��\n";
    cout << "��                                                       ��\n";
    cout << "�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��\n\n";
    cout << "          ���۹�� W A S D / ���� : �����̽���   \n";

    //getchar(); // �ƹ�Ű �Է� ��ٸ�
    //system("cls"); // �ܼ� â clear
};

int MenuDraw() { //ù ���� ����ȭ��

    // �ӽ� �׽�Ʈ ��ġ �ľ� �뵵�� ���!
    //gotoxy(2, 2);
    //printf("2, 2 ��ġ�Դϴ�.");
    // �ӽ� �׽�Ʈ ��ġ �ľ� �뵵�� �����!
    // [ �̵����� ]
    // ���� : x����
    // ���� : x����
    // �Ʒ� : y����
    // ���� : y����
    // 
     // �޴� �ڵ� 0:�α��� / 1:idã�� / 2:pwã�� / 3:ȸ������ / 4:��ȭ / 5:����ä�� / 6:ģ�� / 7:�������
    int x = 21;
    int y = 24;
    gotoxy(x - 2, y); // 20 , 24
    printf(">  �α���  "); //�ٸ� �ֵ麸�� x���� ������ > ������ �ڿ������� ���Ϸ���
    gotoxy(x, y + 1); // 21 , 25
    printf(" ID ã�� ");
    gotoxy(x, y + 2); // 21, 26
    printf(" PW ã�� ");
    gotoxy(x, y + 3); // 21, 27
    printf(" ȸ�� ���� ");
    gotoxy(x, y + 4); // 21, 28
    printf(" ���α׷� ���� ");

    while (1) {
        int n = keyControl(); //Ű�� �ޱ�
        switch (n) {
        case UP: {
            if (y > 24) {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, --y);
                printf(">");
            }
            break;
        }
        case DOWN: {
            if (y < 28) {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, ++y);
                printf(">");
            }
            break;
        }
        case SUBMIT: {
            return y - 24; // ���� ��ġ�� ���� �޴� ������ 0 , 1 , 2 ������ ����.
        }
        }//����ġ�� �ݴ� ��

    }
}

int Login_MenuDraw() { //ù ���� ����ȭ��

    // �ӽ� �׽�Ʈ ��ġ �ľ� �뵵�� ���!
    //gotoxy(2, 2);
    //printf("2, 2 ��ġ�Դϴ�.");
    // �ӽ� �׽�Ʈ ��ġ �ľ� �뵵�� �����!
    // [ �̵����� ]
    // ���� : x����
    // ���� : x����
    // �Ʒ� : y����
    // ���� : y����

    int x = 21;
    int y = 24;
    gotoxy(x - 2, y); // 20 , 24
    printf(">  ��ȭ�ϱ�  "); //�ٸ� �ֵ麸�� x���� ������ > ������ �ڿ������� ���Ϸ���
    gotoxy(x, y + 1); // 21 , 25
    printf(" ���� ��ȭ�� ");
    gotoxy(x, y + 2); // 21, 26
    printf(" ģ�� �߰� ");
    gotoxy(x, y + 3); // 21, 27
    printf(" ��й�ȣ ���� ");
    gotoxy(x, y + 4); // 21, 28
    printf(" ���α׷� ���� ");

    while (1) {
        int n = keyControl(); //Ű�� �ޱ�
        switch (n) {
        case UP: {
            if (y > 24) {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, --y);
                printf(">");
            }
            break;
        }
        case DOWN: {
            if (y < 28) {
                gotoxy(x - 2, y);
                printf(" ");
                gotoxy(x - 2, ++y);
                printf(">");
            }
            break;
        }
        case SUBMIT: {
            return y - 24; // ���� ��ġ�� ���� �޴� ������ 3, 4, 5 ������ ����.
        }
        }//����ġ�� �ݴ� ��

    }
}
