import spark

spark.init("Hello World Demo", 1200, 800, False) 
                                               
rect_texture = spark.create_rect(1.0, 1.0, 0.0)


x = 0
y = 0

while spark.not_window_close():
    spark.clear((0,0,0))

    if spark.key_is_pressed(65):
        x -= 0.01 
    if spark.key_is_pressed(68):
        x += 0.01 


    spark.render_rect(rect_texture, (x, y), (255, 0, 0), 200)
    spark.update()                                                                                                                            