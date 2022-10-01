# Spark
Spark - An easy to use python graphics library that allows for easy interaction with OpenGL. Perfect for making games!

```python
import spark

spark.init("Demo", 500, 500, False)

while spark.not_window_close():
    spark.clear((0, 0, 0))
    spark.shapes.render_rect((0, 0), (1, 0, 0), 400)
    spark.update()   
```

# Features

- Creating a window
- Loading and rendering images
- Rendering rectanges (more shapes to come)
- Apply optional shaders to rectanges
- Rectangle based collisions

# Installation

```
pip install git+https://github.com/ScriptLineStudios/Spark/
```




