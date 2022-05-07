import spark

spark.init("Hello World This is a test", 1200, 600) 

while spark.not_window_close():
    spark.clear()
    spark.render_triangle()
    spark.update()