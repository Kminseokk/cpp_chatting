#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>

#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h> //�Ҹ� ���
#pragma comment(lib,"winmm.lib") //�Ҹ� ���

#include "main.h" // �⺻���� ���� �����س��� ��� �ҷ�����
#include "util.h" // Ŀ�� ���� ���
#include "menu.h" // �޴� ���� �׸���, Ŀ�� ��ǥ�� �� 
#include <conio.h>

#define MAX_SIZE 1024

using namespace std;

SOCKET client_sock;
SOCKADDR_IN client_addr = {};
string my_id, my_nick, my_pw, my_name, number, my_phonenumber, result;
string login_User_nick, login_User_id; //�α��� ���� 
string User_edit_pw; //�α����� ���� �̸� ���̵� ����
string edit_check = "N";
string friend_id; // ģ�� �߰��� ģ�� ���̵�
string chatting_friend; // ��ȭ���� ģ��
string chatting_roomnum; // ��ȭ���� ���ȣ

WSADATA wsa;
// Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�..+
// ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
// 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
int code = WSAStartup(MAKEWORD(2, 2), &wsa);

void login(); // �α��� 
bool login_flag = false;
bool stop_flag = false;
bool findID_flag = false;
bool findPW_flag = false;
bool join_flag = false;
bool join_id_flag = false;
bool dblist_flag = false;
bool register_flag = false;
bool friend_list_flag = false;
bool chat_restart_flag = false;
bool chat_list_flag = false;
bool conversation_flag = false;
bool con_flag = false;
bool user_check_flag = false;
bool User_Edit_falg = false;
void socket_init(); // �������� ����
void findID(); // ���̵� ã��
void findPW(); // �н����� ã��
void join();
void chat_list();
void User_Edit(); // ���� ���� �޴���ư 8
void friend_register();
void friend_list_recv();
void conversation();
void conversation_recv();

int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (!login_flag) { //�̰� ���� �ʼ�!!
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }
            //�����κ��� ���� �޽��� ����
            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (�̰ɷ� �α��� ���� ����) 
            // / [3] : �޴� ��� / [4] ��� ���� �̸� ) / [5] ��� ���̵�
            if (tokens[1] != my_nick) {
                if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                    if (tokens[0] == "1") {
                        result = tokens[2];
                        if (result == "1") {
                            login_User_nick = tokens[4];
                            login_User_id = tokens[5];
                            cout << endl << " ============================================ " << endl;
                            cout << " �� " << login_User_nick << " �� �α��� �Ǿ����ϴ�." << endl;
                            cout << " �� 4�� �ڿ� ���� ȭ������ ���ϴ�." << endl;
                            Sleep(4000);
                            login_flag = true; //�̰� �ؾ��� �α��� ���� ������ �� ����.
                            break;
                        }
                        else {
                            cout << endl << " ============================================ " << endl;
                            cout << " �� �α��� ����" << endl;
                            cout << " �� 4�� �ڿ� ���� ȭ������ ���ϴ�." << endl;
                            Sleep(4000);
                            stop_flag = true;
                            break;
                        }
                    }
                    else {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �α��� ����" << endl;
                        cout << " �� 4�� �ڿ� ���� ȭ������ ���ϴ�." << endl;
                        Sleep(4000);
                        stop_flag = true;
                        break;
                    }
                }
                else {
                    cout << buf << endl;
                }
            }

            if (tokens[1] == login_User_nick) {
                cout << endl << " ============================================ " << endl;
                cout << " �� " << login_User_nick << " �� �α��� �Ǿ����ϴ�." << endl;
                cout << " �� 4�� �ڿ� ���� ȭ������ ���ϴ�." << endl;
                Sleep(4000);
                login_flag = true; //�̰� �ؾ��� �α��� ���� ������ �� ����.
                break;
            }
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}

