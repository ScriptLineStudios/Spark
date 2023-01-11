import spark


with open("shaders/fragmentShaderExample.glsl") as f:
    fragment_src = f.read() 
with open("shaders/vertexShaderExample.glsl") as f:
    vertex_src = f.read() 


spark.init("Hello World Demo", 800, 800, False)

shader = spark.load_shader(vertex_src, fragment_src)
texture = spark.create_image("shaders/player_walk2.png")
texture1 = spark.create_image("shaders/BOSS.png")

x = 0
size = 0

while spark.not_window_close():
    size += 1
    spark.clear((0, 0, 0))
    spark.set_title(str(spark.get_fps()))

    if spark.key.key_is_pressed(spark.key.A):
        x -= 1
    if spark.key.key_is_pressed(spark.key.D):
        x += 1

    spark.render_image(texture1, (0.4, 0), (200, 200))
    spark.render_image(texture, (x, 0), (72, 72))

    spark.update()
