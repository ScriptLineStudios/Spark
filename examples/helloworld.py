import spark

spark.init("Hello World Demo", 800, 800, False)                                                

x = 0

index = 0

vertexShader = spark.load_shader("/home/scriptline/Spark/examples/shaders/vertexShaderExample.glsl")
fragmentShader = spark.load_shader("/home/scriptline/Spark/examples/shaders/fragmentShaderExample.glsl")

texture2 = spark.create_rect("examples/grass.png", vertexShader, fragmentShader)

while spark.not_window_close():
    spark.clear((0,0,0))

    if spark.key_is_pressed(65):
        x -= 0.05
    if spark.key_is_pressed(68):
        x += 0.05

    spark.render_rect(texture2, (x, 0), (255, 0, 0), 100)
    spark.set_title(str(spark.get_fps()))

    spark.update()                                                                                                                            
