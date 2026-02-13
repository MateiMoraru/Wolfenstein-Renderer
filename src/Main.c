#include <stdio.h>
#include <math.h>

#include "Window.h"
#include "RayCaster.h"
#include "Player.h"
#include "Renderer.h"

#define WIDTH 2500
#define HEIGHT 1400

#define MAP_WIDTH 640
#define MAP_HEIGHT 480

#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0f)

char** map_init()
{
    char** map = malloc(MAP_HEIGHT * sizeof(char*));
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        map[y] = malloc(MAP_WIDTH * sizeof(char));
        if (!map[y])
        {
            printf("Failed to malloc map[%d]\n", y);
        }
    }

    if (!map)
    {
        printf("Failed to malloc map\n");
        return NULL;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map[y][x] = ' ';
        }
    }

    return map;
}

void map_put(char** map, int x, int y, char c)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return;
    map[y][x] = c;
}


void map_put_rect(char** map, int x, int y, int w, int h, char c)
{
    for (int dy = y; dy < y + h; dy++)
    {
        for (int dx = x; dx < x + w; dx++)
        {
            map_put(map, dx, dy, c);
        }
    }
}

void draw_map(char** map, Window* window, int scale)
{
    window_set_draw_color(window, 0x538BBDFF);
    SDL_Rect rect = {0, 0, scale, scale};
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            rect.x = x * scale;
            rect.y = y * scale;

            if (map[y][x] == ' ')
            {
                continue;
            }
            else if (map[y][x] == '#')
            {
                window_set_draw_color(window, 0x538BBDFF);
            }
            else if (map[y][x] == 'p')
            {
                window_set_draw_color(window, 0x9c2458FF);
            }

            window_draw_rect(window, &rect);
        }
    }
    window_set_draw_color(window, 0x696d70FF);
}

void handle_mouse(Mouse* mouse)
{
    int x, y;
    mouse->button = SDL_GetMouseState(&x, &y);

    mouse->dx = x - mouse->x;

    mouse->x = x;
    mouse->y = y;
}

void handle_keys(char** map, Player* player, RayCaster* ray_caster, float delta_time)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    float move = player->speed * delta_time;
    float forward = 0;
    float strafe = 0;

    if (keys[SDL_SCANCODE_W]) forward += move;
    if (keys[SDL_SCANCODE_S]) forward -= move;
    if (keys[SDL_SCANCODE_A]) strafe -= move;
    if (keys[SDL_SCANCODE_D]) strafe += move;

    if (forward != 0 || strafe != 0)
        player_move(map, player, ray_caster, forward, strafe);
}


void map_load(char** map, const char* map_load_file)
{
    FILE* fin = fopen(map_load_file, "r");
    if (!fin)
    {
        printf("Failed to open map file\n");
        return;
    }

    char buffer[1024];
    int y = 0;

    while (fgets(buffer, sizeof(buffer), fin) && y < MAP_HEIGHT)
    {
        int x = 0;
        while (buffer[x] != '\0' && buffer[x] != '\n' && x < MAP_WIDTH)
        {
            map[y][x] = buffer[x];
            x++;
        }

        while (x < MAP_WIDTH)
        {
            map[y][x] = ' ';
            x++;
        }

        y++;
    }

    fclose(fin);
    
}

void map_save(char** map, const char* filename)
{
    FILE* fout = fopen(filename, "w");
    if (!fout)
    {
        printf("Failed to open write file\n");
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fputc(map[y][x], fout);
        }

        fputc('\n', fout);
    }

    fclose(fout);
}

void map_draw_mouse(char** map, Mouse* mouse)
{
    if (mouse->button & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        printf("DSA\n");
        int x = mouse->x / SCALE;
        int y = mouse->y / SCALE;
        map[y][x] = '#';
    }
}

int main()
{   
    Window* window = window_create(-1, -1, "Domm");

    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_CaptureMouse(SDL_FALSE);
    SDL_SetWindowGrab(window->window, SDL_FALSE);
    SDL_ShowCursor(SDL_TRUE);   

    const char* map_load_file = "assets/data/map.txt";

    char** map;

    map = map_init();

    map_load(map, map_load_file);

    Player player = {
        .x = 50, 
        .y = 14,
        .speed = 8, 
        .direction = 240, 
        .fov = 60
    };

    RayCaster ray_caster;
    
    ray_caster_init(player.x, player.y, player.direction, player.fov, &ray_caster);

    player_set_position(map, &player, &ray_caster, 50, 18);
    
    ray_caster_set_position(&ray_caster, player.x, player.y);

    Mouse mouse = {0};

    mouse.x = WIDTH / 2;
    mouse.y = HEIGHT / 2;

    // SDL_WarpMouseInWindow(window->window, mouse.x, mouse.y);

    Renderer* renderer = renderer_init(&player, &ray_caster, window);

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time; 
    float delta_time = 0;

    while(window->running)
    {
        current_time = SDL_GetTicks();
        delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        while(window_poll_event(window))
        {

            if (window->event.type == WINDOW_QUIT)
            {
                window_quit(window);
            }

            if (window_get_key(window) == SDLK_ESCAPE)
            {
                window_quit(window);
            }
            else if (window_get_key(window) == SDLK_1)
            {
                    map_save(map, map_load_file);
            }

        }

        handle_keys(map, &player, &ray_caster, delta_time);
        ray_caster_set_position(&ray_caster, player.x, player.y);

        handle_mouse(&mouse);
        player.direction += mouse.dx * SENSITIVITY;
        
        if (player.direction < 0) player.direction += 360;
        if (player.direction >= 360) player.direction -= 360;

        //map_draw_mouse(map, &mouse);
        
        ray_caster_rotate(&ray_caster, mouse.dx);

        renderer_update(renderer);

        for (int i = 0; i < NUMBER_RAYS; i++) {
            renderer->ray_caster->rays[i].len = ray_hits_wall(map, &renderer->ray_caster->rays[i]);
        }

        window_set_draw_color(window, 0x696d70FF);
        window_clear(window);

        renderer_draw(renderer, window);

        //draw_map(map, window, SCALE);

        //draw_rays(window, &ray_caster.rays, map);        

        window_show(window);
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        free(map[y]);
    }

    free(map);

    window_terminate(window);
    return 0;
}