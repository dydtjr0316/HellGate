#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>
#include <iostream>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace chrono;

int hole_x[] = { 3,12,24,32,49,51,65,77,83,94,5,6,7,8,9,10,11,12,13,14,15 };
int hole_y[] = { 3,12,24,32,49,51,65,77,83,94,5,6,7,8,9,10,11,12,13,14,15 };



#include "../../../../HellGate/Be_Anyone/Server/Server/Protocol.h"

sf::TcpSocket g_socket;

constexpr auto SCREEN_WIDTH = 20;
constexpr auto SCREEN_HEIGHT = 20;

constexpr auto TILE_WIDTH = 65;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH / 2 + 10;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH / 2 + 10;
constexpr auto BUF_SIZE = 200;
//constexpr auto MAX_USER = 10;

// 추후 확장용.
int NPC_ID_START = 1000;

bool message = false;
bool attacked = false;
int messagetime = 0;
int attack_m_id = 0;

int g_left_x;
int g_top_y;
int g_myid;

sf::RenderWindow* g_window;
sf::Font g_font;

class OBJECT {
private:
	bool m_showing;
	sf::Sprite m_sprite;

	char m_mess[MAX_STR_LEN];
	high_resolution_clock::time_point m_time_out;
	sf::Text m_text;
	sf::Text m_name;

public:
	int m_x, m_y;
	short hp;
	short level;
	int   exp;

	int	  iMax_exp;
	short	  sHp_Regen;
	short Attack_Damage;

	char name[MAX_ID_LEN];
	OBJECT(sf::Texture& t, int x, int y, int x2, int y2) {
		m_showing = false;
		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
		m_time_out = high_resolution_clock::now();
	}
	OBJECT() {
		m_showing = false;
		m_time_out = high_resolution_clock::now();
	}
	void show()
	{
		m_showing = true;
	}
	void hide()
	{
		m_showing = false;
	}

	void a_move(int x, int y) {
		m_sprite.setPosition((float)x, (float)y);
	}

	void a_draw() {
		g_window->draw(m_sprite);
	}

	void move(int x, int y) {
		m_x = x;
		m_y = y;
	}
	void draw() {
		if (false == m_showing) return;
		float rx = (m_x - g_left_x) * 65.0f + 8;
		float ry = (m_y - g_top_y) * 65.0f + 8;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);
		m_name.setPosition(rx - 10, ry - 10);
		g_window->draw(m_name);
		if (high_resolution_clock::now() < m_time_out) {
			m_text.setPosition(rx - 10, ry + 20);
			g_window->draw(m_text);
		}
	}
	void State_draw() {
		if (false == m_showing) return;
		float rx = (m_x - g_left_x) * 65.0f + 8;
		float ry = (m_y - g_top_y) * 65.0f + 8;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);
		m_name.setPosition(rx - 10, ry - 10);
		g_window->draw(m_name);
		if (high_resolution_clock::now() < m_time_out) {
			m_text.setPosition(rx - 10, ry + 20);
			g_window->draw(m_text);
		}
	}
	void set_name(char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setStyle(sf::Text::Bold);
	}
	void add_chat(char chat[]) {
		m_text.setFont(g_font);
		m_text.setString(chat);
		m_text.setFillColor(sf::Color(255, 0, 0));	// 글자 색 변경 (지금 검정색, 교수님은 핑크색 했음) 핑크 : 255, 0, 255
		m_text.setStyle(sf::Text::Bold);
		m_time_out = high_resolution_clock::now() + 1s;
	}
};

OBJECT avatar;
unordered_map <int, OBJECT> npcs;

OBJECT white_tile;
OBJECT black_tile;
OBJECT water_tile;
OBJECT hole_tile;


sf::Texture* board;
sf::Texture* pieces;

void client_initialize()
{
	board = new sf::Texture;
	pieces = new sf::Texture;
	if (false == g_font.loadFromFile("cour.ttf")) {
		cout << "Font Loading Error!\n";
		while (true);
	}
	board->loadFromFile("map.png");
	pieces->loadFromFile("among.png");

	white_tile = OBJECT{ *board, 0, 0, TILE_WIDTH, TILE_WIDTH };
	black_tile = OBJECT{ *board, 65, 0, TILE_WIDTH, TILE_WIDTH };
	hole_tile = OBJECT{ *board, 65 *2, 0, TILE_WIDTH, TILE_WIDTH };
	water_tile = OBJECT{ *board, 65 *3, 0, TILE_WIDTH, TILE_WIDTH };
	
	avatar = OBJECT{ *pieces, 64, 0, 64, 64 };
	avatar.move(4, 4);
}

void client_finish()
{
	delete board;
	delete pieces;
}

