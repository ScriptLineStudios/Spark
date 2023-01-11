import spark

with open("shaders/fragmentShaderExample.glsl") as f:
    fragment_src = f.read() 
with open("shaders/vertexShaderExample.glsl") as f:
    vertex_src = f.read() 

spark.init("Hello World Demo", 800, 800, False)
shader = spark.load_shader(vertex_src, fragment_src)
img = spark.create_image("block1.png")
time =0
while spark.not_window_close():
    time += .1
    spark.clear((0, 0, 0))
    
    spark.send_float(shader, "tx", time/10)
    spark.send_float(shader, "ty", time/10)

    spark.render_image(img, (0, 0), (256, 256), shader) 
    spark.update()
