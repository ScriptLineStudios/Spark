import spark

spark.init("Hello World Demo", 1200, 800, False) 

rect_texture = spark.create_rect(1.0, 1.0, 0.0)


while spark.not_window_close():
    spark.clear((0,0,0))
    spark.render_rect(rect_texture, (0.0, 0.0), (255, 0, 0), 200)
    spark.update()