void ProcessPacket(char* ptr)
{

	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		g_myid = my_packet->id;
		avatar.move(my_packet->x, my_packet->y);
		g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
		g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
		avatar.show();

		npcs[g_myid].hp = my_packet->hp;
		npcs[g_myid].level = my_packet->level;
		npcs[g_myid].exp = my_packet->exp;
			 
		npcs[g_myid].iMax_exp = my_packet->iMax_exp;

		npcs[g_myid].Attack_Damage = my_packet->Attack_Damage;


	}
	break;

	case SC_PACKET_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			//g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
			//g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
			avatar.show();
		}
		else {
			if (id < MAX_USER)
				npcs[id] = OBJECT{ *pieces, 64 * 2, 0, 64, 64 };
			
			else if ((id >= START_MONSTER && id < END_MONSTER))
			{
		
				for (int i = 0; i < 4; ++i)
				{
					//cout << "ID -> " << id << endl;
					if (id < START_MONSTER + DIVIDE_MONNSTER * (i+1)
						&& id>= START_MONSTER + DIVIDE_MONNSTER * i
						)
						npcs[id] = OBJECT{ *pieces, 64*i, 64, 64, 64 };
				}
			}
			else if (id >= START_NPC && id < END_NPC)
				npcs[id] = OBJECT{ *pieces, 0, 0, 64, 64 };

			strcpy_s(npcs[id].name, my_packet->name);
			npcs[id].set_name(my_packet->name);
			npcs[id].move(my_packet->x, my_packet->y);
			npcs[id].show();
		}
	}
	break;
	case SC_PACKET_MOVE:
	{ 
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
			g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
		}
		else {
			if (0 != npcs.count(other_id))
				npcs[other_id].move(my_packet->x, my_packet->y);
		}
	}
	break;

	case SC_PACKET_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.hide();
		}
		else {
			if (0 != npcs.count(other_id))
				npcs[other_id].hide();
		}
	}
	break;

	case SC_PACKET_CHAT:
	{
		sc_packet_chat* my_packet = reinterpret_cast<sc_packet_chat*>(ptr);


		char* temp = reinterpret_cast<char*>(my_packet->message);


		npcs[my_packet->id].add_chat(temp);
	}
		break;

	case SC_PACKET_ATTACK:
	{
		//cout << "어택 패킷 받음" << endl;

		sc_packet_attack* p = reinterpret_cast<sc_packet_attack*>(ptr);
		if (p->id == g_myid)
		{
			npcs[p->id].hp = p->hp;
			attacked = true;
		}
		else {
			if (0 != npcs.count(p->id))
			{
				npcs[p->id].hp = p->hp;
				attacked = false;
			}
		}
		attack_m_id = p->id;
		message = true;

	}
	break;
	case SC_PACKET_LEVEL_UP:
	{
 		//cout << "레벨업 패킷 받음" << endl;
		sc_packet_level_up* p = reinterpret_cast<sc_packet_level_up*>(ptr);
		if (p->id == g_myid)
		{
			npcs[p->id].hp = p->hp;
			npcs[p->id].level = p->level;
			npcs[p->id].exp = p->exp;
			npcs[p->id].iMax_exp = p->max_exp;
			npcs[p->id].Attack_Damage = p->attack_damage;
		}
		else {
			if (0 != npcs.count(p->id))
			{
				npcs[p->id].hp = p->hp;
				npcs[p->id].level = p->level;
				npcs[p->id].exp = p->exp;
				npcs[p->id].iMax_exp = p->max_exp;
				npcs[p->id].Attack_Damage = p->attack_damage;
			}
		}
	}
	break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);

	}
}

void process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUF_SIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

