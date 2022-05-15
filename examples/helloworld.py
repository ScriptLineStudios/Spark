import spark

spark.init("Hello World Demo", 800, 800, False) 
                                               
rect = spark.create_rect(1, 0, 0)

x = 0
y = 0

while spark.not_window_close():
    spark.clear((0,0,0))

    spark.render_rect(rect, (0, 0), (255, 0, 0), 200)

    spark.update()                                                                                                                            