int findID_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ID ã�� ���� ����) / [3] : �޴� ��� / [4] : ���(ã�� ID) )
            if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                if (tokens[0] == "2") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ���̵� ã�� ����!" << endl;
                        string find_User_id = tokens[4];
                        cout << " �� " << my_name << " ���� ���̵�� : " << find_User_id << " �Դϴ�." << endl;
                        cout << " �� 4�� �� ���� ȭ������ ���ư��ϴ�." << endl;
                        findID_flag = true;
                        Sleep(4000);
                        break;
                        // ���⿡�� ���(result)�� ����ϰų� ó��
                    }
                    else {
                        findID_flag = false; // ID ã�� ���� ����
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ID ã�� ����. �Է��� ������ �ٽ� Ȯ�����ּ���." << endl;
                        Sleep(4000);
                        break;
                    }
                }
                else {
                    findID_flag = false; // ID ã�� ���� ����
                    cout << endl << " ============================================ " << endl;
                    cout << " �� ID ã�� ����. �Է��� ������ �ٽ� Ȯ�����ּ���." << endl;
                    Sleep(4000);
                    break;
                }
            }
            else {
                cout << buf << endl;
            }
        }
    }
}

int findPW_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ID ã�� ���� ����) / [3] : �޴� ��� / [4] : ���(ã�� ID) )
            if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                if (tokens[0] == "3") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ��й�ȣ ã�� ����!" << endl;
                        string find_User_pw = tokens[4];
                        cout << " �� " << my_name << " ���� ��й�ȣ�� : " << find_User_pw << " �Դϴ�." << endl;
                        cout << " �� 4�� �� ���� ȭ������ ���ư��ϴ�." << endl;
                        findPW_flag = true;
                        Sleep(4000);
                        break;
                        // ���⿡�� ���(result)�� ����ϰų� ó��
                    }
                    else {
                        findPW_flag = false; // ID ã�� ���� ����
                        cout << endl << " ============================================ " << endl;
                        cout << " �� PW ã�� ����. �Է��� ������ �ٽ� Ȯ�����ּ���." << endl;
                        Sleep(4000);
                        break;
                    }
                }
                else {
                    findPW_flag = false; // ID ã�� ���� ����
                    cout << endl << " ============================================ " << endl;
                    cout << " �� PW ã�� ����. �Է��� ������ �ٽ� Ȯ�����ּ���." << endl;
                    Sleep(4000);
                    break;
                }
            }
            else {
                cout << buf << endl;
            }

        }
    }
}

int join_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ȸ������ ���� ����) / [3] : �޴� ��� / [4] : ���� �̸� )
            if (tokens[1] == "server") {
                if (tokens[0] == "4") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ȸ�� ���� ����!" << endl;
                        cout << " �� " << my_name << " �� ȯ���մϴ�. " << endl;
                        cout << " �� 4�� �� ���� ȭ������ ���ư��ϴ�. " << endl;
                        join_flag = true; // ȸ������ ���� ����
                        Sleep(4000);
                        break;
                    }
                    else if (result == "3") {
                        join_flag = false;
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �Է��� ���̵� �������� �ʽ��ϴ�." << endl;
                        cout << " �� ȸ�������� �����մϴ�." << endl;
                        cout << " ============================================ " << endl;
                        join_id_flag = true;
                        Sleep(4000);
                        break;
                    }
                    else if (result == "4") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ȸ������ ����! �̹� �����ϴ� ���̵��Դϴ�. " << endl;
                        join_flag = false;
                        join_id_flag = false;
                        Sleep(4000);
                        join();
                        break;
                    }
                    else {
                        join_flag = false; // ȸ������ ���� ����
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ȸ������ ����! �Է��� ������ �ٽ� Ȯ�����ּ���. " << endl;
                        Sleep(4000);
                        join();
                        break;
                    }
                }
                else {
                    join_flag = false; // ȸ������ ���� ����
                    cout << endl << " ============================================ " << endl;
                    cout << " �� ȸ������ ����! �Է��� ������ �ٽ� Ȯ�����ּ���. " << endl;
                    Sleep(4000);
                    join();
                    break;
                }
            }


        }
    }
}

void login() {
    
    while (!login_flag) {
        system("cls");
        login_Menu();
        // �̰� �����ؾ� �ٽ� �α��νõ� ����. 
        if (login_flag == true) { break; }

        if (stop_flag == true) { break; }

        string User_input;
        string User_request = "1";
        cout << " ���̵� �Է� >> ";
        cin >> User_input;
        my_nick = User_input;
        cout << " ��й�ȣ �Է� >> ";
        cin >> User_input;
        my_pw = User_input;

        while (1) {
            string msg = User_request + " " + my_nick + " " + my_pw;
            send(client_sock, msg.c_str(), msg.length(), 0);
            break;
        }

        std::thread th2(chat_recv);

        th2.join();
    }
}