void client_main()
{
	srand((unsigned int)time(NULL));
	char net_buf[BUF_SIZE];
	size_t	received;

	auto recv_result = g_socket.receive(net_buf, BUF_SIZE, received);
	if (recv_result == sf::Socket::Error)
	{
		wcout << L"Recv 에러!";
		while (true);
	}

	if (recv_result == sf::Socket::Disconnected)
	{
		wcout << L"서버 접속 종료.";
		g_window->close();
	}

	if (recv_result != sf::Socket::NotReady)
		if (received > 0) process_data(net_buf, received);

	for (int i = 0; i < SCREEN_WIDTH; ++i) {
		int tile_x = i + g_left_x;
		if (tile_x >= WORLD_WIDTH) break;
		if (tile_x < 0) continue;
		for (int j = 0; j < SCREEN_HEIGHT; ++j)
		{
			int tile_y = j + g_top_y;
			if (tile_y < 0) continue;
			if (tile_y >= WORLD_HEIGHT) break;

			if (((tile_x / 3 + tile_y / 3) % 2) == 0) {
				white_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				white_tile.a_draw();
			}
			else if(((tile_x / 3 + tile_y / 3) % 2) == 1)
			{
				black_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				black_tile.a_draw();
			}

			if (tile_x % 8 == 0||tile_y%8==0)
			{
				water_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				water_tile.a_draw();
			}

			if (tile_x % 8 == 3 && (tile_y % 6 == 1))
			{
				hole_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				hole_tile.a_draw();
			}
			if ((tile_x % 16 == 14) && (tile_y % 4 == 3))
			{
				hole_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				hole_tile.a_draw();
			}
		}
	}


		

	avatar.draw();
	//	for (auto &pl : players) pl.draw();
	for (auto& npc : npcs) npc.second.draw();

	sf::Text text;
	text.setFont(g_font);
	char buf[100];
	sprintf_s(buf, "LEVEL : %d   HP : %d   EXP : %d\nPOS : (%d, %d)", npcs[g_myid].level, npcs[g_myid].hp, npcs[g_myid].exp, avatar.m_x, avatar.m_y);
	text.setString(buf);
	text.setCharacterSize(65);
	text.setStyle(sf::Text::Bold||sf::Text::Underlined);
	//text.setPosition(100, 100);
	text.setFillColor(sf::Color::Magenta);
	g_window->draw(text);

	if (message) {
		sf::Text text1;
		text1.setFont(g_font);
		char buf1[100];
		if (attacked)
		{
			sprintf_s(buf1, "Be Attacked", attack_m_id);
			text1.setString(buf1);
			text1.setCharacterSize(80);
			text1.setStyle(sf::Text::Bold || sf::Text::Underlined);
			text1.setPosition(400, 400);
			text1.setFillColor(sf::Color::Red);
			g_window->draw(text1);
		}
		else
		{
			sprintf_s(buf1, "Player attack Monster %d.", attack_m_id);
			text1.setString(buf1);
			text1.setCharacterSize(30);
			text1.setStyle(sf::Text::Bold || sf::Text::Underlined);
			text1.setPosition(npcs[g_myid].m_x, npcs[g_myid].m_y+10);
			text1.setFillColor(sf::Color::Magenta);
			g_window->draw(text1);
		}

		/*text1.setString(buf1);
		text1.setCharacterSize(80);
		text1.setStyle(sf::Text::Bold || sf::Text::Underlined);
		text1.setPosition(250, 250);
		text1.setFillColor(sf::Color::Magenta);
		g_window->draw(text1);*/

		messagetime++;

		if (messagetime > 1000)
		{
			message = false;
			messagetime = 0;
		}
	}
}

void send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	size_t sent;
	g_socket.send(p, p[0], sent);
}

void send_move_packet(unsigned char dir)
{
	cs_packet_move m_packet;
	m_packet.type = CS_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	send_packet(&m_packet);
}
void send_attck_packet()
{
	cs_packet_attack m_packet;
	m_packet.type = CS_ATTACK;
	m_packet.size = sizeof(m_packet);
	send_packet(&m_packet);
}

int main()
{
	wcout.imbue(locale("korean"));
	sf::Socket::Status status = g_socket.connect("127.0.0.1", SERVER_PORT);
	g_socket.setBlocking(false);

	if (status != sf::Socket::Done) {
		wcout << L"서버와 연결할 수 없습니다.\n";
		while (true);
	}

	client_initialize();



	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = CS_LOGIN;
	int t_id = GetCurrentProcessId();
	char tempName[MAX_ID_LEN];
	string tempstring;
	cout << "nick name : ";
	cin >> tempName;
	sprintf_s(l_packet.name, tempName);
	strcpy_s(avatar.name, l_packet.name);
	avatar.set_name(l_packet.name);
	send_packet(&l_packet);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH , WINDOW_HEIGHT), "2D CLIENT");
	g_window = &window;

	sf::View view = g_window->getView();
	view.zoom(2.0f);
	view.move(SCREEN_WIDTH * TILE_WIDTH / 4, SCREEN_HEIGHT * TILE_WIDTH / 4);
	g_window->setView(view);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				int p_type = -1;
				switch (event.key.code) {
				case sf::Keyboard::Left:
					send_move_packet(MV_LEFT);
					break;
				case sf::Keyboard::Right:
					send_move_packet(MV_RIGHT);
					break;
				case sf::Keyboard::Up:
					send_move_packet(MV_UP);
					break;
				case sf::Keyboard::Down:
					send_move_packet(MV_DOWN);
					break;
				case sf::Keyboard::A:
					send_attck_packet();
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
			}
		}

		window.clear();
		client_main();



		window.display();
	}
	client_finish();

	return 0;
}