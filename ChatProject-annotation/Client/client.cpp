#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>

#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h> //소리 출력
#pragma comment(lib,"winmm.lib") //소리 출력


#include "main.h" // 기본적인 설정 저장해놓은 헤더 불러오기
#include "util.h" // 커서 관련 기능
#include "menu.h" // 메뉴 관련 그리기, 커서 좌표값 등 
#include <conio.h>


#define MAX_SIZE 1024

using namespace std;

SOCKET client_sock;
SOCKADDR_IN client_addr = {};
string my_id, my_nick, my_pw, my_name, my_phonenumber, result;
string login_User_nick, login_User_id; //로그인 정보 
string User_edit_pw; //로그인한 유저 이름 아이디 저장
string edit_check = "N";
string friend_id; // 친구 추가할 친구 아이디
string chatting_friend; // 대화중인 친구
string chatting_roomnum; // 대화중인 방번호

WSADATA wsa;
// Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미..+
// 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
// 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
int code = WSAStartup(MAKEWORD(2, 2), &wsa);

void login(); // 로그인 
bool login_flag = false;
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
void socket_init(); // 소켓정보 저장
void findID(); // 아이디 찾기
void findPW(); // 패스워드 찾기
void join();
bool User_Edit_falg = false;
void chat_list();
void User_Edit(); // 정보 수정 메뉴버튼 8
void friend_register();
void friend_list_recv();
void conversation();
void conversation_recv();

