import spark
import math
from _framework.images import bullet_img

class Bullet:
    def __init__(self, x, y, mouse_x, mouse_y, rel_x, rel_y):
        self.x = x
        self.y = y
        self.mouse_x = mouse_x
        self.mouse_y = mouse_y
        self.speed = 10
        self.angle = math.atan2(y-mouse_y, x-mouse_x)
        self.x_vel = math.cos(self.angle) * self.speed
        self.y_vel = math.sin(self.angle) * self.speed

        self.angle_rot = ((180 / math.pi) * -math.atan2(rel_y, rel_x))
    def main(self, display):
        self.x -= int(self.x_vel)
        self.y -= int(self.y_vel)

        spark.render_image(bullet_img, (self.x, self.y), (32, 32))
        #display.blit(pygame.transform.rotate(bullet_img, self.angle_rot), (self.x, self.y))