void findID() {
    system("cls");
    FindID_Menu();

    while (!code) {

        if (findID_flag == true) { break; }

        if (stop_flag == true) { break; }

        string User_request = "2"; // ID ã�� ��ȣ?
        cout << " �̸� �Է� >> ";
        cin >> my_name;

        if (my_name == "exit") { // exit �Է� �� ����ȭ��
            stop_flag = true;
            break;
        }

        while (true) {
            cout << " ��ȭ��ȣ �Է� >> ";
            cin >> number;

            if (number.length() != 11) {
                cout << endl << " ============================================ " << endl;
                cout << " �� ��ȭ��ȣ�� �ٽ� �Է����ּ���." << endl;
                continue;
            }
            break;
        }

        string first = number.substr(0, 3);
        string second = number.substr(3, 4);
        string third = number.substr(7, 4);
        my_phonenumber = first + "-" + second + "-" + third;
        string msg = User_request + " " + my_name + " " + my_phonenumber;
        send(client_sock, msg.c_str(), msg.length(), 0);

        std::thread th2(findID_recv);

        th2.join();
    }
}

void findPW() {
    system("cls");
    FindPW_Menu();

    while (!code) {

        if (findPW_flag == true) { break; }

        if (stop_flag == true) { break; }

        string User_request = "3"; // PW ã�� ��ȣ?
        cout << " ID �Է� >> ";
        cin >> my_id;

        if (my_id == "exit") {
            stop_flag == true;
            break;
        }

        cout << " �̸� �Է� >> ";
        cin >> my_name;

        while (true) {
            cout << " ��ȭ��ȣ �Է�(- ����) >> ";
            cin >> number;

            if (number.length() != 11) {
                cout << endl << " ============================================ " << endl;
                cout << " �� ��ȭ��ȣ�� �ٽ� �Է����ּ���." << endl;
                continue;
            }
            break;
        }

        string first = number.substr(0, 3);
        string second = number.substr(3, 4);
        string third = number.substr(7, 4);
        my_phonenumber = first + "-" + second + "-" + third;
        string msg = User_request + " " + my_id + " " + my_name + " " + my_phonenumber;
        send(client_sock, msg.c_str(), msg.length(), 0);

        std::thread th2(findPW_recv);
 
        th2.join();
    }
}

void join() {
    system("cls");
    Join_Menu();

    while (!code) {

        if (join_flag == true) { break; }

        if (join_id_flag == false) {

            while (1) {
                cout << " ���̵� �Է��ϼ���. >> ";
                cin >> my_id;

                if (my_id.length() > 10) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �Է� ������ �ڸ����� �Ѿ����ϴ�. �ٽ� �Է����ּ���. ";
                    continue;
                }
                else { break; }
            }

            string User_request = "41"; // ȸ������ �� ���̵� Ȯ�� ��û ��ȣ
            string msg_id = User_request + " " + my_id;
            send(client_sock, msg_id.c_str(), msg_id.length(), 0);
        }

        else if (join_id_flag == true) {
            while (1) {
                cout << " �̸��� �Է��ϼ���. >> ";
                cin >> my_name;

                if (my_name.length() > 10) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �Է� ������ �ڸ����� �Ѿ����ϴ�. �ٽ� �Է����ּ���. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " ��й�ȣ�� �Է��ϼ���. >> ";
                cin >> my_pw;

                if (my_pw.length() > 10) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �Է� ������ �ڸ����� �Ѿ����ϴ�. �ٽ� �Է����ּ���. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " ��ȭ��ȣ�� �Է��ϼ���. >> ";
                cin >> number;

                if (number.length() > 11) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �߸� �Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " �г����� �Է��ϼ���. >> ";
                cin >> my_nick;

                if (my_nick.length() > 10) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �Է� ������ �ڸ����� �Ѿ����ϴ�. �ٽ� �Է����ּ���. ";
                    continue;
                }
                else { break; }
            }

            string first = number.substr(0, 3);
            string second = number.substr(3, 4);
            string third = number.substr(7, 4);
            my_phonenumber = first + "-" + second + "-" + third;
            string User_request = "4"; // ȸ������ �� ���̵� Ȯ�� ��û ��ȣ
            string msg = User_request + " " + my_id + " " + my_name + " " + my_pw + " " + my_phonenumber + " " + my_nick; // ģ�� ����� ����?
            send(client_sock, msg.c_str(), msg.length(), 0);

        }

        std::thread th(join_recv);

        while (1) {
            break;
            string text;
            std::getline(cin, text);
            const char* buffer = text.c_str();
            send(client_sock, buffer, strlen(buffer), 0);
        }
        th.join();
    }
}

