import spark
import os
from _framework.constants import *

def load_img(path, color=(255,255,255)):
    img = spark.create_image(os.path.join("assets", "images", f"{path}.png"))
    print(img)
    return img

dirt_block = load_img("block0")
print("___")
grass_block = load_img("block1")
block_left = load_img("block1_left")
block_right = load_img("block1_right")
block_side_left = load_img("block_side_left")
block_side_right = load_img("block_side_right")
player_run_imgs = [load_img("player_walk1"), load_img("player_walk2"), load_img("player_walk3"),
load_img("player_walk4"), load_img("player_walk5")]
player_idle_imgs = [load_img("player_idle1"), load_img("player_idle2")]
player_run_names = ["assets/images/player_walk_1.png", "assets/images/player_walk_2.png", "assets/images/player_walk_3.png"]
player_idle_names = ["assets/images/player_idle1.png", "assets/images/player_idle2.png"]
attack_img = load_img("attack")
inventory_img = load_img("inventory")
cloud_img = load_img("cloud")
staff_img = load_img("staff")
light_mask_base = load_img('light')
torch = load_img('torch')
foliage1 = load_img("foliage1")
foliage2 = load_img("foliage2")
spark_img = load_img("spark")
bullet_img = load_img("bullet")
enemy_bullet_img = load_img("enemy_bullet")
heart_img = load_img("heart")
alphabet = {
    "a": load_img("a", (0,0,0)),
    "b": load_img("b", (0,0,0)),
    "c": load_img("c", (0,0,0)),
    "d": load_img("d", (0,0,0)),
    "e": load_img("e", (0,0,0)),
    "f": load_img("f", (0,0,0)),
    "g": load_img("g", (0,0,0)),
    "h": load_img("h", (0,0,0)),
    "i": load_img("i", (0,0,0)),
    "j": load_img("j", (0,0,0)),
    "k": load_img("k", (0,0,0)),
    "l": load_img("l", (0,0,0)),
    "m": load_img("m", (0,0,0)),
    "n": load_img("n", (0,0,0)),
    "o": load_img("o", (0,0,0)),
    "p": load_img("p", (0,0,0)),
    "q": load_img("q", (0,0,0)),
    "r": load_img("r", (0,0,0)),
    "s": load_img("s", (0,0,0)),
    "t": load_img("t", (0,0,0)),
    "u": load_img("u", (0,0,0)),
    "v": load_img("v", (0,0,0)),
    "w": load_img("w", (0,0,0)),
    "x": load_img("x", (0,0,0)),
    "y": load_img("y", (0,0,0)),
    "z": load_img("z", (0,0,0)),
    " ": load_img("space", (0,0,0)),
    "0": load_img("0", (0,0,0)),
    "1": load_img("1", (0,0,0)),
    "2": load_img("2", (0,0,0)),
    "3": load_img("3", (0,0,0)),
    "4": load_img("4", (0,0,0)),
    "5": load_img("5", (0,0,0)),
    "6": load_img("6", (0,0,0)),
    "7": load_img("7", (0,0,0)),
    "8": load_img("8", (0,0,0)),
    "9": load_img("9", (0,0,0)),
    "/": load_img("tick", (0,0,0)),

}
print("Images loaded!")
