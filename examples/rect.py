import spark
import math

spark.init("Demo", 500, 500, False)

rect1 = [[0, 0], (1.0, 0.0, 0.0), (50.0, 50.0)]
rect2 = [[0.0, 0.0], (1.0, 1.0, 0.0), [50.0, 50.0]]

tiles = [
    [(16, -0.6), (1.0, 0.0, 0.0), (16.0, 16.0)],
    [(32, -0.6), (1.0, 0.0, 0.0), (16.0, 16.0)],
    [(48, -0.6), (1.0, 0.0, 0.0), (16.0, 16.0)],
    [(64, -0.6), (1.0, 0.0, 0.0), (16.0, 16.0)],
    [(80, -0.6), (1.0, 0.0, 0.0), (16.0, 16.0)],
]

img = spark.create_image("shaders/BOSS.png")

moving_left = False
moving_right = False

scroll = [0, 0]

time =0
while spark.not_window_close():
    moving_left = False
    moving_right = False
    time += .1
    spark.clear((0, 0, 0))
    if spark.key.key_is_pressed(spark.key.D):
        rect2[0][0] += 5
    if spark.key.key_is_pressed(spark.key.A):
        rect2[0][0] -= 5

    if spark.key.key_is_pressed(spark.key.W):
        rect2[0][1] += 5
    if spark.key.key_is_pressed(spark.key.S):
        rect2[0][1] -= 5

    spark.render_image(img, rect2[0], rect2[2]) 

    print(rect2)
    
    spark.update()
