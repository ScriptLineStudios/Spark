import spark
spark.init("Tiles", 600, 600, False)

game_map = [['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','2','2','2','2','2','0','0','0','0','0','0','0'],
            ['0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'],
            ['2','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2','2'],
            ['1','1','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'],
            ['1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1']]

tile_img = spark.create_image("block1.png")
_y = 0

player_rect = [[0, -120], [1, 1, 1], [16, 16]]

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
    '''for tile in hit_list:
        if movement[0] > 0:
            rect[0][0] = tile[0][0] + tile[2][0]
            collision_types['right'] = True
        elif movement[0] < 0:
            rect[0][0] = tile[0][0] - rect[2][0] 
            collision_types['left'] = True'''

    '''rect[0][1] += movement[1]
    hit_list = collision_test(rect, tiles)
    for tile in hit_list:
        if movement[1] > 0:
            rect[0][1] = tile[0][1] - (rect[2][1] / 400) * 2
            collision_types['bottom'] = True
        elif movement[1] < 0:
            rect[0][1] = tile[0][1] + (tile[2][1] / 400) * 2
            collision_types['top'] = True'''
    return rect, collision_types

y_vel = 0

while spark.not_window_close():
    spark.clear((0, 0, 0))

    tile_rects = []
    y = 80
    for row in game_map:
        x = -300
        for tile in row:
            if tile == "1":
                spark.render_image(tile_img, (x, y), (32, 32))
            if tile != "0":
                tile_rects.append([[x, y], [0, 0, 0], [32, 32]])
            
            x += 32
        y -= 32

    player_movement = [0, 0]
    if spark.key.key_is_pressed(spark.key.D):
        player_movement[0] += 3
    if spark.key.key_is_pressed(spark.key.A):
        player_movement[0] -= 3

    player_movement[1] += y_vel
    y_vel += 0.2
    if y_vel > 3:
        y_vel = 3

    for tile in tile_rects:
        print(spark.shapes.collide_rect(player_rect, tile))
    #player_rect, collisions = move(player_rect, player_movement, tile_rects)
    spark.shapes.render_rect(player_rect[0], player_rect[1], player_rect[2])

    spark.update()