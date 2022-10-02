import spark

spark.init("Demo", 500, 500, False)

rect1 = [(0.0, 0.0), (1.0, 0.0, 0.0), (50.0, 50.0)]
rect2 = [[0.5, 0.0], (1.0, 1.0, 0.0), (55.0, 50.0)]

tiles = [
    [(0.0, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.1, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.2, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.3, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
    [(0.4, -0.6), (1.0, 0.0, 0.0), (50.0, 50.0)],
]

while spark.not_window_close():
    spark.clear((0, 0, 0))
    if spark.key.key_is_pressed(spark.key.D):
        rect2[0][0] += 0.006
    if spark.key.key_is_pressed(spark.key.A):
        rect2[0][0] -= 0.006

    if spark.key.key_is_pressed(spark.key.W):
        rect2[0][1] += 0.006
    if spark.key.key_is_pressed(spark.key.S):
        rect2[0][1] -= 0.006

    spark.shapes.render_rect(rect1[0], rect1[1], rect1[2])
    spark.shapes.render_rect(rect2[0], rect2[1], rect2[2])

    #or tile in tiles:
     #   spark.shapes.render_rect(tile[0], tile[1], tile[2])

    print(spark.shapes.collide_rect(rect1, rect2))
    #if spark.shapes.collide_rect(rect1, rect2):
     #   rect2[0][0] = rect1[0][0] + (rect2[2][0] / 500) * 2

    spark.set_title(str(spark.get_fps()))
    spark.update()
