import spark
import math

with open("shaders/fragmentShaderExample.glsl") as f:
    fragment_src = f.read() 
with open("shaders/vertexShaderExample.glsl") as f:
    vertex_src = f.read() 
spark.init("Hello World Demo", 500, 500, False)

shader = spark.load_shader(vertex_src, fragment_src)
shader2 = spark.load_shader(vertex_src, fragment_src)

texture = spark.create_image("shaders/BOSS.png")
texture2 = spark.create_image("shaders/player_walk2.png")

time = 0

x = 0
while spark.not_window_close():
    time += .1
    spark.clear((0, 0, 0))

    spark.send_image(shader, "imageTexture", texture)
    spark.send_float(shader, "tx", time)
    spark.send_float(shader, "ty", time)

    spark.shapes.render_rect((0, 0), (1, 0, 0), 400)

    spark.set_title(str(spark.get_fps()))
    spark.update()
