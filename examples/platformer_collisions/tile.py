"""
This file is a part of the 'Pygame Examples (pgex)' source code.
The source code is distributed under the MIT license.
"""

import spark


class Tile:
    def __init__(self, rect):
        self.rect = rect

    def collision(self, player_rect) -> bool:
        return spark.shapes.collide_rect(self.rect, player_rect)

    def draw(self):
        spark.render_rect(self.rect[0], self.rect[1], self.rect[2])
        #pygame.draw.rect(display, self.color, self.rect)