int edit_recv() {
    string msg;

    while (!User_Edit_falg) {
        char buf[MAX_SIZE] = { };
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ID ã�� ���� ����) / [3] : �޴� ��� / [4] : ���(ã�� ID) )
            if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                if (tokens[0] == "8") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �Է��Ͻ� ��й�ȣ�� ��ġ�մϴ�." << endl;
                        cout << " �� 4�� �� ���� �������� �Ѿ�ϴ�." << endl;
                        Sleep(4000);
                        system("cls");
                        edit_Menu();

                        cout << " ============================================ " << endl;
                        cout << " �� �����Ͻ� ��й�ȣ�� �Է��ϼ���. >> ";
                        string User_input;
                        string User_request = "8";
                        edit_check = "Y";

                        cin >> User_input;
                        User_edit_pw = User_input;

                        while (1) {
                            string msg = User_request + " " + User_edit_pw + " " + login_User_id + " " + edit_check;
                            send(client_sock, msg.c_str(), msg.length(), 0);
                            break;
                        }
                        // ���⿡�� ���(result)�� ����ϰų� ó��
                    }

                    else if (result == "2") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ��й�ȣ�� �߸� �Է��ϼ̽��ϴ�. " << endl;

                        Sleep(4000);
                        break;
                        User_Edit();

                    }
                    else if (result == "3") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ��й�ȣ�� ����Ǿ����ϴ�." << endl;
                        User_Edit_falg = true;
                        tokens.clear();
                        Sleep(4000);
                        break;
                    }
                    else {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �������." << endl;
                        Sleep(4000);
                    }
                }
                else {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� �������." << endl;
                    Sleep(4000);
                    User_Edit();
                }
            }
            else {
                cout << buf << endl;
            }
        }
    }
}


void User_Edit() {
    while (edit_check == "N") {
        system("cls");
        edit_Menu();

        //edit_check �� �⺻�� N
        string User_input;
        string User_request = "8";

        cout << " ============================================ " << endl;
        cout << " " << login_User_nick << " ���� ��й�ȣ ���� ������" << endl;
        cout << " [����] ���� ��й�ȣ�� �Է��ϼ���. >> ";
        cin >> User_input;
        User_edit_pw = User_input;

        while (1) {
            string msg = User_request + " " + User_edit_pw + " " + login_User_id + " " + edit_check;
            send(client_sock, msg.c_str(), msg.length(), 0);
            break;
        }
        std::thread th2(edit_recv);

        th2.join();
    }
}

int chatlist_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (!dblist_flag) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            system("cls");
            // ������û ��ȣ / ������� : server / �޴»�� : �� / ���� ��ȭ�ϴ� ��� / ��ȭ ����
            std::vector<std::string> tokens;
            std::vector<std::string> DB_contents;
            std::vector<std::string> DB_contents_separate;
            std::stringstream ss(buf);
            std::string token, content;

            while (std::getline(ss, token, '/')) {
                // "/"�� ������ ù ��° �κ��� ������ �������� �и��Ͽ� tokens ���Ϳ� ����
                if (tokens.empty()) {
                    std::stringstream tokens_iss(token);
                    while (tokens_iss >> token) {
                        tokens.push_back(token);
                    }
                }
                else {
                    // "/"�� ������ ������ �κ��� "\n"�� �������� �и��Ͽ� ����
                    std::stringstream DB_contents_iss(token);
                    std::string line;
                    while (std::getline(DB_contents_iss, line, '\n')) {
                        DB_contents.push_back(line);
                    }
                    std::stringstream DB_contents_separate_iss(line);
                    while (DB_contents_separate_iss >> line) {
                        DB_contents_separate.push_back(line);
                    }
                }
            }

            for (int i = 0; i < DB_contents.size(); i++) {
                content = DB_contents[i];
                std::stringstream DB_contents_separate_iss(content);

                while (DB_contents_separate_iss >> content) {
                    DB_contents_separate.push_back(content);
                }
            }

            if (tokens[0] == "5") {
                std::cout << tokens[3] << "�� ��ȭ�ϰ� �ֽ��ϴ�." << endl;
                cout << " ============================================ " << endl;

                // second ���� �� ���
                for (size_t i = 0; i < DB_contents_separate.size(); i++) {
                    std::cout << "[" << DB_contents_separate[i+1] << "] " << DB_contents_separate[i+2] << " (" << DB_contents_separate[i+3] + " " + DB_contents_separate[i+4] << ")" << std::endl;
                    i += 4;
                }
                PlaySound(TEXT("katalk.wav"), 0, SND_FILENAME | SND_ASYNC); //�Ϲ� ���
                cout << " ============================================ " << endl;
                cout << " ���� �޼����� �Է��ϼ���. >> ";
            }
            else if (tokens[0] == "501") {
                break;
            }

            //break;
        }
    }
}

