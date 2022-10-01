import pygame

pygame.init()

display = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()

img = pygame.image.load("shaders/player_walk2.png")

rect = pygame.Rect(10, 10, 100, 100)
rect1 = pygame.Rect(400, 400, 800, 800)


while True:
    display.fill((0, 0, 0))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            break

    rect1.x += pygame.key.get_pressed()[pygame.K_d]
    rect1.x -= pygame.key.get_pressed()[pygame.K_a]

    rect1.y += pygame.key.get_pressed()[pygame.K_s]
    rect1.y -= pygame.key.get_pressed()[pygame.K_w]

    #pygame.draw.rect(display, (100, 0, 0), rect)
    pygame.draw.rect(display, (0, 100, 0), rect1)

    if (rect.x < rect1.x + rect1.w and
        rect.x + rect.w > rect1.x and
        rect.y < rect1.y + rect1.h and
        rect.h + rect.y > rect1.y):
            print("yay")
    else:
        print("no")


    pygame.display.set_caption(f"{clock.get_fps()}")
    pygame.display.update()
    clock.tick(60)
