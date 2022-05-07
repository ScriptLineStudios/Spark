import spark

spark.init("Hello World This is a test", 1200, 600) 

x = 0.0
y = 0.5
size = 0.1
while spark.not_window_close():
    spark.clear()
    spark.render_triangle(x, y, size)
    spark.update()