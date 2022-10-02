"""
This file is a part of the 'Pygame Examples (pgex)' source code.
The source code is distributed under the MIT license.
"""

from typing import List

import spark 

import common
from tile import Tile


class Player:
    SPEED = 0.001
    JUMP_HEIGHT = 0.07

    def __init__(self, x, y, width, height, color) -> None:
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.color = color

        self.y_velocity = 0.03
        self.is_on_ground = False
        self.rect = [[self.x, self.y], self.color, [self.width, self.height]]

    def get_colliding_tiles(
        self, tiles: List[Tile], player_rect
    ) -> List[Tile]:
        """
        Returns a list of tiles the player is currently colliding with
        """
        return_tiles = []
        for tile in tiles:
            if tile.collision(player_rect):
                return_tiles.append(tile)

        return tiles

    def calculate_rect(
        self, movement: dict, player_rect, map_tiles: List[Tile]
    ):
        """
        Calculates the Rect of the player based on their movement and the surrounding tiles
        """
        player_rect[0][0] += movement["horizontal"]
        tiles = self.get_colliding_tiles(map_tiles, player_rect)
        for tile in tiles:
            print(tile.collision(player_rect))
            if tile.collision(player_rect):
                pass
                # if movement["horizontal"] > 0:
                #     print(player_rect[0][0], tile.rect[0][0])
                #     player_rect[0][0] = tile.rect[0][0] 
                # if movement["horizontal"] < 0:
                #     player_rect[0][0] = tile.rect[0][0] 

        '''self.is_on_ground = False
        player_rect[0][1] -= movement["vertical"]
        tiles = self.get_colliding_tiles(map_tiles, player_rect)
        for tile in tiles:
            if tile.collision(player_rect):
                if movement["vertical"] > 0:
                    player_rect[0][1] = tile.rect[0][1] + (tile.rect[2][1] / 600) * 2

                    self.is_on_ground = True
                    self.y_velocity = 0.03
                if movement["vertical"] < 0:
                    player_rect[0][1] = tile.rect[0][1] - (tile.rect[2][1] / 600) * 2.5'''

        return player_rect

    def handle_movement(self, key_presses: dict):
        """
        Handles all code relating to the movement of the player
        """

        player_movement = {"horizontal": 0, "vertical": self.y_velocity}

        if key_presses["a"]:
            player_movement["horizontal"] -= self.SPEED
        if key_presses["d"]:
            player_movement["horizontal"] += self.SPEED

        if self.y_velocity < 0.5:
            self.y_velocity += 0.002
        self.rect = self.calculate_rect(player_movement, self.rect, common.tiles)

    def draw(self) -> None:
        """
        Draws the player at the rect position
        """
        spark.shapes.render_rect(self.rect[0], self.rect[1], self.rect[2])
