import spark
spark.init("Planyt", 900, 700, False)
import time
from _framework.images import *
from _framework.constants import *
from _framework.framework import *
from _framework.player import Player
from _framework.bullet import Bullet
import random
import numpy as np
import sys
import os


player = Player(0, 0.2, 2)
prev_time = time.time()
scroll = [0, 0]
BLOCK_DICT = {
    "grass_left": block_left,
    "grass_right": block_right,
    "grass": grass_block,
    "grass_side_left": block_side_left,
    "grass_side_right": block_side_right,
    "dirt": dirt_block,
    "torch": torch,
    "enemy": foliage1,
    "foliage2": foliage2
}

health = 5
grass = []
bullets = []
torches = []
enemies = []
decorations = []

dead = False
has_played_death_animation = False

special_tiles = [["torch", torches], ["foliage2", decorations]]

enemy_count = 0

levels = ["tutorial", "map", "map1", "map2", "map3", "map4", "map5", "map6", "map7", "end"]
level_index = 0
tiles = load_map(f"assets/map/{levels[level_index]}.txt")

for tile in tiles:
      for t in special_tiles:
            if t[0] == tile[2]:
                  t[1].append(tile)
                  tiles.remove(tile)

      if tile[2] == "enemy":
            enemy_count += 1
            enemies.append([tile[0], tile[1], tile[2], random.randrange(0, 30)])
            tiles.remove(tile)

bg_sqaures = []

small_squares = []
global_time = 0

leaves = []

screen_shake = 0

kills = 0

explosion_effects = []

enemy_bullets = []
enemy_bullet_pattern = [[0, 1], [0, -1], [1, 0], [-1, 0], [.5, .5], [.5, -.5], [-.5, .5], [-.5, -.5]]

enemy_bullet_cooldown = 0

def glow(surf, host, pos, radius, offset=0):
    if host:
        timing_offset = (hash(host) / 1000) % 1
        timing_offset += 1
    else:
        timing_offset = 0
    glow_width = abs(math.sin(global_time+offset)*25) + radius *2

    glow_img = light_mask_base
    spark.render_image(glow_img, (pos[0]-glow_width/2, pos[1]-glow_width/2), (glow_width, glow_width))

torch_effects = []
particles = []
explosions = []
spark.set_title("Planyt")
radius = 5

while spark.not_window_close():
      mx, my = spark.mouse_pos()
      scroll[0] += (player.player_rect[0][0]-scroll[0]-(0.5)+mx/50)/10
      scroll[1] += (player.player_rect[0][1]-scroll[1]-(0.5)+my/50)/10
      spark.clear((77/255, 77/255, 126/255))
      dt = 1/60

      global_time += dt

      mouse_x, mouse_y = spark.mouse_pos()

      rand_event = random.randrange(0, 20)
      if rand_event == 3:
            for i in range(1):
                  bg_sqaures.append([random.randrange(0, 800), 400, 0])
                  
      for square in bg_sqaures:
            square[1] -= 1
            square[2] += 1
            if square[1] < -100:
                  bg_sqaures.remove(square)
            ##### come back to this! spark.render_image(spark.create_image("assets/images/square.png"), (square[0]-scroll[0], square[1]-scroll[1]), (10, 10))
      
      for t in torches:
            #display.blit(torch, (int(t[0])-scroll[0], int(t[1])-scroll[1]-350))
            event = random.randrange(0,5)
            if event == 3:
                  torch_effects.append([int(t[0])+random.randrange(0, 10), int(t[1])-345, global_time*random.random(), .2, 3, random.randrange(0, 100)])

      if spark.key.key_is_pressed(spark.key.SPACE):
            if player.air_timer < 6:
                  player.vertical_momentum = -4

      if spark.key.mouse_is_pressed(spark.key.LEFT_MOUSE):
            rel_x, rel_y = mouse_x - (player.player_rect[0][0]-scroll[0]), mouse_y - (player.player_rect[0][1]-scroll[1])
            bullets.append(Bullet(player.player_rect[0][0]-scroll[0]+0.025, player.player_rect[0][1]-scroll[1]+0.025, mouse_x, mouse_y, rel_x, rel_y))

      for b in range(len(bullets)):
            try:
                  bullet = bullets[b]
                  for tile in tiles:
                        #if spark.colliderect([(bullet.x, bullet,y), (0, 0, 0), (0.025, 0.025)], [(bullet.x, bullet,y), (0, 0, 0), (0.025, 0.025)])
                        if pygame.Rect(bullet.x, bullet.y, 4, 4).colliderect(pygame.Rect(int(tile[0])-scroll[0], int(tile[1])-scroll[1]-350, 16, 16)):
                              for i in range(15):
                                    particles.append([int(tile[0]), int(tile[1])-350, random.randrange(-15, 15), random.randrange(-3*5, 3*5), random.randrange(0, 360), 100])
                              screen_shake += 4

                              bullets.remove(bullet)

                  for enemy in enemies:
                        rect = pygame.Rect(int(enemy[0])-scroll[0], int(enemy[1])-scroll[1]-350, 16, 16)
                        if pygame.Rect(bullet.x, bullet.y, 4, 4).colliderect(rect):
                              for i in range(100):
                                    explosions.append([int(enemy[0]), int(enemy[1])-350+random.randrange(-7, 7), random.randrange(-4, 4),random.randrange(-2, 7), 1, (242, 211, 171), False, .2, 100])
                              explosion_sound.play()
                              for i in range(15):
                                    particles.append([int(tile[0]), int(tile[1])-350, random.randrange(-15, 15), random.randrange(-3*5, 3*5), random.randrange(0, 360), 1])
                              screen_shake += 8
                              kills += 1
                              enemies.remove(enemy)
                              bullets.remove(bullet)

            except:
                  pass
            bullet.main(display)

      spark.update()