int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (!login_flag) { //이거 설정 필수!!
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            cout << "chat_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            /*cout << " tokens[0] 은 " << tokens[0] << endl;
            cout << " tokens[1] 은 " << tokens[1] << endl;
            cout << " tokens[2] 은 " << tokens[2] << endl;*/


            //msg = buf;
            //std::stringstream ss(msg);  // 문자열을 스트림화
            //string user;
            //ss >> user; // 스트림을 통해, 문자열을 공백 분리해 변수에 할당. 보낸 사람의 이름만 user에 저장됨.
            //if (user != my_nick) cout << buf << endl; // 내가 보낸 게 아닐 경우에만 출력하도록.
            //cout << "씨 아웃 버퍼 : " << buf << endl;;

            //void dm_send_result(int server_request, const string & sender, int variable, const string & recipientUser)
            //서버로부터 오는 메시지 형태
            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (이걸로 로그인 성공 여부) 
            // / [3] : 받는 사람 / [4] 디비 유저 이름 ) / [5] 디비 아이디
            if (tokens[1] != my_nick) {
                if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                    //ss >> result; // 결과
                    if (tokens[0] == "1") {
                        result = tokens[2];
                        if (result == "1") {
                            cout << "==============테스트 정보 출력=============" << endl;
                            cout << "tokens[o] : " << tokens[0] << endl;
                            cout << "로그인 성공" << endl;
                            cout << " tokens[0] 은 " << tokens[0] << endl;
                            cout << " tokens[1] 은 " << tokens[1] << endl;
                            cout << " tokens[2] 은 " << tokens[2] << endl;
                            cout << " tokens[3] 은 " << tokens[3] << endl;
                            cout << " tokens[4] 은 " << tokens[4] << endl;
                            cout << " tokens[5] 은 " << tokens[5] << endl;
                            login_User_nick = tokens[4];
                            login_User_id = tokens[5];
                            cout << " 내 닉네임 저장 => " << login_User_nick << endl;
                            cout << "==============테스트 정보 출력=============" << endl;
                            // 여기에서 결과(result)를 사용하거나 처리
                            cout << login_User_nick << " 님 로그인 되었습니다." << endl;
                            cout << " 4초 뒤에 메인 화면으로 갑니다." << endl;
                            Sleep(10000);
                            login_flag = true; //이걸 해야지 로그인 여부 결정할 수 있음.
                            break;
                        }
                        else {
                            cout << " 로그인 실패" << endl;
                            Sleep(10000);
                            login();
                        }
                    }
                    else {
                        cout << " 로그인 실패 2초대기" << endl;
                        Sleep(10000);
                        login();
                    }
                }
                else {
                    cout << buf << endl;
                }
            }

            if (tokens[1] == login_User_nick) {
                //cout <<" 메세지가 전송되었습니다." << endl;
                //cout << tokens[1] << " 에게 메세지가 전송되었습니다." << endl;
                cout << " " << login_User_nick << " 님 로그인 되었습니다." << endl;
                cout << " 4초 뒤에 메인 화면으로 갑니다." << endl;
                Sleep(4000);
                login_flag = true; //이걸 해야지 로그인 여부 결정할 수 있음.
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
            cout << "findID_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (ID 찾기 성공 여부) / [3] : 받는 사람 / [4] : 결과(찾은 ID) )
            if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                //ss >> result; // 결과
                if (tokens[0] == "2") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << " ※ 아이디 찾기 성공!" << endl;
                        string find_User_id = tokens[4];
                        cout << " " << my_name << " 님의 아이디는 : " << find_User_id << " 입니다." << endl;
                        cout << " 4초 후 메인 화면으로 돌아갑니다." << endl;
                        findID_flag = true;
                        Sleep(10000);
                        break;
                        // 여기에서 결과(result)를 사용하거나 처리
                    }
                    else {
                        findID_flag = false; // ID 찾기 성공 여부
                        cout << " ID 찾기 실패. 입력한 정보를 다시 확인해주세요." << endl;
                        Sleep(10000);
                        break;
                    }
                }
                else {
                    findID_flag = false; // ID 찾기 성공 여부
                    cout << " ID 찾기 실패. 입력한 정보를 다시 확인해주세요." << endl;
                    Sleep(10000);
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
            cout << "findPW_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (ID 찾기 성공 여부) / [3] : 받는 사람 / [4] : 결과(찾은 ID) )
            if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                //ss >> result; // 결과
                if (tokens[0] == "3") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << " ※ 비밀번호 찾기 성공!" << endl;
                        string find_User_pw = tokens[4];
                        cout << my_name << " 님의 비밀번호는 : " << find_User_pw << " 입니다." << endl;
                        cout << " 3초 후 메인 화면으로 돌아갑니다." << endl;
                        findPW_flag = true;
                        Sleep(10000);
                        break;
                        // 여기에서 결과(result)를 사용하거나 처리
                    }
                    else {
                        findPW_flag = false; // ID 찾기 성공 여부
                        cout << " PW 찾기 실패. 입력한 정보를 다시 확인해주세요." << endl;
                        Sleep(10000);
                        break;
                    }
                }
                else {
                    findPW_flag = false; // ID 찾기 성공 여부
                    cout << " PW 찾기 실패. 입력한 정보를 다시 확인해주세요." << endl;
                    Sleep(10000);
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
            cout << "join_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (회원가입 성공 여부) / [3] : 받는 사람 / [4] : 유저 이름 )
            if (tokens[1] == "server") {
                if (tokens[0] == "4") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << " ※ 회원 가입 성공!" << endl;
                        cout << " ※ " << my_name << " 님 환영합니다. " << endl;
                        cout << " 4초 뒤 메인 화면으로 돌아갑니다. " << endl;
                        join_flag = true; // 회원가입 성공 여부
                        Sleep(10000);
                        break;
                    }
                    else if (result == "3") {
                        join_flag = false;
                        cout << " 입력한 아이디가 존재하지 않습니다." << endl;
                        cout << " 회원가입을 진행합니다." << endl;
                        join_id_flag = true;
                        Sleep(10000);
                        break;
                    }
                    else if (result == "4") {
                        cout << " 회원가입 실패! 이미 존재하는 아이디입니다. " << endl;
                        join_flag = false;
                        join_id_flag = false;
                        Sleep(10000);
                        join();
                        break;
                    }
                    else {
                        join_flag = false; // 회원가입 성공 여부
                        cout << " 회원가입 실패! 입력한 정보를 다시 확인해주세요. " << endl;
                        Sleep(10000);
                        join();
                        break;
                    }
                }
                else {
                    join_flag = false; // 회원가입 성공 여부
                    cout << " 회원가입 실패! 입력한 정보를 다시 확인해주세요. " << endl;
                    Sleep(10000);
                    join();
                    break;
                }
            }


        }
    }
}

