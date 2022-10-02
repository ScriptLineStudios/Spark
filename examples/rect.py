import spark
import math

spark.init("Demo", 500, 500, False)

rect1 = [[0, 0], (1.0, 0.0, 0.0), (50.0, 50.0)]
rect2 = [[-0.5, 0.0], (1.0, 1.0, 0.0), [50.0, 50.0]]

tiles = [
    [(0.0, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.1, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.2, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.3, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.4, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
]

moving_left = False
moving_right = False

time =0
while spark.not_window_close():
    moving_left = False
    moving_right = False
    time += .1
    spark.clear((0, 0, 0))
    if spark.key.key_is_pressed(spark.key.D):
        moving_right = True
        rect2[0][0] += 3
    if spark.key.key_is_pressed(spark.key.A):
        moving_left = True

        rect2[0][0] -= 3

    if spark.key.key_is_pressed(spark.key.W):
        rect2[0][1] += 3
    if spark.key.key_is_pressed(spark.key.S):
        rect2[0][1] -= 3

    spark.shapes.render_rect(rect1[0], rect1[1], rect1[2])
    spark.shapes.render_rect(rect2[0], rect2[1], rect2[2])    

    #print(spark.shapes.collide_rect(rect1, rect2))
    if spark.shapes.collide_rect(rect1, rect2):
        pass
        #rect2[0][0] = rect1[0][0] + rect1[2][0]
        #rect2[0][0] = rect1[0][0] - rect2[2][0] 

        #rect2[0][1] = rect1[0][1] - rect2[2][1]
        #rect2[0][1] = rect1[0][1] + rect1[2][1]

    

    spark.set_title(str(spark.get_fps()))
    spark.update()
