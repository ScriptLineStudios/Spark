import math
from _framework.images import alphabet
import spark

def animate(image_list, animation_index, time_to_show_image_on_screen):
    if animation_index+1 >= len(image_list)*time_to_show_image_on_screen:
        animation_index = 0
    animation_index += 1

    return animation_index


def load_map(map_name):
    blocks = []
    with open(map_name, "r") as a_file:
        for line in a_file:
            stripped_line = line.strip()
            stripped_line = stripped_line.split(" ")
            if stripped_line[2] != "block3":
                blocks.append(stripped_line)
        a_file.close()

    return blocks


def render_tiles(scroll, tiles, player_pos, tile_index):
    tile_rects = []
    for _ in tiles:

        tile_rects.append([int(_[0]), int(_[1])-350, 16, 16, _[2]])
        x = int(_[0])-scroll[0]
        y = int(_[1])-scroll[1]-350
        dist = math.hypot(player_pos[0]-x, player_pos[1]-y)
        if dist < 300:
            try:
                spark.render_image(tile_index[_[2]], (x, y), (16, 16))
            except:
                pass

    return tile_rects

def render_text(display, word, pos, scroll):
      for index, letter in enumerate(word):
            img = alphabet[letter]
            if letter == " ":
                spark.render_image(img, ((pos[0]+index*32)-scroll[0], pos[1]-scroll[1]), (16, 16))
            else:
                spark.render_image(img, ((pos[0]+index*32)-scroll[0], pos[1]-scroll[1]), (16, 16))

print("Framework loaded!")