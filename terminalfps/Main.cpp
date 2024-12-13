#include <iostream>
#include <cmath>
#include <Windows.h>

#define M_PI 3.14159265358979323846

struct vec2 {
	float x;
	float y;
};

float to_radians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

int map[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
};

int FOV = 60;
const int t_width = 80;
const int t_height = 24;
float ray_angle_step = to_radians((float)FOV) / t_width;
float p_angle = 90.0f;
float p_angle_rad = to_radians(p_angle);
vec2 p_pos = { 4.0f, 4.0f };

void cast_thee(char* screen) {
	for (int i = 0; i < t_width; i++) {
		float r_angle = p_angle_rad - to_radians((float)FOV / 2) + (i * ray_angle_step);
		vec2 r_dir = { cos(r_angle), sin(r_angle) };

		float dis = 0.0f;
		bool hit = false;

		while (!hit && dis < 20.0f) {
			dis += 0.1f;
			int map_x = (int)(p_pos.x + r_dir.x * dis);
			int map_y = (int)(p_pos.y + r_dir.y * dis);
			
			if (map_x < 0 || map_x >= 8 || map_y < 0 || map_y >= 8 || map[map_y][map_x] == 1) {
				hit = true;
			}
		}

		int w_height = (int)(t_height / dis);

		for (int y = 0; y < t_height; y++) {
			if (y < t_height / 2 - w_height / 2 || y > t_height / 2 + w_height / 2)
				screen[y * t_width + i] = ' ';
			else
				screen[y * t_width + i] = '#';
		}
	}
}

void render(char* screen) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;

	for (int y = 0; y < t_height; y++) {
		for (int x = 0; x < t_width; x++) {
			coord.X = x;
			coord.Y = y;
			SetConsoleCursorPosition(hConsole, coord);
			std::cout << screen[y * t_width + x];
		}
	}
}

int main() {
	ShowCursor(FALSE);

	char screen[t_width * t_height];

	while (true) {
		cast_thee(screen);
		render(screen);

		if (GetAsyncKeyState(VK_LEFT)) {
			p_angle -= 2.0f;
			if (p_angle < 0) p_angle += 360.0f;
			p_angle_rad = to_radians(p_angle);
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			p_angle += 2.0f;
			if (p_angle >= 360.0f) p_angle -= 360.0f;
			p_angle_rad = to_radians(p_angle);
		}
		if (GetAsyncKeyState(VK_UP)) {
			p_pos.x += cos(p_angle_rad) * 0.1f;
			p_pos.y += sin(p_angle_rad) * 0.1f;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			p_pos.x -= cos(p_angle_rad) * 0.1f;
			p_pos.y -= sin(p_angle_rad) * 0.1f;
		}
		
		Sleep(25);
	}

	return 0;
}
