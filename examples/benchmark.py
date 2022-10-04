import pygame
import spark
import matplotlib.pyplot as plt

class SparkEnv:
    def __init__(self):
        spark.init("Demo", 600, 600, False)
        self.image = spark.create_image("shaders/BOSS.png")
        self.image2 = spark.create_image("shaders/player_walk2.png")

    def render_batch(self, size):
        framerate = 0
        for i in range(100):

            spark.clear((0, 0, 0))

            for j in range(size):
                spark.render_image(self.image, (0, 0), (32, 32))
                spark.render_image(self.image2, (64, 0), (32, 32))

            framerate += spark.get_fps()
            spark.update()

        return framerate / 100
    
class PygameEnv:
    def __init__(self):
        pygame.init()
        self.display = pygame.display.set_mode((600, 600))
        self.clock = pygame.time.Clock()

        self.image = pygame.image.load("shaders/BOSS.png").convert()
        self.image2 = pygame.image.load("shaders/player_walk2.png").convert()

    def render_batch(self, size):
        framerate = 0
        for i in range(100):
            self.display.fill((0, 0, 0))

            for j in range(size):
                self.display.blit(pygame.transform.scale(self.image, (32, 32)), (0, 0))
                self.display.blit(pygame.transform.scale(self.image2, (32, 32)), (64, 0))

            framerate += self.clock.get_fps()

            self.clock.tick(60)
            pygame.display.update()

        return framerate / 100

pg_env = PygameEnv()
spark_env = SparkEnv()

x_axis = []

pg_fps = []
spark_fps = []

for size in range(2000, 14000, 1):
    spark_fps.append(spark_env.render_batch(0))
    pg_fps.append(pg_env.render_batch(0))
    print(f"SPARK = {spark_fps[-1]} PYGAME = {pg_fps[-1]}")

    x_axis.append(size)

    plt.plot(x_axis, pg_fps, label = "Pygame", color="green")
    plt.plot(x_axis, spark_fps, label = "Spark", color="black")

    plt.pause(0.0001) #Note this correction
plt.show()

