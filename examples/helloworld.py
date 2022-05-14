import spark

spark.init("Window", 1200, 600) 

rect_texture = spark.create_rect(1.0, 1.0, 0.0)

while spark.not_window_close():
    spark.clear((0,0,0))
    spark.render_rect(0.1, 0.1, 0.1, rect_texture)
    spark.update()
