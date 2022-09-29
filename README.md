# Spark
Spark - An easy to use python graphics library that allows for easy interaction with OpenGL. Perfect for making games!

```python
import spark

spark.init("Hello World This is a test", 1200, 600, False) 

rect_texture = spark.create_rect(1.0, 1.0, 0.0)

while spark.not_window_close():
    spark.clear((0,0,0))
    spark.render_rect(rect_texture, (0, 0), (255, 0, 0), 200)
    spark.update()       
```

# Installation

```pip install git+https://github.com/ScriptLineStudios/Spark/```




