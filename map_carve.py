import random

width, height = 640, 480

map_grid = [['#' for _ in range(width)] for _ in range(height)]

map_grid[18][50] = ' '

def add_room(x, y, w, h):
    for i in range(y, min(y+h, height-1)):
        for j in range(x, min(x+w, width-1)):
            map_grid[i][j] = ' '

num_rooms = 200
for _ in range(num_rooms):
    rw, rh = random.randint(10, 20), random.randint(10, 20)
    rx, ry = random.randint(1, width-2-rw), random.randint(1, height-2-rh)
    if not (rx <= 50 < rx+rw and ry <= 18 < ry+rh):
        add_room(rx, ry, rw, rh)

for _ in range(num_rooms * 2):
    x1, y1 = random.randint(1, width-2), random.randint(1, height-2)
    x2, y2 = random.randint(1, width-2), random.randint(1, height-2)
    if map_grid[y1][x1] == ' ' and map_grid[y2][x2] == ' ':
        for x in range(min(x1,x2), max(x1,x2)+1):
            map_grid[y1][x] = ' '
        for y in range(min(y1,y2), max(y1,y2)+1):
            map_grid[y][x2] = ' '

file_name = "assets/data/maze.txt"
with open(file_name, "w") as f:
    for row in map_grid:
        f.write("".join(row) + "\n")