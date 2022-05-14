import spark

spark.init("Hello World Demo", 1200, 600, False) 

rect_texture = spark.create_rect(1.0, 1.0, 0.0)

while spark.not_window_close():
    spark.clear((0,0,0))
    spark.render_rect(rect_texture, (0.1, 0.1), (255, 0, 0), 0.1)
    spark.update()
