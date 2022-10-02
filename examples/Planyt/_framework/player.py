import spark
from _framework.framework import animate
from _framework.entity import Entity
from _framework.images import *

class Player(Entity):
    def __init__(self, x, y, movement_speed):
        super().__init__(x, y)
        self.player_rect = [(self.x, self.y), (1.0, 0.0, 0.0), (72, 72)]
        self.player_movement = [0,0]
        self.moving_left = False
        self.moving_right = False
        self.movement_speed = movement_speed
        self.scroll = [0,0]
        self.vertical_momentum = 0
        self.air_timer = 0
        self.current_animation = 0
        self.animations = [player_idle_imgs, player_run_imgs]
        self.animation_names = [player_idle_names, player_run_names]
        self.animation_index = 0

    def get_input(self):
        self.player_movement = [0,0]
        self.air_timer += 1
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:
            self.player_movement[0] -= self.movement_speed
            self.moving_left = True
            self.moving_right = False
        elif keys[pygame.K_d]:
            self.player_movement[0] += self.movement_speed
            self.moving_left = False
            self.moving_right = True

        if not keys[pygame.K_d] and not keys[pygame.K_a]:
            self.current_animation = 0
            self.moving_left = False
            self.moving_right = False
        else:
            self.current_animation = 1

        self.player_movement[1] += self.vertical_momentum
        self.vertical_momentum += 0.1
        if self.vertical_momentum > 3:
            self.vertical_momentum = 3

    def check_collisions(self, tile_rects, scroll):
        hit_list = []
        for tile in tile_rects:
            if pygame.Rect(self.player_rect.x-scroll[0], self.player_rect.y-scroll[1], self.player_rect.width, self.player_rect.height).colliderect(pygame.Rect(tile[0]-scroll[0], tile[1]-scroll[1], tile[2], tile[3])):
                hit_list.append(pygame.Rect(tile[0], tile[1], 16, 16))
        return hit_list

    def move(self, tile_rects, scroll):
        collisions = {"top": False,
                    "bottom": False, 
                    "left": False,
                    "right": False}
        self.player_rect[0][0] += self.player_movement[0]
        hit_list = self.check_collisions(tile_rects, scroll)
        for tile in hit_list:
            if self.player_movement[0] > 0:
                self.player_rect.right = tile.left
                collisions["right"] = True
            elif self.player_movement[0] < 0:
                self.player_rect.left = tile.right
                collisions["left"] = True


        self.player_rect[0][1] += self.player_movement[1]
        hit_list = self.check_collisions(tile_rects, scroll)
        for tile in hit_list:
            if self.player_movement[1] > 0:
                self.player_rect.bottom = tile.top 
                collisions["bottom"] = True
                self.rotation = 0
                self.jumping = False
            elif self.player_movement[1] < 0:
                self.player_rect.top = tile.bottom
                collisions["top"] = True

        return self.player_rect, collisions


    def draw(self, scroll):
        self.animation_index = animate(self.animations[self.current_animation], self.animation_index, 10)
        if self.moving_left:
            spark.render_image(self.animations[self.current_animation][self.animation_index//10], (self.player_rect[0][0]-scroll[0], self.player_rect[0][1]-scroll[1]+1), (72, 72))
            #display.blit(pygame.transform.flip(self.animations[self.current_animation][self.animation_index//10], True, False), (self.player_rect.x-scroll[0], self.player_rect.y-scroll[1]+1))
        elif self.moving_right:
            spark.render_image(self.animations[self.current_animation][self.animation_index//10], (self.player_rect[0][0]-scroll[0], self.player_rect[0][1]-scroll[1]+1), (72, 72))
            #display.blit(self.animations[self.current_animation][self.animation_index//10], (self.player_rect.x-scroll[0], self.player_rect.y-scroll[1]+1))
        else:
            spark.render_image(self.animations[self.current_animation][self.animation_index//10], (self.player_rect[0][0]-scroll[0], self.player_rect[0][1]-scroll[1]+1), (72, 72))
            #display.blit(self.animations[self.current_animation][self.animation_index//10], (self.player_rect.x-scroll[0], self.player_rect.y-scroll[1]+1))
        
    def main(self, tile_rects, scroll):
        self.get_input()
        self.rect, collisions = self.move(tile_rects, scroll)
        if collisions['bottom'] == True:
            self.air_timer = 0
        if collisions['top'] == True:
            self.vertical_momentum = 0

        self.draw(scroll)

