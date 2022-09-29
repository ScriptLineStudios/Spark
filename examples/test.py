import pygame
pygame.init()

display = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()

img = pygame.image.load("player_walk2.png")

while True:
    display.fill((0, 0, 0))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            break
    
    for i in range(1000):
        display.blit(pygame.transform.scale(img, (100, 100)), (300, 300))

    pygame.display.set_caption(f"{clock.get_fps()}")
    pygame.display.update()
    clock.tick(60)