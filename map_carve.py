import random

width, height = 640, 480

# Initialize the grid with walls
map_grid = [['#' for _ in range(width)] for _ in range(height)]

# Make (50,18) always blank (player start)
map_grid[18][50] = ' '

# Function to add a rectangular room
def add_room(x, y, w, h):
    for i in range(y, min(y+h, height-1)):
        for j in range(x, min(x+w, width-1)):
            map_grid[i][j] = ' '

# Generate multiple random rooms
num_rooms = 200  # increased for more rooms
for _ in range(num_rooms):
    rw, rh = random.randint(20, 60), random.randint(20, 60)  # room size
    rx, ry = random.randint(1, width-2-rw), random.randint(1, height-2-rh)
    
    # Avoid overwriting the starting point
    if not (rx <= 50 < rx+rw and ry <= 18 < ry+rh):
        add_room(rx, ry, rw, rh)

# Connect rooms with corridors
for _ in range(num_rooms * 2):
    x1, y1 = random.randint(1, width-2), random.randint(1, height-2)
    x2, y2 = random.randint(1, width-2), random.randint(1, height-2)
    if map_grid[y1][x1] == ' ' and map_grid[y2][x2] == ' ':
        # Horizontal corridor
        for x in range(min(x1,x2), max(x1,x2)+1):
            map_grid[y1][x] = ' '
        # Vertical corridor
        for y in range(min(y1,y2), max(y1,y2)+1):
            map_grid[y][x2] = ' '

with open(file, "w") as f:
    for row in map_grid:
        file.write("".join(row) + "\n")

print("Maze generated successfully!")