void chat_list() {

    while (!chat_list_flag) {
        system("cls");

        while (1) {
            string User_request = "5"; //ä���ϱ� �ʹݺ�
            string msg = User_request + " " + login_User_id + " " + chatting_friend + " " + chatting_roomnum;
            send(client_sock, msg.c_str(), msg.length(), 0);
            break;
        }

        std::thread th2(chatlist_recv);
        while (std::cin.get() != '\n'); //ä���ϱ� ���ö� ���� �����̽��� ����

        cout << " ============================================ " << endl;
        while (1) {
            string User_request = "51";
            if (std::cin.peek() != EOF) { //������� �Է��� ������
                string user_msg;
                getline(cin, user_msg); // ����� �Է��� �� �ٷ� �б�
                
                // ������ ��ĭ(����)�� ����
                int start = 0;
                while (1) {
                    string r = "��";
                    int index = user_msg.find(" ", start);
                    if (index == -1) { break; }
                    user_msg.replace(index, 1, r);
                    start = index + 1;
                }

                if (user_msg == "exit") {
                    chat_list_flag = true;
                    string User_request = "52";
                    string msg = User_request + " " + login_User_id;
                    send(client_sock, msg.c_str(), msg.length(), 0);
                    break;
                }
                string msg = User_request + " " + login_User_id + " " + user_msg + " " + chatting_roomnum;
                send(client_sock, msg.c_str(), msg.length(), 0);
            }
        }
        cout << " ============================================ " << endl;
        th2.join();
        break;
    }
}

void friend_register() {
    system("cls");

    while (!code) {

        while (!register_flag) {

            if (stop_flag == true) { break; }

            if (friend_list_flag == false) {
                string User_request = "71";
                string msg = User_request + " " + login_User_id;
                send(client_sock, msg.c_str(), msg.length(), 0);
            }

            if (friend_list_flag == true) {
                while (1) {
                    cout << " �߰��� ģ���� ���̵� �Է��ϼ���. >> ";
                    cin >> friend_id;

                    // �� ���̵�� �߰��� �� ����
                    if (login_User_id == friend_id) {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ģ�� �߰��� ������ �� ���� ���̵��Դϴ�. " << endl;
                        cout << " �� ���̵� �ٽ� Ȯ�����ּ���. " << endl;
                        continue;
                    }
                    else { break; }
                }

                if (friend_id == "exit") {
                    stop_flag = true;
                    break;
                }

                string User_request = "7"; //
                string msg_register = User_request + " " + login_User_id + " " + friend_id;
                send(client_sock, msg_register.c_str(), msg_register.length(), 0);
            }

            std::thread th(friend_list_recv);

            th.join();
        }
        break;
    }
}