void login() {
    system("cls");

    while (!login_flag) {

        // 이걸 먼저해야 다시 로그인시도 못함. 
        if (login_flag == true) {
            break;
        }

        string User_input;

        string User_request = "1";
        cout << " 아이디 입력 >> ";
        cin >> User_input;
        my_nick = User_input;
        cout << " 비밀번호 입력 >> ";
        cin >> User_input;
        my_pw = User_input;

        cout << " 아이디는 : " << my_nick << endl;

        while (1) {
            string msg = User_request + " " + my_nick + " " + my_pw;
            send(client_sock, msg.c_str(), msg.length(), 0);
            break;
        }

        std::thread th2(chat_recv);
        //while(1)

        th2.join();
        //closesocket(client_sock); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨

    }

    //WSACleanup(); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨

}

void findID() {
    system("cls");

    while (!code) {

        if (findID_flag == true) {
            break;
        }

        string User_request = "2"; // ID 찾기 번호?
        cout << " 이름 입력 >> ";
        cin >> my_name;

        while (true) {
            cout << " 전화번호 입력(- 포함) >> ";
            cin >> my_phonenumber;

            if (my_phonenumber.length() != 13) {
                cout << " ※ 전화번호를 다시 입력해주세요. (- 포함)" << endl;
                continue;
            }
            break;
        }

        string msg = User_request + " " + my_name + " " + my_phonenumber;
        send(client_sock, msg.c_str(), msg.length(), 0);


        cout << User_request << endl;
        cout << my_name << endl;
        cout << my_phonenumber << endl;

        std::thread th2(findID_recv);
        //if (keyControl() == SUBMIT) { //스페이스바 누르기 전 까지는 이 정보창에 머무릅니다.
        //    login_flag = false;
        //}

        while (1) {
            break;
            string text;
            std::getline(cin, text);
            const char* buffer = text.c_str(); // string형을 char* 타입으로 변환       
            send(client_sock, buffer, strlen(buffer), 0);
        }

        th2.join();
        //closesocket(client_sock); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨

    }

    //WSACleanup(); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨
}

void findPW() {
    system("cls");

    while (!code) {

        if (findPW_flag == true) {
            break;
        }

        string User_request = "3"; // PW 찾기 번호?
        cout << " ID 입력 >> ";
        cin >> my_id;
        cout << " 이름 입력 >> ";
        cin >> my_name;

        while (true) {
            cout << " 전화번호 입력(- 포함) >> ";
            cin >> my_phonenumber;

            if (my_phonenumber.length() != 13) {
                cout << " ※ 전화번호를 다시 입력해주세요. (- 포함)" << endl;
                continue;
            }
            break;
        }

        string msg = User_request + " " + my_id + " " + my_name + " " + my_phonenumber;
        send(client_sock, msg.c_str(), msg.length(), 0);

        std::thread th2(findPW_recv);
        //if (keyControl() == SUBMIT) { //스페이스바 누르기 전 까지는 이 정보창에 머무릅니다.
        //    login_flag = false;
        //}

        th2.join();
        //closesocket(client_sock); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨

    }

    //WSACleanup(); //이거 주석처리 안하면 페이지 전환시 (서버에서) 로그 아웃 처리됨
}

