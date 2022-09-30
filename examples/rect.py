import spark
import math

with open("shaders/fragmentShaderExample.glsl") as f:
    fragment_src = f.read() 
with open("shaders/vertexShaderExample.glsl") as f:
    vertex_src = f.read() 

spark.init("Hello World Demo", 500, 500, False)

shader = spark.load_shader(vertex_src, fragment_src)
texture = spark.create_image("shaders/BOSS.png")

time = 0

x = 0
while spark.not_window_close():
    time += .1
    spark.clear((0, 0, 0))

    if spark.key.key_is_pressed(spark.key.A):
        x -= 0.05
    if spark.key.key_is_pressed(spark.key.D):
        x += 0.05

    spark.send_shader(shader, "time", time/10)
    spark.shapes.render_rect(shader, (0, 0), (1, 0.5, 1), 400)

    spark.set_title(str(spark.get_fps()))
    spark.update()