void friend_list_recv() {

    while (!register_flag) {
        system("cls");
        friend_register_Menu();

        char buf[MAX_SIZE] = { };
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ID ã�� ���� ����) / [3] : �޴� ��� / [4] : ã�� ģ�� ����Ʈ(����) )
            if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                if (tokens[0] == "71") {
                    result = tokens[2];
                    if (result == "1" && tokens.size() > 4) { // �迭 ����ִ°� empty �Լ��� �߰� ���� �ʵ��� �����ϱ�
                        cout << " �� ���� ��ϵǾ� �ִ� ģ�� ����Դϴ�. " << endl;
                        cout << " ============================================ " << endl;
                        for (int i = 4; i < tokens.size(); i++) {
                            cout << "  " << i - 3 << ". ID : " << tokens[i] << endl;
                        }
                        cout << " ============================================ " << endl;
                        friend_list_flag = true;
                        Sleep(4000);
                        break;
                    }
                    else if (result == "2") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ���� ��ϵǾ� �ִ� ģ���� �����ϴ�." << endl;
                        cout << " �� ��ȭ�� ���ϸ� ģ���� �߰����ּ���." << endl;
                        cout << " ============================================ " << endl;
                        friend_list_flag = true;
                        Sleep(4000);
                        break;
                    }
                }
                else if (tokens[0] == "7") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ģ�� �߰��� �Ϸ� �Ǿ����ϴ�. " << endl;
                        register_flag = true;
                        Sleep(4000);
                    }
                    else if (result == "2") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �̹� �����ϴ� ģ���Դϴ�." << endl;
                        register_flag = false;
                        Sleep(4000);
                        friend_register();
                    }
                    else if (result == "3") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� �������� �ʴ� ������Դϴ�." << endl;
                        cout << " �� ID�� �ٽ� Ȯ�����ּ���." << endl;
                        register_flag = false;
                        Sleep(4000);
                        friend_register();
                    }
                }
            }
        }
    }
}

void conversation() { //6 ģ�� ��� ��������

    while (1) {

        if (conversation_flag == false) {
            system("cls");
            conversation_Menu();
            string User_request = "6"; // ��ȭ�ϱ� �� ���� ä�ù� �ִ��� Ȯ��
            string msg = User_request + " " + login_User_id;
            send(client_sock, msg.c_str(), msg.length(), 0);
        }

        if (conversation_flag == true && user_check_flag == false) {

            while (1) {
                cout << endl << " ============================================ " << endl;
                cout << " �� ��ȭ�� ���ϴ� ������� ���̵� �Է��ϼ���. (�ű� ��ȭ�� ����) >> ";
                cin >> friend_id;

                // �� �ڽŰ� ��ȭ �Ұ���
                if (friend_id == login_User_id) {
                    cout << endl << " ============================================ " << endl;
                    cout << " �� ��ȭ�� ������ �� ���� ���̵��Դϴ�. " << endl;
                    cout << " �� ���̵� �ٽ� Ȯ�����ּ���. " << endl;
                    continue;
                }
                else { break; }
            }

            if (friend_id == "exit") {
                stop_flag = true;
                break;
            }

            string User_request = "61";
            string msg = User_request + " " + login_User_id + " " + friend_id;
            send(client_sock, msg.c_str(), msg.length(), 0);
        }

        //����Ʈ �ҷ����� true + ��� ���̵� ������ true
        if (conversation_flag == true && user_check_flag == true) { break; }

        if (stop_flag == true) { break; }

        std::thread th(conversation_recv);

        th.join();

    }
}

void conversation_recv() {

    while (!con_flag) {
        //system("cls");
        char buf[MAX_SIZE] = { };
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            // ���ڿ��� ��Ʈ���� �ְ� ������ �������� �����Ͽ� ���Ϳ� ����
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : ��û ��� (1=�α��� ��) / [1] : ���� ��� ( �ظ��ؼ� "server") / [2] : ����� (ID ã�� ���� ����) / [3] : �޴� ��� / [4] : ã�� ģ�� ����Ʈ(����) )
            if (tokens[1] == "server") { // �����κ��� ���� �޽����� 
                //ss >> result; // ���
                if (tokens[0] == "6") {
                    result = tokens[2];
                    if (result == "1") { // �迭 ����ִ°� empty �Լ��� �߰� ���� �ʵ��� �����ϱ�
                        cout << " �� ���� ��ȭ�� �����ߴ� ģ�� ����Դϴ�. " << endl;
                        cout << " ============================================ " << endl;
                        for (int i = 4; i < tokens.size(); i++) {
                            cout << "  " << i - 3 << ". ID : " << tokens[i] << endl;
                        }
                        cout << " ============================================ " << endl;

                        conversation_flag = true;
                        Sleep(1000);
                        break;
                    }
                    else if (result == "2") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ���� ��ȭ�� �����ߴ� ģ���� �����ϴ�. " << endl;
                        conversation_flag = true;
                        Sleep(1000);
                        break;
                        conversation();
                    }
                    else if (result == "3") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ���̵� �˻� ����! ���� ��ϵǾ� �ִ� ����ڰ� �ƴմϴ�." << endl;
                        user_check_flag = false;
                        Sleep(1000);
                        break;
                        conversation();
                    }
                    else if (result == "4") {
                        cout << endl << " ============================================ " << endl;
                        cout << " �� ���̵� �˻� ����! ��ȭ���� �ҷ��ɴϴ�." << endl;
                        //break;
                    }

                }
                else if (tokens[0] == "601") { //���������� �� ������� �� ����� �;���
                    chatting_friend = tokens[3];
                    chatting_roomnum = tokens[4];
                    user_check_flag = true; // �� �κ� �ʿ�
                    break;
                }
            }
        }
    }
}

