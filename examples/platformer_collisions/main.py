"""
This file is a part of the 'Pygame Examples (pgex)' source code.
The source code is distributed under the MIT license.
"""

import asyncio
from typing import List

import spark 

import common
from player import Player
from tile import Tile


class Game:
    def __init__(self) -> None:
        spark.init("Platformer Collisions Example", 600, 600, False)

        self.key_presses = {"a": False, "d": False}
        self.player = Player(x=0.0, y=0.2, width=20, height=20, color=(255, 0, 0))

        self.running = True

    def render_map(self, tiles: List[Tile]) -> None:
        """
        Renders the games tiles
        """

        for tile in tiles:
            spark.shapes.render_rect(tile.rect[0], tile.rect[1], tile.rect[2])
            #pygame.draw.rect(self.display, (60, 255, 100), tile.rect)

    async def main(self) -> None:
        """
        Game main function. Handles key presses, map and player rendering
        """
        while self.running:
            spark.clear((0, 0, 0))

            if spark.key.key_is_pressed(spark.key.SPACE):
                if self.player.is_on_ground:
                    self.player.y_velocity -= self.player.JUMP_HEIGHT

            self.key_presses["a"] = spark.key.key_is_pressed(spark.key.A)
            self.key_presses["d"] = spark.key.key_is_pressed(spark.key.D)

            self.player.handle_movement(self.key_presses)
            self.player.draw()

            self.render_map(common.tiles)

            spark.update()
            await asyncio.sleep(0)

    def run(self) -> None:
        """
        Run the main loop; using asyncio for WASM compatibility
        """

        asyncio.run(self.main())

Game().run()
