import spark

spark.init("Hello World Demo", 800, 800, False)

texture = spark.create_rect("shaders/player_walk2.png")
texture1 = spark.create_rect("shaders/BOSS.png")

x = 0
size = 0

while spark.not_window_close():
    size += 1
    spark.clear((0, 0, 0))
    spark.set_title(str(spark.get_fps()))

    if spark.key_is_pressed(65):
        x -= 0.05
    if spark.key_is_pressed(68):
        x += 0.05

    spark.render_rect(texture1, (0.4, 0), (255, 0, 0), 200)
    spark.render_rect(texture, (x, 0), (255, 0, 0), size)

    spark.update()