void socket_init() {

    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

    // ������ ���� ���� ���� �κ�
    //SOCKADDR_IN client_addr = {};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(7777);
    InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) { // ���� ������ ������ �ش��ϴ� server�� ����!            
            cout << "***SYSTEM MESSAGE : Server Connect***" << endl;
            string msg = "auto produce";
            send(client_sock, msg.c_str(), msg.length(), 0); // ���ῡ �����ϸ� client �� �Է��� �г����� ������ ����
            //send(client_sock, my_pw.c_str(), my_pw.length(), 0);          

            break;
        }
        cout << "Connecting..." << endl;
    }
}

int main()
{
    init(); //Ŀ�� �����Ÿ��°� �������ִ� �Լ�
    socket_init();
    while (1) {
        //std::thread th2(chat_recv);
        if (login_flag == false) {
            stop_flag = false;
            findID_flag = false;
            findPW_flag = false;
            join_id_flag = false;
            join_flag = false;
            MainMenu(); // ���� �޴� �׸��� ������ ȣ��
            int menuCode = MenuDraw(); // ���ӽ��� ��ư ���� �� y��ǥ �� ����
            //printf("�޴� �ڵ�� %d ", menuCode); <<�� y��ǥ Ȯ�� ����
            // 1:�α��� / 2:idã�� / 3:pwã�� / 4:ȸ������ / 5:��ȭ / 6:����ä�� / 7:ģ�� / 8:�������
            if (menuCode == 0) {
                login(); // �α��� �Լ� ����
            }
            else if (menuCode == 1) { // 1 ���̵� ã��
                findID();
            }
            else if (menuCode == 2) { // 2 ��й�ȣ ã��
                findPW();
            }
            else if (menuCode == 3) { // 3 ȸ�� ����
                join();
            }
            else if (menuCode == 4) { // 4 ȸ�� ����
                cout << "\n\n\n";
                WSACleanup();
                return 0; // ����
            }
            system("cls"); // �ܼ�â�� Ŭ�� �϶� �ǹ�
        }
        //�α��� �������� ���� Ʈ��� �ٲ������Ƿ�, �α��� ���� ���� ���̵� ��µ�.
        else if (login_flag == true) {
            edit_check = "N";
            User_Edit_falg = false;
            dblist_flag = false;
            register_flag = false;
            friend_list_flag = false;
            chat_list_flag = false;
            conversation_flag = false;
            user_check_flag = false;
            stop_flag = false;
            MainMenu(); // ���� �޴� �׸��� ������ ȣ��

            cout << endl << "\t    "; cout << "�α��� ����! " << login_User_nick << " �� ȯ���մϴ�." << endl;
            //cout << "�ּ� ó�� �ʼ�! Ȯ�ο�! " << login_User_id << " �� ȯ���մϴ�." << endl;
            //cout << edit_check << " ���̵�ƮƮ üũ " << User_Edit_falg << "�÷��� " << endl;
            int menuCode = Login_MenuDraw();

            if (menuCode == 0) { // 5 ��ȭ�ϱ�
                system("cls");
                conversation(); // ģ�� ��� ��������
                if (stop_flag == false) {
                    chat_list();
                }
            }
            else if (menuCode == 1) { // 6 ���� ��ȭ�� �ҷ�����
                conversation();
                if (stop_flag == false) {
                    chat_list();
                }
            }
            else if (menuCode == 2) { // 7 ģ�� �߰�
                friend_register();
            }
            else if (menuCode == 3) { // 8 ��� ��ȣ ����
                User_Edit();
            }
            else if (menuCode == 4) {  // ���� �ϱ�
                cout << "\n\n\n";
                WSACleanup();

                return 0; // ����
            }
            system("cls"); // �ܼ�â�� Ŭ�� �϶� �ǹ�
        }
        system("cls"); // �ܼ�â�� Ŭ�� �϶� �ǹ�
    }
}