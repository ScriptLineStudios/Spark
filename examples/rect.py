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

    #spark.shapes.render_rect(rect1[0], rect1[1], rect1[2])
#    spark.shapes.render_rect(rect2[0], rect2[1], rect2[2])   
    #spark.shapes.render_rect(rect1[0], rect1[1], rect1[2])   
    spark.render_image(img, rect2[0], rect2[2]) 

    for tile in tiles:
        spark.shapes.render_rect(tile[0], tile[1], tile[2])   
        if spark.shapes.collide_rect(tile, rect2):
            print(time)

    if spark.shapes.collide_rect(rect1, rect2):
        pass
        #rect2[0][0] = rect1[0][0] + rect1[2][0]
        #rect2[0][0] = rect1[0][0] - rect2[2][0] 

        #rect2[0][1] = rect1[0][1] - rect2[2][1]
        #rect2[0][1] = rect1[0][1] + rect1[2][1]

    

    spark.set_title(str(spark.get_fps()))
    spark.update()