void join() {
    system("cls");


    while (!code) {

        if (join_flag == true) {
            break;
        }

        if (join_id_flag == false) {

            while (1) {
                cout << " 아이디를 입력하세요. (영문 10자리 이하) >> ";
                cin >> my_id;

                if (my_id.length() > 10) {
                    cout << " 입력 가능한 자리수를 넘었습니다. 다시 입력해주세요. ";
                    continue;
                }
                else { break; }
            }

            string User_request = "41"; // 회원가입 전 아이디 확인 요청 번호
            string msg_id = User_request + " " + my_id;
            send(client_sock, msg_id.c_str(), msg_id.length(), 0);
        }

        else if (join_id_flag == true) {
            while (1) {
                cout << " 이름을 입력하세요. >> ";
                cin >> my_name;

                if (my_name.length() > 10) {
                    cout << " 입력 가능한 자리수를 넘었습니다. 다시 입력해주세요. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " 비밀번호를 입력하세요. (10자리 이하) >> ";
                cin >> my_pw;

                if (my_pw.length() > 10) {
                    cout << " 입력 가능한 자리수를 넘었습니다. 다시 입력해주세요. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " 전화번호를 입력하세요. (- 포함) >> ";
                cin >> my_phonenumber;

                if (my_phonenumber.length() > 13) {
                    cout << " 잘못 입력하셨습니다. 다시 입력해주세요. ";
                    continue;
                }
                else { break; }
            }

            while (1) {
                cout << " 닉네임을 입력하세요. (10자리 이하) >> ";
                cin >> my_nick;

                if (my_nick.length() > 10) {
                    cout << " 입력 가능한 자리수를 넘었습니다. 다시 입력해주세요. ";
                    continue;
                }
                else { break; }
            }

            string User_request = "4"; // 회원가입 전 아이디 확인 요청 번호
            string msg = User_request + " " + my_id + " " + my_name + " " + my_pw + " " + my_phonenumber + " " + my_nick; // 친구 목록은 제외?
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
            cout << "edit_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (ID 찾기 성공 여부) / [3] : 받는 사람 / [4] : 결과(찾은 ID) )
            if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                //ss >> result; // 결과
                if (tokens[0] == "8") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << " ============================================ " << endl;
                        cout << " 입력하신 비밀번호가 일치합니다. 2초 후 변경 페이지로 넘어갑니다." << endl;
                        Sleep(10000);
                        system("cls");

                        cout << " ============================================ " << endl;
                        cout << " 변경하실 비밀번호를 입력하세요. ";
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

                        // 여기에서 결과(result)를 사용하거나 처리
                    }

                    else if (result == "2") {
                        cout << " 비밀번호를 잘못 입력하셨습니다. " << endl;

                        Sleep(10000);
                        break;
                        User_Edit();

                    }
                    else if (result == "3") {
                        cout << " 비밀번호가 변경되었습니다." << endl;
                        User_Edit_falg = true;
                        tokens.clear();
                        Sleep(10000);
                        break;

                    }
                    else {

                        cout << " 변경실패." << endl;
                        Sleep(10000);
                        //User_Edit();
                    }
                }
                else {

                    cout << " 변경실패." << endl;
                    Sleep(10000);
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
        //edit_check 의 기본값 N
        string User_input;
        string User_request = "8";

        cout << " ============================================ " << endl;
        cout << " " << login_User_nick << " 님의 비밀번호 변경 페이지" << endl;
        cout << " [보안] 현재 비밀번호를 입력하세요. >> ";
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
            //cout << "list_recv buf = " << buf << endl;
            //void dm_send_db(int server_request, const string& sender, const std::string & recipientUser, const std::string& user_2, const std::vector<std::vector<std::string>>&result)
            // 서버요청 번호 / 보낸사람 : server / 받는사람 : 나 / 같이 대화하는 사람 / 대화 내용
            std::vector<std::string> tokens;
            std::vector<std::string> DB_contents;
            std::stringstream ss(buf);
            std::string token;  

            while (std::getline(ss, token, '/')) {
                // "/"로 분할한 첫 번째 부분은 공백을 기준으로 분리하여 tokens 벡터에 저장
                if (tokens.empty()) {
                    std::stringstream tokens_iss(token);
                    while (tokens_iss >> token) {
                        tokens.push_back(token);
                    }
                }
                else {
                    // "/"로 분할한 나머지 부분은 "\n"을 기준으로 분리하여 저장
                    std::stringstream DB_contents_iss(token);
                    std::string line;
                    while (std::getline(DB_contents_iss, line, '\n')) {
                        DB_contents.push_back(line);
                    }
                }
            }
            
            //cout << tokens[0] << " < 토큰즈[0] 임 " << endl;
            

            if (tokens[0] == "5") {
                std::cout << tokens[3] << "과 대화하고 있습니다." << endl;
                cout << " ============================================ " << endl;

                // second 벡터 값 출력
                for (size_t i = 0; i < DB_contents.size(); ++i) {
                    std::cout << "DB_contents[" << i << "]: " << DB_contents[i] << std::endl;
                }        
                PlaySound(TEXT("katalk.wav"), 0, SND_FILENAME | SND_ASYNC); //일반 재생
                cout << " ============================================ " << endl;
                cout << " 보낼 메세지를 입력하세요. >> ";
            }        
            else if (tokens[0] == "501") {
                break;
            }
            
            //break;
        }
    }
}

void chat_list() {
    system("cls");  

    while (!chat_list_flag) {
        system("cls");
        //cout << "친구 목록을 요청합니다." << endl;        
        string User_request = "5"; //채팅하기 초반부

        while (1) {
            string msg = User_request + " " + login_User_id + " " + chatting_friend + " " + chatting_roomnum;
            send(client_sock, msg.c_str(), msg.length(), 0);
            break;
        }


        std::thread th2(chatlist_recv);
        while (std::cin.get() != '\n'); //채팅하기 들어올때 누른 스페이스바 삭제

        cout << " ============================================ " << endl;       
        while (1) {
            string User_request = "51";
            if (std::cin.peek() != EOF) { //사용자의 입력이 있으면
                string user_msg;
                getline(cin, user_msg); // 사용자 입력을 한 줄로 읽기

                if (user_msg == "exit") {
                    chat_list_flag = true;
                    string User_request = "52";
                    string msg = User_request + " " + login_User_id;
                    send(client_sock, msg.c_str(), msg.length(), 0);
                    break;
                }
                string msg = User_request + " " + login_User_id + " " + user_msg + " " + chatting_roomnum;
                //수정 필요
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

            if (friend_list_flag == false) {
                string User_request = "71";
                string msg = User_request + " " + login_User_id;
                send(client_sock, msg.c_str(), msg.length(), 0);
            }

            if (friend_list_flag == true) {
                while (1) {
                    cout << " 추가할 친구의 아이디를 입력하세요. >> ";
                    cin >> friend_id;

                    // 내 아이디는 추가할 수 없게
                    if (login_User_id == friend_id) {
                        cout << " ※ 친구 추가를 진행할 수 없는 아이디입니다. " << endl;
                        cout << " ※ 아이디를 다시 확인해주세요. " << endl;
                        continue;
                    }
                    else { break; }

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
    system("cls");

    while (!register_flag) {
        char buf[MAX_SIZE] = { };
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            cout << "friend_list_recv buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // 토큰 배열 저장 값 확인용
            for (int i = 0; i < tokens.size(); i++) {
                cout << "tokens[" << i << "]" << tokens[i] << endl;
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (ID 찾기 성공 여부) / [3] : 받는 사람 / [4] : 찾은 친구 리스트(한줄) )
            if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                //ss >> result; // 결과
                if (tokens[0] == "71") {
                    result = tokens[2];
                    if (result == "1" && tokens.size() > 4) { // 배열 비어있는건 empty 함수로 추가 하지 않도록 수정하기
                        cout << " ※ 현재 등록되어 있는 친구 목록입니다. " << endl;
                        cout << " ============================================ " << endl;
                        for (int i = 4; i < tokens.size(); i++) {
                            cout << "  " << i - 3 << ". ID : " << tokens[i] << endl;
                        }
                        cout << " ============================================ " << endl;
                        friend_list_flag = true;
                        Sleep(10000);
                        break;
                    }
                    else if (result == "2" ) {
                        cout << " 현재 등록되어 있는 친구가 없습니다." << endl;
                        cout << " 대화를 원하면 친구를 추가해주세요." << endl;
                        friend_list_flag = true;
                        Sleep(10000);
                        break;
                    }
                }
                else if (tokens[0] == "7") {
                    result = tokens[2];
                    if (result == "1") {
                        cout << " 친구 추가가 완료 되었습니다. " << endl;
                        register_flag = true;
                        Sleep(10000);
                    }
                    else if (result == "2") {
                        cout << " 이미 존재하는 친구입니다." << endl;
                        register_flag = false;
                        Sleep(10000);
                        friend_register();
                    }
                    else if (result == "3") {
                        cout << " 존재하지 않는 사용자입니다." << endl;
                        cout << " ID를 다시 확인해주세요." << endl;
                        register_flag = false;
                        Sleep(10000);
                        friend_register();
                    }
                }
            }
        }
    }
}

void conversation() { //6 친구 목록 가져오기

    while (1) {

        if (conversation_flag == false) {
            system("cls");
            string User_request = "6"; // 대화하기 전 기존 채팅방 있는지 확인
            string msg = User_request + " " + login_User_id;
            send(client_sock, msg.c_str(), msg.length(), 0);
        }

        if (conversation_flag == true && user_check_flag == false) {

            while (1) {
                cout << " ※ 대화를 원하는 사용자의 아이디를 입력하세요. (신규 대화도 가능) >> ";
                cin >> friend_id;

                // 나 자신과 대화 불가능
                if (friend_id == login_User_id) {
                    cout << " ※ 대화를 진행할 수 없는 아이디입니다. " << endl;
                    cout << " ※ 아이디를 다시 확인해주세요. " << endl;
                    continue;
                }
                else { break; }
            }

            string User_request = "61";
            string msg = User_request + " " + login_User_id + " " + friend_id;
            send(client_sock, msg.c_str(), msg.length(), 0);
        }

        if (conversation_flag == true && user_check_flag == true) { //리스트 불러오면 true + 상대 아이디 있으면 true
            break;

        }

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
            cout << "들어온 buf = " << buf << endl;

            // 문자열을 스트림에 넣고 공백을 기준으로 분할하여 벡터에 저장
            std::istringstream iss(buf);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            // ( [0] : 요청 결과 (1=로그인 등) / [1] : 보낸 사람 ( 왠만해선 "server") / [2] : 결과값 (ID 찾기 성공 여부) / [3] : 받는 사람 / [4] : 찾은 친구 리스트(한줄) )
            if (tokens[1] == "server") { // 서버로부터 오는 메시지인 
                //ss >> result; // 결과
                if (tokens[0] == "6") {
                    result = tokens[2];
                    if (result == "1") { // 배열 비어있는건 empty 함수로 추가 하지 않도록 수정하기
                        cout << " ※ 기존 대화를 진행했던 친구 목록입니다. " << endl;
                        cout << " ============================================ " << endl;
                        for (int i = 4; i < tokens.size(); i++) {
                            cout << "  " << i - 3 << ". ID : " << tokens[i] << endl;
                        }
                        cout << " ============================================ " << endl;

                        conversation_flag = true;
                        Sleep(10000);
                        break;
                    }
                    else if (result == "2") {
                        cout << " ※ 기존 대화를 진행했던 친구가 없습니다. " << endl;
                        cout << " ============================================ " << endl;
                        conversation_flag = true;
                        Sleep(10000);
                        break;
                        conversation();
                    }
                    else if (result == "3") {
                        cout << " ※ 아이디 검색 실패! 현재 등록되어 있는 사용자가 아닙니다." << endl;
                        user_check_flag = false;
                        Sleep(10000);
                        break;
                        conversation();
                    }
                    else if (result == "4") {
                        cout << " ※ 아이디 검색 성공! 대화방을 불러옵니다." << endl;
                        //break;
                    }

                }
                else if (tokens[0] == "601") { //정상적으로 다 실행됐을 때 여기로 와야함
                    cout << "클라 # 992 의 " << tokens[3] << endl;
                    chatting_friend = tokens[3];
                    cout << "클라 # 994 의 " << chatting_friend << endl;
                    chatting_roomnum = tokens[4];
                    user_check_flag = true; // 이 부분 필요
                    break;
                }
            }
        }
    }
}

void socket_init() {

    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

    // 연결할 서버 정보 설정 부분
    //SOCKADDR_IN client_addr = {};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(7777);
    InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) { // 위에 설정한 정보에 해당하는 server로 연결!            
            cout << "***SYSTEM MESSAGE : Server Connect***" << endl;
            string msg = "auto produce";
            send(client_sock, msg.c_str(), msg.length(), 0); // 연결에 성공하면 client 가 입력한 닉네임을 서버로 전송
            //send(client_sock, my_pw.c_str(), my_pw.length(), 0);          

            break;
        }
        cout << "Connecting..." << endl;
    }
}

int main()
{
    init(); //커서 깜빡거리는거 삭제해주는 함수
    socket_init();
    while (1) {
        //std::thread th2(chat_recv);
        if (login_flag == false) {
            findID_flag = false;
            findPW_flag = false;
            join_id_flag = false;
            join_flag = false;
            MainMenu(); // 메인 메뉴 그리기 생성자 호출
            int menuCode = MenuDraw(); // 게임시작 버튼 생성 및 y좌표 값 저장
            //printf("메뉴 코드는 %d ", menuCode); <<로 y좌표 확인 가능
            // 1:로그인 / 2:id찾기 / 3:pw찾기 / 4:회원가입 / 5:대화 / 6:기존채팅 / 7:친추 / 8:비번수정
            if (menuCode == 0) {
                login(); // 로그인 함수 실행
            }
            else if (menuCode == 1) { // 1 아이디 찾기
                findID();
            }
            else if (menuCode == 2) { // 2 비밀번호 찾기
                findPW();
            }
            else if (menuCode == 3) { // 3 회원 가입
                join();
            }
            else if (menuCode == 4) { // 4 회원 가입
                cout << "\n\n\n";
                WSACleanup();
                return 0; // 종료
            }
            system("cls"); // 콘솔창을 클린 하란 의미
        }
        //로그인 성공했을 때만 트루로 바꿔줬으므로, 로그인 됐을 때만 아이디가 출력됨.
        else if (login_flag == true) {
            edit_check = "N";
            User_Edit_falg = false;
            dblist_flag = false;
            register_flag = false;
            friend_list_flag = false;
            chat_list_flag = false;
            conversation_flag = false;
            user_check_flag = false;
            MainMenu(); // 메인 메뉴 그리기 생성자 호출

            cout << endl << "\t    ";cout << "로그인 성공! " << login_User_nick << " 님 환영합니다." << endl;
            //cout << "주석 처리 필수! 확인용! " << login_User_id << " 님 환영합니다." << endl;
            //cout << edit_check << " 에이디트트 체크 " << User_Edit_falg << "플래기 " << endl;
            int menuCode = Login_MenuDraw();

            if (menuCode == 0) { // 5 대화하기
                system("cls");
                conversation(); // 친구 목록 가져오기
                chat_list();
            }
            else if (menuCode == 1) { // 6 기존 대화방 불러오기
                conversation();
                chat_list();
            }
            else if (menuCode == 2) { // 7 친구 추가
                friend_register();
            }
            else if (menuCode == 3) { // 8 비밀 번호 수정
                User_Edit();
            }
            else if (menuCode == 4) {  // 종료 하기
                cout << "\n\n\n";
                WSACleanup();
                
                return 0; // 종료
            }
            system("cls"); // 콘솔창을 클린 하란 의미
        }
        system("cls"); // 콘솔창을 클린 하란 의미
    }
}
