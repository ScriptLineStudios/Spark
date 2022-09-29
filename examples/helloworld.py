import spark

spark.init("Hello World Demo", 800, 800, False) 
                                               
texture = spark.create_rect("examples/player_walk2.png")
x = 0

while spark.not_window_close():
    spark.clear((0,0,0))

    if spark.key_is_pressed(65):
        x -= 0.05
    if spark.key_is_pressed(68):
        x += 0.05

    spark.render_rect(texture, (x, 0), (255, 0, 0), 200)

    spark.update()       