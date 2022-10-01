import spark

spark.init("Demo", 500, 500, False)

rect1 = [(0.0, 0.0), (1.0, 0.0, 0.0), (50.0, 50.0)]
rect2 = [[0.0, 0.0], (1.0, 1.0, 0.0), (50.0, 50.0)]

while spark.not_window_close():
    spark.clear((0, 0, 0))
    if spark.key.key_is_pressed(spark.key.D):
        rect2[0][0] += 0.01
    if spark.key.key_is_pressed(spark.key.A):
        rect2[0][0] -= 0.01

    if spark.key.key_is_pressed(spark.key.W):
        rect2[0][1] += 0.01
    if spark.key.key_is_pressed(spark.key.S):
        rect2[0][1] -= 0.01

    spark.shapes.render_rect(rect1[0], rect1[1], rect1[2])
    spark.shapes.render_rect(rect2[0], rect2[1], rect2[2])

    print(spark.shapes.collide_rect(rect1, rect2))

    spark.update()
