import pygame

pygame.init()

display = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()

img = pygame.image.load("shaders/player_walk2.png")

rect = pygame.Rect(10, 10, 100, 100)
rect1 = pygame.Rect(400, 400, 100, 100)


while True:
    display.fill((0, 0, 0))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            break

    rect1.x += pygame.key.get_pressed()[pygame.K_d] * 10
    rect1.x -= pygame.key.get_pressed()[pygame.K_a] * 10

    rect1.y += pygame.key.get_pressed()[pygame.K_s] * 10
    rect1.y -= pygame.key.get_pressed()[pygame.K_w] * 10

    pygame.draw.rect(display, (100, 0, 0), rect)
    pygame.draw.rect(display, (0, 100, 0), rect1)
    
    print(rect.colliderect(rect1))
    if rect.colliderect(rect1):
        rect1.left = rect.right


    pygame.display.set_caption(f"{clock.get_fps()}")
    pygame.display.update()
    clock.tick(60)
