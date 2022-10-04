import spark
spark.init("Spark Tiles Demo", 600, 600, False)

game_map = [['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','2','2','2','2','2','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','2','1','1','1','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','2','1','1','1','1','0','0','0','0','0','0','0','2','2'],
            ['2','2','2','2','2','1','1','1','1','1','2','2','2','2','2','2','2','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1']]

tile_img = spark.create_image("block1.png") 
dirt_img = spark.create_image("block0.png")
player_img = spark.create_image("player_idle1.png")

player_rect = [[0, 120], [1, 1, 1], [64, 64]]

player_movement = [0, 0]

def collision_test(rect, tiles):
    hit_list = []
    for tile in tiles:
        if spark.shapes.collide_rect(rect, tile):
            hit_list.append(tile)
    return hit_list

def move(rect, movement, tiles):
    collision_types = {'top': False, 'bottom': False, 'right': False, 'left': False}
    rect[0][0] += movement[0]
    hit_list = collision_test(rect, tiles)
    for tile in hit_list:
        if movement[0] > 0:
            rect[0][0] = tile[0][0] - rect[2][0] 
            collision_types['right'] = True
        elif movement[0] < 0:
            rect[0][0] = tile[0][0] + tile[2][0]
            collision_types['left'] = True

    rect[0][1] -= movement[1]
    hit_list = collision_test(rect, tiles)
    for tile in hit_list:
        if movement[1] > 0:
            rect[0][1] = tile[0][1] + tile[2][1]
            collision_types['bottom'] = True
        elif movement[1] < 0:
            rect[0][1] = tile[0][1] - rect[2][1]
            collision_types['top'] = True
    return rect, collision_types

y_vel = 0
air_timer = 0

while spark.not_window_close():
    spark.clear((77/255, 77/255, 126/255))

    tile_rects = []
    y = 200
    for row in game_map:
        x = -300
        for tile in row:
            if tile == "2":
                spark.render_image(tile_img, (x, y), (64, 64))
                #spark.shapes.render_rect((x, y), (1, 0, 0), (32, 32))

            if tile == "1":
                spark.render_image(dirt_img, (x, y), (64, 64))

            if tile != "0":
                tile_rects.append([[x, y], [0, 0, 0], [64, 64]])
            
            x += 64
        y -= 64

    player_movement = [0, 0]
    if spark.key.key_is_pressed(spark.key.D):
        player_movement[0] += 3
    if spark.key.key_is_pressed(spark.key.A):
        player_movement[0] -= 3

    player_movement[1] += y_vel
    y_vel += 1
    if y_vel > 10:
        y_vel = 8

    if spark.key.key_is_pressed(spark.key.SPACE):
        if air_timer < 6:
            y_vel = -20

    player_rect, collisions = move(player_rect, player_movement, tile_rects)
    if collisions['bottom'] == True:
        air_timer = 0
        y_vel = 0
    else:
        air_timer += 1

    spark.set_title(str(spark.get_fps()))
    spark.render_image(player_img, player_rect[0], player_rect[2])
    spark.update()